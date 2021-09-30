/*  Date: 09/30/21
    Author: Artem Khlyupin
*/
#ifndef FFT_H
#define FFT_H

#include <complex.h>
#include <stdbool.h>

typedef enum {
    FFT_UNKNOWN, FFT_BASIC, FFT_LUT, FFT_SSE
} FFT_Impl_t;

extern bool FFT_Init(FFT_Impl_t type, int n);
extern bool FFT_Close();

extern bool FFT_Process(complex float * in, complex float * out, int n);

#endif