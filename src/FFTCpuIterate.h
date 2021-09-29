/*  Date: 09/22/21
    Author: Artem Khlyupin
*/
#ifndef FFT_CPU_ITERATE_H
#define FFT_CPU_ITERATE_H

extern void FFTCpuIterate_Init(int n);
extern void FFTCpuIterate_Close();

extern void FFTCpuIterate_Process(complex float * in, complex float * out, int n);

#endif