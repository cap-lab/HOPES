#include <assert.h>
#include <curand.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <unistd.h>

#include <uem_common.h>


#include "blas.h"
#include "cuda.h"

const char *cublasGetErrorString(cublasStatus_t status) {
    switch (status) {
    case CUBLAS_STATUS_SUCCESS:
        return "CUBLAS_STATUS_SUCCESS";
    case CUBLAS_STATUS_NOT_INITIALIZED:
        return "CUBLAS_STATUS_NOT_INITIALIZED";
    case CUBLAS_STATUS_ALLOC_FAILED:
        return "CUBLAS_STATUS_ALLOC_FAILED";
    case CUBLAS_STATUS_INVALID_VALUE:
        return "CUBLAS_STATUS_INVALID_VALUE";
    case CUBLAS_STATUS_ARCH_MISMATCH:
        return "CUBLAS_STATUS_ARCH_MISMATCH";
    case CUBLAS_STATUS_MAPPING_ERROR:
        return "CUBLAS_STATUS_MAPPING_ERROR";
    case CUBLAS_STATUS_EXECUTION_FAILED:
        return "CUBLAS_STATUS_EXECUTION_FAILED";
    case CUBLAS_STATUS_INTERNAL_ERROR:
        return "CUBLAS_STATUS_INTERNAL_ERROR";
    case CUBLAS_STATUS_NOT_SUPPORTED:
        return "CUBLAS_STATUS_NOT_SUPPORTED";
    case CUBLAS_STATUS_LICENSE_ERROR:
        return "CUBLAS_STATUS_LICENSE_ERROR";
    }
    return "unknown error";
}

void cuda_set_device(int n) {
    cudaError_t status = cudaSetDevice(n);
    check_error(status);
}

int cuda_get_device() {
    int n = 0;
    cudaError_t status = cudaGetDevice(&n);
    check_error(status);
    return n;
}

void error(const char *s) {
    perror(s);
    exit(-1);
}

void check_error(cudaError_t status) {
    // cudaDeviceSynchronize();
    cudaError_t status2 = cudaGetLastError();
    if (status != cudaSuccess) {
        const char *s = cudaGetErrorString(status);
        char buffer[256];
        printf("CUDA Error: %s\n", s);
        assert(0);
        snprintf(buffer, 256, "CUDA Error: %s", s);
        error(buffer);
    }
	
    if (status2 != cudaSuccess) {
        const char *s = cudaGetErrorString(status2);
        char buffer[256];
        printf("CUDA Error Prev: %s\n", s);
        assert(0);
        snprintf(buffer, 256, "CUDA Error Prev: %s", s);
        error(buffer);
    }
}

dim3 cuda_gridsize(size_t n) {
    size_t k = (n - 1) / BLOCK + 1;
    size_t x = k;
    size_t y = 1;
    if (x > 65536) {
        x = ceil(sqrt(k));
        y = (n - 1) / (x * BLOCK) + 1;
    }
    dim3 d = {x, y, 1};
    //printf("n : %ld x : %ld y : %ld x*y*BLOCK : %ld\n", n, x, y, x*y*BLOCK);
    return d;
}

#define MAX_GPU_NUM (16)

static int s_nCudnnHandleInit[MAX_GPU_NUM] = {0};
static cudnnHandle_t s_hCudnnHandle[MAX_GPU_NUM];

void cudnn_handle_create() {
	int nCount = 0;
	int nLoop = 0;
	cudaError_t status;
	status = cudaGetDeviceCount(&nCount);
	check_error(status);

	nCount = MIN(nCount, MAX_GPU_NUM);
	
	for(nLoop = 0 ; nLoop < nCount; nLoop++)
	{
		cudaSetDevice(nLoop);
	    cudnnCreate(&s_hCudnnHandle[nLoop]);
		s_nCudnnHandleInit[nLoop] = 1;
	}
}

static int s_nCublasHandleInit[MAX_GPU_NUM] = {0};
static cublasHandle_t s_hCublasHandle[MAX_GPU_NUM];

void blas_handle_create() {
	int nCount = 0;
	int nLoop = 0;
	cudaError_t status;
	status = cudaGetDeviceCount(&nCount);
	check_error(status);

	nCount = MIN(nCount, MAX_GPU_NUM);

	for(nLoop = 0 ; nLoop < nCount; nLoop++)
	{
        cublasCreate(&s_hCublasHandle[nLoop]);
		s_nCublasHandleInit[nLoop] = 1;
	}
}

cudnnHandle_t cudnn_handle() {
    int i = cuda_get_device();
    //int i = 0;
    while (!s_nCudnnHandleInit[i]) {
		usleep(10000);
    }
    return s_hCudnnHandle[i];
}

cublasHandle_t blas_handle() {
    int i = cuda_get_device();
    while (!s_nCublasHandleInit[i]) {
        usleep(10000);
    }
    return s_hCublasHandle[i];
}

float *cuda_make_array(float *x, size_t n) {
    float *x_gpu;
    size_t size = sizeof(float) * n;
    cudaStream_t stream;
    cudnnGetStream(cudnn_handle(), &stream);

    cudaError_t status = cudaMalloc((void **)&x_gpu, size);
    check_error(status);
    if (x) {
        status = cudaMemcpyAsync(x_gpu, x, size, cudaMemcpyHostToDevice, stream);
        check_error(status);
    } else {
        check_error(cudaMemsetAsync(x_gpu, 0, size, stream));
    }
    if (!x_gpu)
        error("Cuda malloc failed\n");
    return x_gpu;
}

void cuda_random(float *x_gpu, size_t n) {
    static curandGenerator_t gen[16];
    static int init[16] = {0};
    int i = cuda_get_device();
    if (!init[i]) {
        curandCreateGenerator(&gen[i], CURAND_RNG_PSEUDO_DEFAULT);
        curandSetPseudoRandomGeneratorSeed(gen[i], time(0));
        init[i] = 1;
    }
    curandGenerateUniform(gen[i], x_gpu, n);
    check_error(cudaPeekAtLastError());
}

// float cuda_compare(float *x_gpu, float *x, size_t n, char *s)
//{
//    float *tmp = calloc(n, sizeof(float));
//    cuda_pull_array(x_gpu, tmp, n);
//    //int i;
//    //for(i = 0; i < n; ++i) printf("%f %f\n", tmp[i], x[i]);
//    axpy_cpu(n, -1, x, 1, tmp, 1);
//    float err = dot_cpu(n, tmp, 1, tmp, 1);
//    printf("Error %s: %f\n", s, sqrt(err/n));
//    free(tmp);
//    return err;
//}

int *cuda_make_int_array(int *x, size_t n) {
    int *x_gpu;
    size_t size = sizeof(int) * n;
    cudaError_t status = cudaMalloc((void **)&x_gpu, size);
    check_error(status);
    if (x) {
        status = cudaMemcpy(x_gpu, x, size, cudaMemcpyHostToDevice);
        check_error(status);
    }
    if (!x_gpu)
        error("Cuda malloc failed\n");
    return x_gpu;
}

void cuda_free(float *x_gpu) {
    cudaError_t status = cudaFree(x_gpu);
    check_error(status);
}

void cuda_push_array(float *x_gpu, float *x, size_t n) {
    size_t size = sizeof(float) * n;
    cudaStream_t stream;
    cudnnGetStream(cudnn_handle(), &stream);
    cudaError_t status = cudaMemcpyAsync(x_gpu, x, size, cudaMemcpyHostToDevice, stream);
    check_error(status);
}

void cuda_pull_array(float *x_gpu, float *x, size_t n) {
    size_t size = sizeof(float) * n;
    cudaStream_t stream;
    cudnnGetStream(cudnn_handle(), &stream);
    cudaError_t status = cudaMemcpyAsync(x, x_gpu, size, cudaMemcpyDeviceToHost, stream);
    check_error(status);
}

// float cuda_mag_array(float *x_gpu, size_t n)
//{
//    float *temp = calloc(n, sizeof(float));
//    cuda_pull_array(x_gpu, temp, n);
//    float m = mag_array(temp, n);
//    free(temp);
//    return m;
//}
