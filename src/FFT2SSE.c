/*  Date: 09/25/21
    Author: Artem Khlyupin
*/
#include "FFT2SSE.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <xmmintrin.h>
#include "Complex.h"


#define _W(N, k) (cexp(-2.0f * M_PI * I * (float)(k) / (float)(N)))
#define W(N, k) (float complex)(_W(N, k))

data_t ** LUT;

static void ditfft2(data_t * in, data_t * out, int log2stride, int stride, int N) {
    //double kth = -2 * k * M_PI / n;
    //wk.re = cos(kth); wk.im = sin(kth);

	//Complex_t temp0;
	//mulComplex(&wk, &oddFFT[k], &temp0);
	//addComplex(&evenFFT[k], &temp0, &data_out[k]);
	//subComplex(&evenFFT[k], &temp0, &data_out[k + n / 2]);

    if (N == 2) {
        out[0] = in[0] + in[stride];
        out[N/2] = in[0] - in[stride];

    } else if (N == 4) {
        ditfft2(in, out, log2stride + 1, stride << 1, N >> 1);
        ditfft2(in + stride, out + N/2, log2stride + 1, stride << 1, N >> 1);

        //data_t temp0 = out[0] + out[2];
        //data_t temp1 = out[0] - out[2];
        //data_t temp2 = out[1] - I * out[3];
        //data_t temp3 = out[1] + I * out[3];
        //double kth = -2 * k * M_PI / N;
        /*data_t w0 = cos(0) + I * sin(0);
        data_t w1 = cos(-M_PI / 2) + I * sin(-M_PI / 2);*/

        data_t temp0 = out[0] + out[2];
        data_t temp1 = out[0] - out[2];
        data_t temp2 = out[1] - I * out[3];
        data_t temp3 = out[1] + I * out[3];



        if (log2stride) {
            out[0] = creal(temp0) + creal(temp2) * I;
            out[1] = creal(temp1) + creal(temp3) * I;
            out[2] = cimag(temp0) + cimag(temp2) * I;
            out[3] = cimag(temp1) + cimag(temp3) * I;

        } else {
            out[0] = temp0;
            out[2] = temp1;
            out[1] = temp2;
            out[3] = temp3;
        }
        
    } else if (!log2stride) {
        ditfft2(in, out, log2stride + 1, stride << 1, N >> 1);
        ditfft2(in + stride, out + N/2, log2stride + 1, stride << 1, N >> 1);

        for (int k = 0; k < N / 2; k += 4) {
            __m128 Ok_re = _mm_load_ps((float *)&out[k + N / 2]);
            __m128 Ok_im = _mm_load_ps((float *)&out[k + N / 2 + 2]);
            __m128 w_re = _mm_load_ps((float *)&LUT[log2stride][k]);
            __m128 w_im = _mm_load_ps((float *)&LUT[log2stride][k + 2]);
            __m128 Ek_re = _mm_load_ps((float *)&out[k]);
            __m128 Ek_im = _mm_load_ps((float *)&out[k + 2]);

            __m128 wOk_re = _mm_sub_ps(_mm_mul_ps(Ok_re, w_re), _mm_mul_ps(Ok_im, w_im));
            __m128 wOk_im = _mm_add_ps(_mm_mul_ps(Ok_re, w_im), _mm_mul_ps(Ok_im, w_re));
            __m128 out0_re = _mm_add_ps(Ek_re, wOk_re);
            __m128 out0_im = _mm_add_ps(Ek_im, wOk_im);
            __m128 out1_re = _mm_sub_ps(Ek_re, wOk_re);
            __m128 out1_im = _mm_sub_ps(Ek_im, wOk_im);

            _mm_store_ps((float *)(out + k), _mm_unpacklo_ps(out0_re, out0_im));
            _mm_store_ps((float *)(out + k + 2), _mm_unpackhi_ps(out0_re, out0_im));
            _mm_store_ps((float *)(out + k + N / 2), _mm_unpacklo_ps(out1_re, out1_im));
            _mm_store_ps((float *)(out + k + N / 2 + 2), _mm_unpackhi_ps(out1_re, out1_im));

        }
    } else {
        ditfft2(in, out, log2stride + 1, stride << 1, N >> 1);
        ditfft2(in + stride, out + N/2, log2stride + 1, stride << 1, N >> 1);

        for (int k = 0; k < N / 2; k += 4) {
            __m128 Ok_re = _mm_load_ps((float *)&out[k + N / 2]);
            __m128 Ok_im = _mm_load_ps((float *)&out[k + N / 2 + 2]);
            __m128 w_re = _mm_load_ps((float *)&LUT[log2stride][k]);
            __m128 w_im = _mm_load_ps((float *)&LUT[log2stride][k + 2]);
            __m128 Ek_re = _mm_load_ps((float *)&out[k]);
            __m128 Ek_im = _mm_load_ps((float *)&out[k + 2]);

            __m128 wOk_re = _mm_sub_ps(_mm_mul_ps(Ok_re, w_re), _mm_mul_ps(Ok_im, w_im));
            __m128 wOk_im = _mm_add_ps(_mm_mul_ps(Ok_re, w_im), _mm_mul_ps(Ok_im, w_re));

            _mm_store_ps((float *)(out + k), _mm_add_ps(Ek_re, wOk_re));
            _mm_store_ps((float *)(out + k + 2), _mm_add_ps(Ek_im, wOk_im));
            _mm_store_ps((float *)(out + k + N / 2), _mm_sub_ps(Ek_re, wOk_re));
            _mm_store_ps((float *)(out + k + N / 2 + 2), _mm_sub_ps(Ek_im, wOk_im));
        }
    }
}

/*static void ditfft2(data_t * in, data_t * out, int log2stride, int stride, int N) {
    if (N == 2) { 
        out[0] = in[0] + in[stride];
        out[N / 2] = in[0] - in[stride];
    } else {
        ditfft2(in, out, log2stride + 1, stride << 1, N >> 1);
        ditfft2(in + stride, out + N/2, log2stride + 1, stride << 1, N >> 1);

        data_t Ek = out[0];
        data_t Ok = out[N / 2];
        out[0] = Ek + Ok;
        out[N / 2] = Ek - Ok;

        for (int k = 1; k < N / 2; k++) {
            data_t Ek = out[k];
            data_t Ok = out[k + N / 2];
            out[k] = Ek + W(N, k) * Ok;
            out[k + N / 2] = Ek - W(N, k) * Ok;
        }
    }
}*/

void FFT2SSE_Init(int N) {
    #define log2(x) ((int)(log(x)/ log(2)))

    int n_luts = log2(N) - 2;
    LUT = malloc(n_luts * sizeof(data_t *));

    for (int i = 0; i < n_luts; i++) {
        int n = N / pow(2, i);

        LUT[i] = _mm_malloc(n / 2 * sizeof(data_t), 16);

        for (int j = 0; j < n / 2; j += 4) {
            data_t w[4];

            for (int k = 0; k < 4; k++) {
                w[k] = W(n, j + k);
            }

            LUT[i][j + 0] = creal(w[0]) + creal(w[1]) * I;
            LUT[i][j + 1] = creal(w[2]) + creal(w[3]) * I;
            LUT[i][j + 2] = cimag(w[0]) + cimag(w[1]) * I;
            LUT[i][j + 3] = cimag(w[2]) + cimag(w[3]) * I;

        }
    }
}

void FFT2SSE_Process(data_t * in, data_t * out, int N) {
    ditfft2(in, out, 0, 1, N);
}

/*#define L 8388608
data_t in[L], out[L];

int main() {
    fft_init(L);

    for (int i = 0; i < L; i++) {
        in[i] = sin(i);
    }

    long t = GetTickCount();

    ditfft2(in, out, 0, 1, L);

    printf("Period = %lu", GetTickCount() - t);

    return 0;
}*/