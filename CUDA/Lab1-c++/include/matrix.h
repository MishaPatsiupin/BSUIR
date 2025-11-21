#ifndef MATRIX_H
#define MATRIX_H

#include <random>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <immintrin.h>

class Matrix {
public:
    std::vector<float> data;
    int rows;
    int cols;

    Matrix(int rows, int cols) : rows(rows), cols(cols) {
        data.resize(rows * cols);
        
        for (int i = 0; i < rows * cols; i++) {
            if (i % 2 == 1){
                data[i] = 0.0f;
            } else {
                data[i] = 1.0f;
            }
        }
    }

    ~Matrix() {}


    static Matrix* multiply(const Matrix* a, const Matrix* b) {
        if (a->cols != b->rows) {
            std::cerr << "Error: Incompatible matrix sizes for multiplication." << std::endl;
            return nullptr;
        }

        Matrix* result = new Matrix(a->rows, b->cols);

        for (int i = 0; i < result->data.size(); i++) {
            result->data[i] = 0.0f;
        }

        for (int i = 0; i < a->rows; i++) {
            for (int j = 0; j < b->cols; j++) {
                for (int k = 0; k < a->cols; k++) {
                    result->data[i * b->cols + j] += a->data[i * a->cols + k] * b->data[k * b->cols + j];
                }
            }
        }

        return result;
    }

    // SIMD умножение для матриц 4x2 и 2x4
    static Matrix* multiply_simd(const Matrix* a, const Matrix* b) {
        if (a->cols != b->rows) {
            std::cerr << "Error: Incompatible matrix sizes for multiplication." << std::endl;
            return nullptr;
        }

        // Проверяем, что матрицы имеют правильные размеры для нашей SIMD реализации
        if (a->rows != 4 || a->cols != 2 || b->rows != 2 || b->cols != 4) {
            std::cerr << "Error: SIMD multiplication only supports 4x2 * 2x4 matrices." << std::endl;
            return multiply(a, b); // Fallback to regular multiplication
        }

        Matrix* result = new Matrix(4, 4);

        // Загружаем матрицу A (4x2)
        __m128 a_row0 = _mm_loadu_ps(&a->data[0]);  // [a00, a01, x, x]
        __m128 a_row1 = _mm_loadu_ps(&a->data[2]);  // [a10, a11, x, x]  
        __m128 a_row2 = _mm_loadu_ps(&a->data[4]);  // [a20, a21, x, x]
        __m128 a_row3 = _mm_loadu_ps(&a->data[6]);  // [a30, a31, x, x]

        // Загружаем матрицу B (2x4) - по строкам
        __m128 b_row0 = _mm_loadu_ps(&b->data[0]);  // [b00, b01, b02, b03]
        __m128 b_row1 = _mm_loadu_ps(&b->data[4]);  // [b10, b11, b12, b13]

        // Распространяем элементы матрицы A для умножения
        __m128 a0_broadcast = _mm_shuffle_ps(a_row0, a_row0, _MM_SHUFFLE(0, 0, 0, 0)); // [a00, a00, a00, a00]
        __m128 a1_broadcast = _mm_shuffle_ps(a_row0, a_row0, _MM_SHUFFLE(1, 1, 1, 1)); // [a01, a01, a01, a01]
        
        __m128 a2_broadcast = _mm_shuffle_ps(a_row1, a_row1, _MM_SHUFFLE(0, 0, 0, 0)); // [a10, a10, a10, a10]
        __m128 a3_broadcast = _mm_shuffle_ps(a_row1, a_row1, _MM_SHUFFLE(1, 1, 1, 1)); // [a11, a11, a11, a11]
        
        __m128 a4_broadcast = _mm_shuffle_ps(a_row2, a_row2, _MM_SHUFFLE(0, 0, 0, 0)); // [a20, a20, a20, a20]
        __m128 a5_broadcast = _mm_shuffle_ps(a_row2, a_row2, _MM_SHUFFLE(1, 1, 1, 1)); // [a21, a21, a21, a21]
        
        __m128 a6_broadcast = _mm_shuffle_ps(a_row3, a_row3, _MM_SHUFFLE(0, 0, 0, 0)); // [a30, a30, a30, a30]
        __m128 a7_broadcast = _mm_shuffle_ps(a_row3, a_row3, _MM_SHUFFLE(1, 1, 1, 1)); // [a31, a31, a31, a31]

        // Вычисляем результат построчно
        // Row 0: a00 * b_row0 + a01 * b_row1
        __m128 row0 = _mm_add_ps(_mm_mul_ps(a0_broadcast, b_row0), 
                                _mm_mul_ps(a1_broadcast, b_row1));
        
        // Row 1: a10 * b_row0 + a11 * b_row1
        __m128 row1 = _mm_add_ps(_mm_mul_ps(a2_broadcast, b_row0), 
                                _mm_mul_ps(a3_broadcast, b_row1));
        
        // Row 2: a20 * b_row0 + a21 * b_row1
        __m128 row2 = _mm_add_ps(_mm_mul_ps(a4_broadcast, b_row0), 
                                _mm_mul_ps(a5_broadcast, b_row1));
        
        // Row 3: a30 * b_row0 + a31 * b_row1
        __m128 row3 = _mm_add_ps(_mm_mul_ps(a6_broadcast, b_row0), 
                                _mm_mul_ps(a7_broadcast, b_row1));

        // Сохраняем результаты
        _mm_storeu_ps(&result->data[0], row0);
        _mm_storeu_ps(&result->data[4], row1);
        _mm_storeu_ps(&result->data[8], row2);
        _mm_storeu_ps(&result->data[12], row3);

        return result;
    }

    // Функция для проверки эквивалентности результатов
    bool is_equal(const Matrix* other, float epsilon = 1e-6f) const {
        if (rows != other->rows || cols != other->cols) {
            return false;
        }
        
        for (int i = 0; i < data.size(); i++) {
            if (std::abs(data[i] - other->data[i]) > epsilon) {
                return false;
            }
        }
        return true;
    }
    
    
    
};

#endif  