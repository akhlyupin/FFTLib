/*  Date: 09/22/21
    Author: Artem Khlyupin
*/
#include "TestData.h"
#include <math.h>

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
    float wk[2];
    int n = 1;

    for (int i = 1; i <= ORDER; i++) {
        n *= 2;

        for (int j = 0; j < L / 2 / n; j++) {
            for (int k = 0; k < n / 2; k++) {
                //double kth = -2 * k * M_PI / n;
                wk[0] = kthCosTable[k];
                wk[1] = kthSinTable[k];

                int evenIndex = (j * n + k) * 2;
                int oddIndex = (j * n + k + n / 2) * 2;

                even[0] = out[evenIndex];
                even[1] = out[evenIndex + 1];
                odd[0] = out[oddIndex];
                odd[1]= out[oddIndex + 1];

                //temp = wk * odd
                temp[0] = wk[0] * odd[0] - wk[1] * odd[1];
                temp[1] = wk[0] * odd[1] + wk[1] * odd[0];


                out[evenIndex] = even[0] + temp[0];
                out[evenIndex + 1] = even[1] + temp[1];
                out[oddIndex] = even[0] - temp[0];
                out[oddIndex + 1] = even[1] - temp[1];

            }
        }
    }
}