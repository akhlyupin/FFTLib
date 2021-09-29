/*  Date: 09/25/21
    Author: Artem Khlyupin
*/
#ifndef FFT_2_SSE_H
#define FFT_2_SSE_H

#include <complex.h>

typedef complex float data_t;

extern void FFT2SSE_Init(int N);
extern void FFT2SSE_Process(data_t * in, data_t * out, int N);
#endif