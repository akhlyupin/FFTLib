#include <stdio.h>
#include <windows.h>
#include <math.h>
#include <stdbool.h>
#include "TestData.h"
#include "FftLut.h"
#include "FFTBasic.h"
#include "FFT2SSE.h"

complex float outComplex[L];
complex float outComplex1[L];
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
    FftLut_Init(L);
    FFT2SSE_Init(L);

    printf("FFT Basic: ");
    fflush(stdout);

    long t = GetTickCount();
    FFTBasic_Process(testComplexData2, outComplex, L);
    printf("%dms\n", GetTickCount() - t);

    printf("FFT with Look-up table: ");
    fflush(stdout);

    t = GetTickCount();
    FftLut_Process(testComplexData2, outComplex1, L);
    printf("%dms\n", GetTickCount() - t);
    
    printf("FFT with Look-up table and SSE instructions: ");
    fflush(stdout);
    
    t = GetTickCount();
    FFT2SSE_Process(testComplexData2, outComplex2, L);
    printf("%dms\n", GetTickCount() - t);
    
    printf("\nCheck equals...\n");
    fflush(stdout);

    t = GetTickCount();
    isDataEquals((float *)outComplex, (float *)outComplex1, L * 2);
    //isDataEquals(testData, (float *)testComplexData2, L * 2);
    isDataEquals((float *)outComplex, (float *)outComplex2, L * 2);
    printf("Check time period: %dms\n", GetTickCount() - t);

    FftLut_Close();
    return 0;
}
