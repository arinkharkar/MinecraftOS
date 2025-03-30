#include "matrix.h"


matrix::matrix(int numRows, int numColumns) {
    rows = numRows;
    columns = numColumns;
    m_data = (float*)calloc(rows * columns, sizeof(float));
}

matrix::matrix(const matrix& other) {
    rows = other.rows;
    columns = other.columns;
    int dataSize = rows * columns;
    m_data = (float*)calloc(dataSize, sizeof(float));
    memcpy(m_data, other.m_data, dataSize);
}

void matrix::print() {
    print_ch('\n');
    for (int i = 0; i < columns; i++) {
        for (int j = 0; j < rows; j++) {
            print_float(m_data[i * columns + j]);
            print_str(" ");
        }
        print_str("\n");
    }
}

void multiply_m4s (matrix_f m1, matrix_f m2, matrix_f dest)
{
    dest[0]  = m1[0] * m2[0] + m1[4] * m2[1] + m1[8] * m2[2] + m1[12] * m2[3];
    dest[1]  = m1[1] * m2[0] + m1[5] * m2[1] + m1[9] * m2[2] + m1[13] * m2[3];
    dest[2]  = m1[2] * m2[0] + m1[6] * m2[1] + m1[10] * m2[2] + m1[14] * m2[3];
    dest[3]  = m1[3] * m2[0] + m1[7] * m2[1] + m1[11] * m2[2] + m1[15] * m2[3];

    dest[4]  = m1[0] * m2[4] + m1[4] * m2[5] + m1[8] * m2[6] + m1[12] * m2[7];
    dest[5]  = m1[1] * m2[4] + m1[5] * m2[5] + m1[9] * m2[6] + m1[13] * m2[7];
    dest[6]  = m1[2] * m2[4] + m1[6] * m2[5] + m1[10] * m2[6] + m1[14] * m2[7];
    dest[7]  = m1[3] * m2[4] + m1[7] * m2[5] + m1[11] * m2[6] + m1[15] * m2[7];

    dest[8]  = m1[0] * m2[8] + m1[4] * m2[9] + m1[8] * m2[10] + m1[12] * m2[11];
    dest[9]  = m1[1] * m2[8] + m1[5] * m2[9] + m1[9] * m2[10] + m1[13] * m2[11];
    dest[10] = m1[2] * m2[8] + m1[6] * m2[9] + m1[10] * m2[10] + m1[14] * m2[11];
    dest[11] = m1[3] * m2[8] + m1[7] * m2[9] + m1[11] * m2[10] + m1[15] * m2[11];

    dest[12] = m1[0] * m2[12] + m1[4] * m2[13] + m1[8] * m2[14] + m1[12] * m2[15];
    dest[13] = m1[1] * m2[12] + m1[5] * m2[13] + m1[9] * m2[14] + m1[13] * m2[15];
    dest[14] = m1[2] * m2[12] + m1[6] * m2[13] + m1[10] * m2[14] + m1[14] * m2[15];
    dest[15] = m1[3] * m2[12] + m1[7] * m2[13] + m1[11] * m2[14] + m1[15] * m2[15];
};

void multiply_m4sv (matrix_f m1, vector4 m2, vector4 dest)
{
    dest[0] = m1[0] * m2[0] + m1[1] * m2[1] + m1[2] * m2[2] + m1[3] * m2[3];  // x component
    dest[1] = m1[4] * m2[0] + m1[5] * m2[1] + m1[6] * m2[2] + m1[7] * m2[3];  // y component
    dest[2] = m1[8] * m2[0] + m1[9] * m2[1] + m1[10] * m2[2] + m1[11] * m2[3]; // z component
    dest[3] = m1[12] * m2[0] + m1[13] * m2[1] + m1[14] * m2[2] + m1[15] * m2[3]; // w component
};


void print_m4s(matrix_f m) {
    print_ch('\n');
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            print_float(m[i * 4 + j]);
            print_str(" ");
        }
        print_str("\n");
    }
}

void print_vec4(vector4 v) {
    for (int j = 0; j < 4; j++) {
        print_float(v[j]);
        print_str(" ");
    }
}
