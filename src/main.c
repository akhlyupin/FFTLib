#include <stdio.h>
#include <windows.h>
#include <math.h>
#include <stdbool.h>
#include "TestData.h"
#include "FFTCpuIterate.h"
#include "FFTCpuRecursive.h"

float out[L * 2];
Complex_t outComplex[L];
complex float outComplex2[L];

#define BIT_RES 1//21

bool isFloatEqual(float a, float b) {
    unsigned int * aI = (unsigned int *)&a;
    unsigned int * bI = (unsigned int *)&b;

    if (*aI > *bI) {
        return *aI - *bI < (1 << BIT_RES);
    } 
    return *bI - *aI < (1 << BIT_RES);
}

void isOutEquals() {
    for (int i = 0 ; i < L; i++) {
        if ((!isFloatEqual(out[i * 2], outComplex[i].re)) || 
            (!isFloatEqual(out[i * 2 + 1], outComplex[i].im)) ) {
            printf("Error: %d\n(%f %f)\n(%f %f)\n", 
                i, out[i * 2], out[i * 2 + 1], outComplex[i].re,  outComplex[i].im);
            //return;

        }
    }
    printf("Equals!\n");
}

void isDataEquals(float * d0, float * d1, int n) {
    for (int i = 0 ; i < n; i++) {
        if ( !isFloatEqual(d0[i], d1[i]) ) {
            printf("Error: %d\n(%f %f)\n", i, d0[i], d1[i]);
            //return;
        }
    }
    printf("Equals!\n");
}

int main() {
    TestData_Init();
    FFTCpuIterate_Init();

    long t = GetTickCount();

    printf("Recursive...\n");
    fflush(stdout);
    FFTCpuRecursive_Process(testComplexData, L, outComplex);

    printf("FFT R Period = %d\n", GetTickCount() - t);

    printf("Iterate...\n");
    fflush(stdout);

    t = GetTickCount();
    FFTCpuIterate_Process(out);
    printf("FFT I Period = %d\n", GetTickCount() - t);
    fflush(stdout);
    
    t = GetTickCount();

    printf("Check equals...\n");
    fflush(stdout);

    t = GetTickCount();
    isDataEquals(out, (float *)outComplex, L * 2);
    printf("Period = %d\n", GetTickCount() - t);

    return 0;
}
