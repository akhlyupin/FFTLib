/*  Date: 09/22/21
    Author: Artem Khlyupin
*/
#ifndef TEST_DATA_H
#define TEST_DATA_H

#include "Complex.h"
#include <complex.h>

#define L       8388608

extern Complex_t testComplexData[L];
extern complex float testComplexData2[L];

extern void TestData_Init();

#endif