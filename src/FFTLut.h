/*  Date: 09/22/21
    Author: Artem Khlyupin
    Overview: FFT with look-up table implementation
*/
#ifndef FFT_LUT_H
#define FFT_LUT_H

#include <complex.h>

extern void FFTLut_Init(int n);
extern void FFTLut_Close();

extern void FFTLut_Process(complex float * in, complex float * out, int n);

#endif