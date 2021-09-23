/*  Date: 09/22/21
    Author: Artem Khlyupin
*/
#ifndef TEST_DATA_H
#define TEST_DATA_H

#include "Complex.h"

#define L       8388608
#define ORDER   23

extern float testData[L * 2]; 
extern Complex_t testComplexData[L];

extern void TestData_Init();

#endif