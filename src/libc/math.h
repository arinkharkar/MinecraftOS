#ifndef __MATH_H_
#define __MATH_H_

#include <stdbool.h>

#define INFINITY (1.0/0.0)

#define PI ((float)3.1415926535)

#define EulersNumber 2.71

float pow(float base, float exp);

// calculates y * log₂ of x, implemented in floatarith.S
float ytimeslogx(float x, float y);

// Calculates log₂(x), implemented in floatarith.S
float logbase2x(float x);

// this calculates 2^x - 1, implemented in floatarith.S
float twoxminus1(float x);

float sin(float angle);

float cos(float angle);

float tan(float angle);

static inline bool isnan(float n) { if (n != n) return true; return false; }

static inline float log(float val, float base) { return logbase2x(val) / logbase2x(base); }

static inline float sqrt(float x) { return pow(x, 0.5); }

#endif