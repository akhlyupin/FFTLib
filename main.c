#include <stdio.h>
#include <windows.h>
#include <math.h>

#define L  8388608

float in[L]; 
float out[L];

int rev_temp(int k, int n) {
    int rev = 0;
    int i = 0;

    do {
        rev = (rev << 1) + (k & 1);
        k >>= 1;
    } while (++i < n);

    return rev;
}

int main() {
    for (int i = 0; i < L; i++) {
        in[i] = sin(i);
    }

    long t = GetTickCount();


    for (int i = 0; i < L; i++) {
     	int u = 0;
	int rev = 0;
	int k = i;
	do {
        	rev = (rev << 1) + (k & 1);
        	k >>= 1;
   	} while (++u < 23);

       out[rev] = in[i];
    }

    long period = GetTickCount() - t;
    printf("Period = %d\n", period);
    return 0;
}
