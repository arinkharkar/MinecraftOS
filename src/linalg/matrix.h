#ifndef MATRIX_H
#define MATRIX_H

#include <stdint.h>
#include <string.h>
#include <video.h>

void* malloc(size_t c);


// matrix "4 square" meaning a 4x4 matrix
typedef float matrix_m4s[16];

// vectors of different sizes
typedef float vector4[4];
typedef float vector3[3];
typedef float vector2[2];

typedef float* matrix_f;

// compute dot product of 2 vector4s
static inline float dotvec4(const vector4 a, const vector4 b) {
    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2] + a[3]*b[3];
}

// compute dot product of 2 vector3s
static inline float dotvec3(const vector3 a, const vector3 b) {
    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}

// multiply 2 4x4 matrices m1 * m2 into dest
void multiply_m4s(const matrix_f m1, const matrix_f m2, matrix_f dest);

// multiply a 4x4 matrix m1 by a 4x1 vector m2 into dest
void multiply_m4sv(const matrix_f m1, const vector4 m2, vector4 dest);

// print the matrix
void print_m4s(const matrix_f matrix);

// print vector
void print_vec4(const vector4 v);

// get the value at row and col of mmatrix
static inline float get_index_m4s(const matrix_m4s mmatrix, const int row, const int col) { 
    return mmatrix[col*4+row]; 
}

// set the value at row and col of mmatrix
static inline void set_index_m4s(matrix_m4s mmatrix, const int row, const int col, const float val) { 
    mmatrix[col*4+row] = val; 
}

static inline bool m4s_equal(const matrix_m4s m1, const matrix_m4s m2) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if(m1[j+i*4] != m2[j+i*4])
                return false;
        }
    }
    return true;
}



#endif