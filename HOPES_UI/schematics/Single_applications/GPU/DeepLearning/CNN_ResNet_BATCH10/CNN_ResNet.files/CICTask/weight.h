#include <stdio.h>

#ifdef __cplusplus
extern "C"
{
#endif

void init_weight();
float *get_weight(int layer_idx, float *buffer, int buffersize);
void set_weight_pointer();

#ifdef __cplusplus
}
#endif

