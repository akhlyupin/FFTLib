/*  Date: 09/22/21
    Author: Artem Khlyupin
*/
#include "TestData.h"
#include <math.h>

Complex_t testComplexData[L];
complex float testComplexData2[L];

void TestData_Init() {
    for (int i = 0; i < L; i++) {  
        testComplexData[i].re = sin(i);
        testComplexData[i].im = 0;
        testComplexData2[i] = sin(i);
    }
}
