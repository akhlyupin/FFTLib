/*  Date: 09/22/21
    Author: Artem Khlyupin
*/
#include "Complex.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


void FFTCpuRecursive_Process(
	Complex_t * data_in, int n, Complex_t * data_out) {

	if (n == 1) {
		data_out[0] = data_in[0];
		return;
	}

	if (n % 2 != 0) {
		printf("Error. Length % 2 != 0.");
		return;
	}

	// compute FFT of even terms
	Complex_t * d = malloc(sizeof(Complex_t) * n / 2);
    for (int k = 0; k < n / 2; k++) {
            d[k].re = data_in[2 * k].re;
			d[k].im = data_in[2 * k].im;
    }
	Complex_t * evenFFT = malloc(sizeof(Complex_t) * n / 2);
	FFTCpuRecursive_Process(d, n / 2, evenFFT);

    // compute FFT of odd terms
    for (int k = 0; k < n / 2; k++) {
        d[k].re = data_in[2 * k + 1].re;
		d[k].im = data_in[2 * k + 1].im;
    }
	Complex_t * oddFFT = malloc(sizeof(Complex_t) * n / 2);
	FFTCpuRecursive_Process(d, n / 2, oddFFT);

	// combine
    for (int k = 0; k < n / 2; k++) {
            double kth = -2 * k * M_PI / n;
            Complex_t wk;
			wk.re = cos(kth); wk.im = sin(kth);

			Complex_t temp0;
			mulComplex(&wk, &oddFFT[k], &temp0);
			addComplex(&evenFFT[k], &temp0, &data_out[k]);
			subComplex(&evenFFT[k], &temp0, &data_out[k + n / 2]);
    }

	free(d);
	free(evenFFT);
	free(oddFFT);
}
