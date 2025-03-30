#ifndef MATRIX_H
#define MATRIX_H

#include <stdint.h>
#include <string.h>
#include <video.h>

void* malloc(size_t c);


// matrix "4 square" meaning a 4x4 matrix
typedef float matrix_m4s[16];

typedef float vector4[4];

typedef float* matrix_f;

// multiply 2 4x4 matrices m1 * m2 into dest
void multiply_m4s(matrix_f m1, matrix_f m2, matrix_f dest);

// multiply a 4x4 matrix m1 by a 4x1 vector m2 into dest
void multiply_m4sv(matrix_f m1, vector4 m2, vector4 dest);

// print the matrix
void print_m4s(matrix_f matrix);

// print vector
void print_vec4(vector4 v);

// get the value at row and col of mmatrix
static inline float get_index_m4s(matrix_m4s mmatrix, int row, int col) { 
    return mmatrix[col*4+row]; 
}

// set the value at row and col of mmatrix
static inline void set_index_m4s(matrix_m4s mmatrix, int row, int col, float val) { 
    mmatrix[col*4+row] = val; 
}

class matrix {
public:
    //pointer to the matrix data (stored as a 1d array)
    float* m_data;
    uint32_t rows, columns;

    matrix(int numRows, int numColumns);

    matrix(const matrix& other);

    // Calculates A*B where A is this matrix and B is other
    matrix multiply(matrix other);
    
    matrix add(matrix other);

    // to access
    float* operator[](size_t row) {
        if (row > rows)
            return nullptr;
        return &m_data[row * columns]; // Return pointer to the start of the row
    }

    const float* operator[](size_t row) const {
        if (row > rows)
            return nullptr;
        return &m_data[row * columns]; // Return pointer to the start of the row
    }

    matrix operator*(const matrix& other) {
        // the multiplication is invalid
        if (columns != other.rows)
            return matrix(0,0);
        
        matrix m = matrix(rows, other.columns);

        for (int i = 0; i < m.rows; i++) {
            for (int j = 0; j < m.columns; j++) {
                m.m_data[i * m.columns + j] = m_data[i * columns + 0] * other.m_data[0 * other.columns + j] +
                              m_data[i * columns + 1] * other.m_data[1 * other.columns + j] +
                              m_data[i * columns + 2] * other.m_data[2 * other.columns + j] +
                              m_data[i * columns + 3] * other.m_data[3 * other.columns + j];
            }
        }
        return m;
    }

    void print();
    
};



#endif