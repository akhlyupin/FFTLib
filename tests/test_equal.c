
#include "FFT.h"
#include <stdio.h>

#define L 4

complex float in[] = {1, 2, 3, 4};
complex float out[L];

int main() {
    if (!FFT_Init(FFT_SSE, L)) {
        printf("FFT Init error.\n");
        return 0;
    }

    if (!FFT_Process(in, out, L)) {
        printf("FFT Process error.\n");
        return 0;
    }

    if (!FFT_Close()) {
        printf("FFT Close error.\n");
        return 0;
    }

    for (int i = 0; i < L; i++) {
        printf("i=%d: %f + %f*I\n", i, creal(out[i]), cimag(out[i]));
    }

    return 0;
}