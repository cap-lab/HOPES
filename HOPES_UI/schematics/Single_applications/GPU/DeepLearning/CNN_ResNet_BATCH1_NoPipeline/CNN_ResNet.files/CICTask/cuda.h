#ifndef CUDA_H
#define CUDA_H

#include <cudnn.h>
#include <cublas_v2.h>
#include <cuda.h>
#include <cuda_fp16.h>

#define BLOCK 512

#define CUDNN_CHECK(result)                                                    \
    {                                                                          \
        cudnnStatus_t status = result;                                         \
        if (status != CUDNN_STATUS_SUCCESS) {                                  \
            fprintf(stderr, "Fail : %s\n",                                     \
                    cudnnGetErrorString((cudnnStatus_t)status));               \
            exit(-1);                                                          \
        }                                                                      \
    }
#define CUBLAS_CHECK(result)                                                   \
    {                                                                          \
        cublasStatus_t status = result;                                        \
        if (status != CUBLAS_STATUS_SUCCESS) {                                 \
            fprintf(stderr, "Fail : %s\n", cublasGetErrorString(status));      \
            exit(-1);                                                          \
        }                                                                      \
    }

#if (CUDA_VERSION >= 9000)
typedef struct __half half;
#else
typedef __half half;
#define __hdiv hdiv
#endif

#ifdef __cplusplus
extern "C"
{
#endif

void cuda_push_array(float *x_gpu, float *x, size_t n);
void cuda_pull_array(float *x_gpu, float *x, size_t n);
void cuda_set_device(int n);
const char *cublasGetErrorString(cublasStatus_t status);
void check_error(cudaError_t status);
cublasHandle_t blas_handle();
float *cuda_make_array(float *x, size_t n);
int *cuda_make_int_array(int *x, size_t n);
void cuda_random(float *x_gpu, size_t n);
float cuda_compare(float *x_gpu, float *x, size_t n, char *s);
dim3 cuda_gridsize(size_t n);
void cuda_free(float *x_gpu);

void cudnn_handle_create();
void blas_handle_create();

//__half_raw *cuda_make_array_16(float *not_used, size_t n);
//void cuda_pull_array_16(__half_raw *x_gpu, float *x, size_t n);
//void cuda_push_array_16(__half_raw *x_gpu, float *x, size_t n);
cudnnHandle_t cudnn_handle();

#ifdef __cplusplus
}
#endif

#endif
