/*  Date: 09/22/21
    Author: Artem Khlyupin
*/
#include "TestData.h"
#include <math.h>

float testData[L * 2]; 
Complex_t testComplexData[L];
complex float testComplexData2[L];

void TestData_Init() {
    for (int i = 0; i < L; i++) {   
        testData[i * 2] = sin(i);
        testData[i * 2 + 1] = 0;
        testComplexData[i].re = sin(i);
        testComplexData[i].im = 0;
        testComplexData2[i] = sin(i);
    }
}
