/*  Date: 09/30/21
    Author: Artem Khlyupin
*/
#include "FFT.h"
#include "FFTBasic.h"
#include "FFTLut.h"
#include "FFTSSE.h"

static FFT_Impl_t implType = FFT_UNKNOWN;

bool FFT_Init(FFT_Impl_t type, int n) {
    implType = type;

    switch(implType) {
        case FFT_BASIC:
            return true;
        case FFT_LUT:
            FFTLut_Init(n);
            return true;
        case FFT_SSE:
            FFTSSE_Init(n);
            return true;
        default:
            return false;
    }
}

bool FFT_Close() {
    switch(implType) {
        case FFT_BASIC:
            return true;
        case FFT_LUT:
            FFTLut_Close();
            return true;
        case FFT_SSE:
            FFTSSE_Close();
            return true;
        default:
            return false;
    }
}

bool FFT_Process(complex float * in, complex float * out, int n) {
    switch(implType) {
        case FFT_BASIC:
            FFTBasic_Process(in, out, n);
            return true;
        case FFT_LUT:
            FFTLut_Process(in, out, n);
            return true;
        case FFT_SSE:
            FFTSSE_Process(in, out, n);
            return true;
        default:
            return false;
    }
}
