/*  Date: 09/22/21
    Author: Artem Khlyupin
*/
#include "Complex.h"

void addComplex(Complex_t * a, Complex_t * b, Complex_t * result) {
	result->re = a->re + b->re;
	result->im = a->im + b->im;
}

void subComplex(Complex_t * a, Complex_t * b, Complex_t * result) {
	result->re = a->re - b->re;
	result->im = a->im - b->im;
}

void mulComplex(Complex_t * a, Complex_t * b, Complex_t * result) {
	result->re = a->re * b->re - a->im * b->im;
	result->im = a->re * b->im + a->im * b->re;

}