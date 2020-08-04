#ifndef _ACTIVATE_H_
#define _ACTIVATE_H_

//#include "cuda.h"

#ifdef __cplusplus
extern "C"
{
#endif

void relu_gpu(float *x, int n);
void leaky_relu_gpu(float *x, int n);
void logistic_gpu(float *x, int n);

void relu_grad_gpu(float *x, int n, float *delta);
void leaky_relu_grad_gpu(float *x, int n, float *delta);
void logistic_grad_gpu(float *x, int n, float *delta);

//void relu_gpu_16(__half_raw *x, int n);
//void logistic_gpu_16(__half_raw *x, int n);
//void leaky_relu_gpu_16(__half_raw *x, int n);

#ifdef __cplusplus
}
#endif

#endif /* ifndef _ACTIVATE_H_ */
