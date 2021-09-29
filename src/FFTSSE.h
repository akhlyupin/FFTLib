/*  Date: 09/25/21
    Author: Artem Khlyupin
*/
#ifndef FFT_SSE_H
#define FFT_SSE_H

#include <complex.h>

extern void FFTSSE_Init(int n);
extern void FFTSSE_Close();
extern void FFTSSE_Process(complex float * in, complex float * out, int n);

#endif