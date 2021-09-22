/*  Date: 09/22/21
    Author: Artem Khlyupin
*/
#include "TestData.h"
#include <math.h>

float testData[L]; 

void TestData_Init() {
    for (int i = 0; i < L; i++) {   
        testData[i] = sin(i);
    }
}
