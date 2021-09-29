/*  Date: 09/22/21
    Author: Artem Khlyupin
    Overview: FFT with look-up table implementation
*/
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <complex.h>

#define log2(x) ((int)(log(x)/ log(2)))

#define _W(N, k) (cexp(-2.0f * M_PI * I * (float)(k) / (float)(N)))
#define W(N, k) (float complex)(_W(N, k))

static complex float * wTable;

void FftLut_Init(int n) {
    wTable = malloc(n / 2 * sizeof(complex float));

    //fill w table
    for (int k = 0; k < n / 2; k++) {
            wTable[k] = W(n, k);
    }
}

void FftLut_Close() {
    free(wTable);
}

static void fft_lut_proc(complex float * in, complex float * out, int stride, int n) {
    if (n == 2) { 
        out[0] = in[0] + in[stride];
        out[1] = in[0] - in[stride];
    } else {
        fft_lut_proc(in, out, stride << 1, n >> 1);
        fft_lut_proc(in + stride, out + n / 2, stride << 1, n >> 1);

        complex float even = out[0];
        complex float odd = out[n / 2];
        out[0] = even + odd;
        out[n / 2] = even - odd;

        for (int k = 1; k < n / 2; k++) {
            even = out[k];
            odd = out[k + n / 2];
            out[k] = even + wTable[k * stride] * odd;
            out[k + n / 2] = even - wTable[k * stride] * odd;
        }
    }
}

void FftLut_Process(complex float * in, complex float * out, int n) {
    fft_lut_proc(in, out, 1, n);
}