#ifndef __MATH_H_
#define __MATH_H_

#include <stdbool.h>
#include <float.h>


#undef INFINITY
#define INFINITY (1.0/0.0)


constexpr float M_PI = 3.1415926535f;
constexpr float M_TAU = 6.2831853072f;
constexpr float PI = 3.1415926535f;

constexpr float EULERS_NUMBER = 2.7182818f;

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

// uses sin_table.cpp's lookup value to quickly 
constexpr float sin_fast(float angle);

constexpr float cos_fast(float angle);

constexpr static inline bool isnan(float n) { if (n != n) return true; return false; }

static inline float log(float val, float base) { return logbase2x(val) / logbase2x(base); }

constexpr static inline float sqrt(float x) { return pow(x, 0.5); }

#endif