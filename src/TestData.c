/*  Date: 09/22/21
    Author: Artem Khlyupin
*/
#include "TestData.h"
#include <math.h>

float testData[L * 2]; 
Complex_t testComplexData[L];

void TestData_Init() {
    for (int i = 0; i < L; i++) {   
        testData[i * 2] = sin(i);
        testData[i * 2 + 1] = 0;
    }
}
