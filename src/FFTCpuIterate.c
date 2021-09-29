/*  Date: 09/22/21
    Author: Artem Khlyupin
*/
#include "TestData.h"
#include <math.h>
#include <stdio.h>

static float kthSinTable[L / 2];
static float kthCosTable[L / 2];
static int revTable[L];

static void fill_kth() {
    for (int k = 0; k < L / 2; k++) {
            double kth = -2 * k * M_PI / L;
            kthCosTable[k] = cos(kth);
            kthSinTable[k] = sin(kth);
    }
}

static void fill_revTable() {
    for (int i = 0; i < L; i++) {
     	int u = 0;
	    int rev = 0;
	    int k = i;
 
        do {
                rev = (rev << 1) + (k & 1);
                k >>= 1;
        } while (++u < ORDER);

        revTable[i] = rev;
    }
}

void FFTCpuIterate_Init() {
    fill_kth();
    fill_revTable();
}

void FFTCpuIterate_Process(float * out) {
    for (int i = 0; i < L; i++) {
        out[revTable[i] * 2] = testData[i * 2];
        out[revTable[i] * 2 + 1] = testData[i * 2 + 1];
    }

    float temp[2];
    float even[2];
    float odd[2];
    int factor = ORDER;

    for (int n = 2; n <= L; n <<= 1) {
        factor--;

        for (int j = 0; j < L / n; j++) {
            for (int k = 0; k < n / 2; k++) {

                int evenIndex = (j * n + k) * 2;
                int oddIndex = evenIndex + n;       //(j * n + k + n / 2) * 2;

                even[0] = out[evenIndex];
                even[1] = out[evenIndex + 1];
                odd[0] = out[oddIndex];
                odd[1]= out[oddIndex + 1];

                //temp = wk * odd
                temp[0] = kthCosTable[k << factor] * odd[0] - kthSinTable[k << factor] * odd[1];
                temp[1] = kthCosTable[k << factor] * odd[1] + kthSinTable[k << factor] * odd[0];

                out[evenIndex] =        even[0] + temp[0];
                out[evenIndex + 1] =    even[1] + temp[1];
                out[oddIndex] =         even[0] - temp[0];
                out[oddIndex + 1] =     even[1] - temp[1];
            }
        }
    }
}