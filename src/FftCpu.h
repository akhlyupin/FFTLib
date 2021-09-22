/*  Date: 09/22/21
    Author: Artem Khlyupin
*/
#ifndef FFT_CPU_H
#define FFT_CPU_H

#include "Complex.h"

extern void fftCpu(Complex_t * data_in, int n, Complex_t * data_out);

#endif