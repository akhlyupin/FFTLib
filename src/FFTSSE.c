/*  Date: 09/25/21
    Author: Artem Khlyupin
*/
#include <math.h>
#include <complex.h>
#include <stdio.h>
#include <stdlib.h>
#include <xmmintrin.h>


#define _W(N, k) (cexp(-2.0f * M_PI * I * (float)(k) / (float)(N)))
#define W(N, k) (float complex)(_W(N, k))

complex float ** LUT;
int length = 0;

static void fft_sse(complex float * in, complex float * out, int log2stride, int stride, int N) {
    if (N == 2) {
        out[0] = in[0] + in[stride];
        out[N/2] = in[0] - in[stride];

    } else if (N == 4) {
        fft_sse(in, out, log2stride + 1, stride << 1, N >> 1);
        fft_sse(in + stride, out + N/2, log2stride + 1, stride << 1, N >> 1);

        complex float t0 = out[0] + out[2];
        complex float t1 = out[0] - out[2];
        complex float t2 = out[1] - I * out[3];
        complex float t3 = out[1] + I * out[3];

        if (log2stride) {
            out[0] = creal(t0) + creal(t2) * I;
            out[1] = creal(t1) + creal(t3) * I;
            out[2] = cimag(t0) + cimag(t2) * I;
            out[3] = cimag(t1) + cimag(t3) * I;

        } else {
            out[0] = t0;
            out[2] = t1;
            out[1] = t2;
            out[3] = t3;
        }
        
    } else if (!log2stride) {
        fft_sse(in, out, log2stride + 1, stride << 1, N >> 1);
        fft_sse(in + stride, out + N/2, log2stride + 1, stride << 1, N >> 1);

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
        fft_sse(in, out, log2stride + 1, stride << 1, N >> 1);
        fft_sse(in + stride, out + N/2, log2stride + 1, stride << 1, N >> 1);

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

void FFTSSE_Init(int n) {
    #define log2(x) ((int)(log(x)/ log(2)))

    length = n;
    int n_luts = log2(n) - 2;
    LUT = malloc(n_luts * sizeof(complex float *));

    for (int i = 0; i < n_luts; i++) {
        int ni = n / pow(2, i);

        LUT[i] = _mm_malloc(ni / 2 * sizeof(complex float), 16);

        for (int j = 0; j < ni / 2; j += 4) {
            complex float w[4];

            for (int k = 0; k < 4; k++) {
                w[k] = W(ni, j + k);
            }

            LUT[i][j + 0] = creal(w[0]) + creal(w[1]) * I;
            LUT[i][j + 1] = creal(w[2]) + creal(w[3]) * I;
            LUT[i][j + 2] = cimag(w[0]) + cimag(w[1]) * I;
            LUT[i][j + 3] = cimag(w[2]) + cimag(w[3]) * I;

        }
    }
}

void FFTSSE_Close() {
    if (length < 8) return;

    int n_luts = log2(length) - 2;
    
    for (int i = 0; i < n_luts; i++) {
        _mm_free(LUT[i]);
    }
    free(LUT);
}

void FFTSSE_Process(complex float * in, complex float * out, int n) {
    fft_sse(in, out, 0, 1, n);
}