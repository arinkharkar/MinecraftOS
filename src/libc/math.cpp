#include <minecraftOS.h>
#include <draw.h>
#include "lookup_tables/sin_table.h"
#include "lookup_tables/cos_table.h"
#include "math.h"


// implemented in floatarith.S
EXTERN_C float _internalsin(float angle);

float sin(float angle) {
    // since sin repeats every 2pi, first bring the value down below / at 2pi
    while (angle >= 2*PI)
        angle -= 2*PI;
    // the sin function in x87 instruction set
    return _internalsin(angle);

}

float cos(float angle) {
// the cos function in x87 instruction set
    return sin(angle + PI/2);
}

float tan(float angle) {
    return sin(angle) / cos(angle);
}

int print_int(const int str);
int print_str(const char* str);


constexpr float sin_fast(float angle) {
    // first, since sin(x) repeats every tau, make sure angle is in the domain [0,tau]
    while (angle > M_TAU)
        angle -= M_TAU;
    // redo this for negative angles, now angle is for sure in [0, tau]
    while (angle < 0)
        angle += M_TAU;
    // the table has values for [0, tau/4], lets work out what happens when angle is in 4 situations
    
    // if the angle is already in the [0, tau/4] range, just return the value
    if (angle < M_TAU / 4)
        return sin_lookup_table[(int)((float)(2*SIN_TABLE_VALUE_COUNT) / M_PI * angle)];

    // if the angle is between [tau/4,tau/2], invert the angle by subtracting from the final value
    if (angle < M_TAU / 2)
        return sin_lookup_table[SIN_TABLE_VALUE_COUNT - (int)((float)(2*SIN_TABLE_VALUE_COUNT) / M_PI * (angle - M_TAU / 4))];
    // if the angle is between [tau/2, 3tau/4] return the negative of [0, tau/4]
    if (angle < M_TAU * 0.75)
        return -sin_lookup_table[(int)((float)(2*SIN_TABLE_VALUE_COUNT) / M_PI * (angle - M_PI))];
        
    return -sin_lookup_table[SIN_TABLE_VALUE_COUNT - (int)((float)(2*SIN_TABLE_VALUE_COUNT) / M_PI * (angle - 3 * M_PI / 2))];
}

constexpr float cos_fast(float angle) {
    // first, since cos(x) repeats every tau, make sure angle is in the domain [0,tau]
    while (angle > M_TAU)
        angle -= M_TAU;
    // redo this for negative angles, now angle is for sure in [0, tau]
    while (angle < 0)
        angle += M_TAU;
    // the table has values for [0, tau/4], lets work out what happens when angle is in 4 situations
    
    // if the angle is already in the [0, tau/4] range, just return the value
    if (angle < M_TAU / 4)
        return cos_lookup_table[(int)((float)(2*COS_TABLE_VALUE_COUNT) / M_PI * angle)];

    // if the angle is between [tau/4,tau/2], invert the angle by subtracting from the final value
    if (angle < M_TAU / 2)
        return -cos_lookup_table[COS_TABLE_VALUE_COUNT - (int)((float)(2*COS_TABLE_VALUE_COUNT) / M_PI * (angle - M_TAU / 4))];
    // if the angle is between [tau/2, 3tau/4] return the negative of [0, tau/4]
    if (angle < M_TAU * 0.75)
        return -cos_lookup_table[(int)((float)(2*COS_TABLE_VALUE_COUNT) / M_PI * (angle - M_PI))];
        
    return cos_lookup_table[COS_TABLE_VALUE_COUNT - (int)((float)(2*COS_TABLE_VALUE_COUNT) / M_PI * (angle - 3 * M_PI / 2))];
}