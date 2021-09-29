/*  Date: 09/22/21
    Author: Artem Khlyupin
*/
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <complex.h>

#define _W(N, k) (cexp(-2.0f * M_PI * I * (float)(k) / (float)(N)))
#define W(N, k) (float complex)(_W(N, k))

static void fft_basic(complex float * in, complex float * out, int stride, int n) {
	if (n == 2) { 
        out[0] = in[0] + in[stride];
        out[1] = in[0] - in[stride];
    } else {
        fft_basic(in, out, stride << 1, n >> 1);
        fft_basic(in + stride, out + n / 2, stride << 1, n >> 1);

        complex float even = out[0];
        complex float odd = out[n / 2];
        out[0] = even + odd;
        out[n / 2] = even - odd;

        for (int k = 1; k < n / 2; k++) {
            even = out[k];
            odd = out[k + n / 2];
            out[k] = even + W(n, k) * odd;
            out[k + n / 2] = even - W(n, k) * odd;
        }
    }
}
void FFTCpuRecursive_Process(complex float * in, complex float * out, int n) {
	fft_basic(in, out, 1,n);
}
