#include "math.h"


// implemented in floatarith.S
float _internalsin(float angle);

float sin(float angle) {
    // since sin repeats every 2pi, first bring the value down below / at 2pi
    while (angle >= 2*PI)
        angle -= 2*PI;
    return _internalsin(angle);

}

float cos(float angle) {
    return sin(angle - PI/2);
}

float tan(float angle) {
    return sin(angle) / cos(angle);
}