/*  Date: 09/22/21
    Author: Artem Khlyupin
*/
#ifndef COMPLEX_H
#define COMPLEX_H

typedef struct {
	float re;
	float im;
} Complex_t;

extern void addComplex(Complex_t * a, Complex_t * b, Complex_t * result);
extern void subComplex(Complex_t * a, Complex_t * b, Complex_t * result);
extern void mulComplex(Complex_t * a, Complex_t * b, Complex_t * result);

#endif