#ifndef BLAS_H
#define BLAS_H

#include "cuda.h"

#ifdef __cplusplus
extern "C"
{
#endif

// CPU
void copy_cpu(int N, float *X, int INCX, float *Y, int INCY);
void axpy_cpu(int N, float ALPHA, float *X, int INCX, float *Y, int INCY);
void scal_cpu(int N, float ALPHA, float *X, int INCX);
// [TODO] remove unnecessary parameters and replace those with DEFINE values.
void mean_cpu(float *x, int filters, int spatial, float *mean);
void variance_cpu(float *x, float *mean, int filters, int spatial,
                  float *variance);
void normalize_cpu(float *x, float *mean, float *variance, int filters,
                   int spatial);
void mean_delta_cpu(float *delta, float *variance, int filters, int spatial,
                    float *mean_delta);
void variance_delta_cpu(float *x, float *delta, float *mean, float *variance,
                        int filters, int spatial, float *variance_delta);
void normalize_delta_cpu(float *x, float *mean, float *variance,
                         float *mean_delta, float *variance_delta, int filters,
                         int spatial, float *delta);
void softmax(float *input, int n, float temp, int stride, float *output);
void softmax_cpu(float *input, int n, int batch, int batch_offset, int groups,
                 int group_offset, int stride, float temp, float *output);

// GPU
void fill_gpu(int N, float ALPHA, float *X, int INCX);
void axpy_gpu(int N, float ALPHA, float *X, int INCX, float *Y, int INCY);
void axpy_gpu_offset(int N, float ALPHA, float *X, int OFFX, int INCX, float *Y,
                     int OFFY, int INCY);
void copy_gpu(int N, float *X, int INCX, float *Y, int INCY);
void copy_gpu_offset(int N, float *X, int OFFX, int INCX, float *Y, int OFFY,
                     int INCY);
void add_gpu(int N, float ALPHA, float *X, int INCX);
void supp_gpu(int N, float ALPHA, float *X, int INCX);
void mask_gpu(int N, float *X, float mask_num, float *mask);
void scale_mask_gpu(int N, float *X, float mask_num, float *mask, float scale);
void const_gpu(int N, float ALPHA, float *X, int INCX);
void pow_gpu(int N, float ALPHA, float *X, int INCX, float *Y, int INCY);
void mul_gpu(int N, float *X, int INCX, float *Y, int INCY);

void constrain_gpu(int N, float ALPHA, float *X, int INCX);
void scal_gpu(int N, float ALPHA, float *X, int INCX);

void mean_gpu(float *x, int batch, int filters, int spatial, float *mean);
void variance_gpu(float *x, float *mean, int batch, int filters, int spatial,
                  float *variance);
void normalize_gpu(float *x, float *mean, float *variance, int batch,
                   int filters, int spatial);

void normalize_delta_gpu(float *x, float *mean, float *variance,
                         float *mean_delta, float *variance_delta, int batch,
                         int filters, int spatial, float *delta);

void fast_mean_delta_gpu(float *delta, float *variance, int batch, int filters,
                         int spatial, float *mean_delta);
void fast_variance_delta_gpu(float *x, float *delta, float *mean,
                             float *variance, int batch, int filters,
                             int spatial, float *variance_delta);

void fast_variance_gpu(float *x, float *mean, int batch, int filters,
                       int spatial, float *variance);
void fast_mean_gpu(float *x, int batch, int filters, int spatial, float *mean);
void shortcut_gpu(int batch, int w1, int h1, int c1, float *add, int w2, int h2,
                  int c2, float *out);
void scale_bias_gpu(float *output, float *biases, int batch, int n, int size);
void backward_scale_gpu(float *x_norm, float *delta, int batch, int n, int size,
                        float *scale_updates);
void scale_bias_gpu(float *output, float *biases, int batch, int n, int size);
void add_bias_gpu(float *output, float *biases, int batch, int n, int size);
void backward_bias_gpu(float *bias_updates, float *delta, int batch, int n,
                       int size);

void smooth_l1_gpu(int n, float *pred, float *truth, float *delta,
                   float *error);
void l2_gpu(int n, float *pred, float *truth, float *delta, float *error);
void l1_gpu(int n, float *pred, float *truth, float *delta, float *error);
void weighted_delta_gpu(float *a, float *b, float *s, float *da, float *db,
                        float *ds, int num, float *dc);
void weighted_sum_gpu(float *a, float *b, float *s, int num, float *c);
void mult_add_into_gpu(int num, float *a, float *b, float *c);
void inter_gpu(int NX, float *X, int NY, float *Y, int B, float *OUT);
void deinter_gpu(int NX, float *X, int NY, float *Y, int B, float *OUT);

void reorg_gpu(float *x, int w, int h, int c, int batch, int stride,
               int forward, float *out);

void softmax_gpu(float *input, int n, int batch, int batch_offset, int groups,
                 int group_offset, int stride, float temp, float *output);
void adam_update_gpu(float *w, float *d, float *m, float *v, float B1, float B2,
                     float eps, float decay, float rate, int n, int batch,
                     int t);
void adam_gpu(int n, float *x, float *m, float *v, float B1, float B2,
              float rate, float eps, int t);

void flatten_gpu(float *x, int spatial, int layers, int batch, int forward,
                 float *out);
// void softmax_tree(float *input, int spatial, int batch, int stride, float
// temp, float *output, tree hier);

#ifdef __cplusplus
}
#endif

#endif
