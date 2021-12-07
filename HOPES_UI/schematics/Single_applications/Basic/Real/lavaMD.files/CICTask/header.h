#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#define DOT(A, B) ((A.x) * (B.x) + (A.y) * (B.y) + (A.z) * (B.z))
#define fp float
#define NUMBER_PAR_PER_BOX 100
#define NUMBER_THREADS 128

typedef struct { fp x, y, z; } THREE_VECTOR;
typedef struct { fp v, x, y, z; } FOUR_VECTOR;
typedef struct nei_str {

    // neighbor box
    int x, y, z;
    int number;
    long offset;

} nei_str;
typedef struct box_str {

    // home box
    int x, y, z;
    int number;
    long offset;

    // neighbor boxes
    int nn;
    nei_str nei[26];

} box_str;

typedef struct par_str { fp alpha; } par_str;
typedef struct dim_str {

    // input arguments
    int cur_arg;
    int arch_arg;
    int boxes1d_arg;

    // system memory
    long number_boxes;
    long box_mem;
    long space_elem;
    long space_mem;
    long space_mem2;

} dim_str;
