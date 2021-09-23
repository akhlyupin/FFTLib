#include <stdio.h>
#include <windows.h>
#include <math.h>
#include "TestData.h"
#include "FFTCpuIterate.h"

float out[L * 2];


int main() {
    TestData_Init();
    FFTCpuIterate_Init();

    long t = GetTickCount();

    FFTCpuIterate_Process(out);

    long period = GetTickCount() - t;
    printf("Period = %d\n", period);

    return 0;
}
