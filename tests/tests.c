/*  Date: 09/30/21
    Author: Artem Khlyupin
*/

#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "FFTLut.h"
#include "FFTBasic.h"
#include "FFTSSE.h"

#if defined(__APPLE__) || defined(__linux__)
    #include <time.h>
    #include <stdint.h>
#elif defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
#endif


#define L       8388608

complex float outData0[L];
complex float outData1[L];
complex float outData2[L];

complex float testData[L];

static void initTestData() {
    for (int i = 0; i < L; i++) {
        testData[i] = sin(i);
    }
}

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

#if defined(__APPLE__) || defined(__linux__)
long GetTickCount() {
    struct timespec ts;
    clock_gettime( CLOCK_REALTIME, &ts );
    return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}
#endif

int main() {
    initTestData();
    FFTLut_Init(L);
    FFTSSE_Init(L);

    printf("FFT Basic: ");
    fflush(stdout);

    long t = GetTickCount();
    FFTBasic_Process(testData, outData0, L);
    printf("%ldms\n", GetTickCount() - t);

    printf("FFT with Look-up table: ");
    fflush(stdout);

    t = GetTickCount();
    FFTLut_Process(testData, outData1, L);
    printf("%ldms\n", GetTickCount() - t);
    
    printf("FFT with Look-up table and SSE instructions: ");
    fflush(stdout);
    
    t = GetTickCount();
    FFTSSE_Process(testData, outData2, L);
    printf("%ldms\n", GetTickCount() - t);
    
    printf("\nCheck equals...\n");
    fflush(stdout);

    t = GetTickCount();
    isDataEquals((float *)outData0, (float *)outData1, L * 2);
    isDataEquals((float *)outData0, (float *)outData2, L * 2);
    printf("Check time period: %ldms\n", GetTickCount() - t);
    fflush(stdout);

    FFTLut_Close();
    FFTSSE_Close();
    printf("Close.\n");
    return 0;
}
