#include <cuda_runtime.h>
#include <curand.h>
#include <cublas_v2.h>
#include <cuda_fp16.h>

//#include "cuda.h"

extern "C" {
#include "cuda.h"
#include "activate.h"
}


__device__ float _relu_gpu(float x) {return x*(x>0);}
__device__ float _relu_differential_gpu(float x) {return (x>0) ? x : .1*x;}

__device__ float _leaky_relu_gpu(float x) {return (x>0) ? x : .1*x;}
__device__ float _leaky_relu_differential_gpu(float x) {return (x>0) ? 1 : .1;}

__device__ float _logistic_gpu(float x){return 1./(1. + exp(-x));}
__device__ float _logistic_differential_gpu(float x){return (1-x)*x;}



__global__ void relu_kernel(float *x, int n) {
    int i = (blockIdx.x + blockIdx.y * gridDim.x) * blockDim.x + threadIdx.x;
    if (i < n) {
        x[i] = _relu_gpu(x[i]);
    }
}


__global__ void leaky_kernel(float *x, int n) {
    int i = (blockIdx.x + blockIdx.y * gridDim.x) * blockDim.x + threadIdx.x;
    if (i < n) {
        x[i] = _leaky_relu_gpu(x[i]);
    }
}

__global__ void logistic_kernel(float *x, int n) {
    int i = (blockIdx.x + blockIdx.y * gridDim.x) * blockDim.x + threadIdx.x;
    if (i < n) {
        x[i] = _logistic_gpu(x[i]);
    }
}

__global__ void gradient_relu_kernel(float *x, int n, float *delta) {
    int i = (blockIdx.x + blockIdx.y * gridDim.x) * blockDim.x + threadIdx.x;
    if (i < n) {
        delta[i] *= _relu_differential_gpu(x[i]);
    }
}

__global__ void gradient_leaky_kernel(float *x, int n, float *delta) {
    int i = (blockIdx.x + blockIdx.y * gridDim.x) * blockDim.x + threadIdx.x;
    if (i < n) {
        delta[i] *= _leaky_relu_differential_gpu(x[i]);
    }
}

__global__ void gradient_logistic_kernel(float *x, int n, float *delta) {
    int i = (blockIdx.x + blockIdx.y * gridDim.x) * blockDim.x + threadIdx.x;
    if (i < n) {
        delta[i] *= _logistic_differential_gpu(x[i]);
    }
}

extern "C" void relu_gpu(float *x, int n) {
    cudaStream_t stream;
    cudnnGetStream(cudnn_handle(), &stream);
    relu_kernel<<<cuda_gridsize(n), BLOCK, 0, stream>>>(x, n);
}

extern "C" void leaky_relu_gpu(float *x, int n) {
    cudaStream_t stream;
    cudnnGetStream(cudnn_handle(), &stream);
    leaky_kernel<<<cuda_gridsize(n), BLOCK, 0>>>(x, n);
    //leaky_kernel<<<cuda_gridsize(n), BLOCK, 0, stream>>>(x, n);
	check_error(cudaSuccess);
}

extern "C" void logistic_gpu(float *x, int n) {
    logistic_kernel<<<cuda_gridsize(n), BLOCK>>>(x, n);
}

extern "C" void relu_grad_gpu(float *x, int n, float *delta) {
    gradient_relu_kernel<<<cuda_gridsize(n), BLOCK>>>(x, n, delta);
}

extern "C" void leaky_relu_grad_gpu(float *x, int n, float *delta) {
    gradient_leaky_kernel<<<cuda_gridsize(n), BLOCK>>>(x, n, delta);
}

extern "C" void logistic_grad_gpu(float *x, int n, float *delta) {
    gradient_logistic_kernel<<<cuda_gridsize(n), BLOCK>>>(x, n, delta);
}
