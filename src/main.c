#include <stdio.h>
#include <windows.h>
#include <math.h>
#include <stdbool.h>
#include "TestData.h"
#include "FFTCpuIterate.h"
#include "FFTCpuRecursive.h"

float out[L * 2];
Complex_t outComplex[L];

bool isFloatEqual(float a, float b) {
    unsigned int * aI = (unsigned int *)&a;
    unsigned int * bI = (unsigned int *)&b;

    if (*aI > *bI) {
        return *aI - *bI < 2;
    } 
    return *bI - *aI < 2;
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

int main() {
    TestData_Init();
    FFTCpuIterate_Init();

    long t = GetTickCount();

    printf("Recursive...\n");
    fflush(stdout);
    FFTCpuRecursive_Process(testComplexData, L, outComplex);

    printf("FFT R Period = %d\n", GetTickCount() - t);
    fflush(stdout);
    

    printf("Iterate...\n");
    fflush(stdout);
    t = GetTickCount();
    FFTCpuIterate_Process(out);

    printf("FFT I Period = %d\n", GetTickCount() - t);
    fflush(stdout);
    t = GetTickCount();

    printf("Check equals...\n");
    fflush(stdout);
    isOutEquals();


    printf("Period = %d\n", GetTickCount() - t);

    return 0;
}
