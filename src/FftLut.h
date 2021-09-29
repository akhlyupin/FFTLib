/*  Date: 09/22/21
    Author: Artem Khlyupin
    Overview: FFT with look-up table implementation
*/
#ifndef FFT_LUT_H
#define FFT_LUT_H

extern void FftLut_Init(int n);
extern void FftLut_Close();

extern void FftLut_Process(complex float * in, complex float * out, int n);

#endif