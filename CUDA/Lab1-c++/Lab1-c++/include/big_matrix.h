#ifndef BIG_MATRIX_H
#define BIG_MATRIX_H

#include "../include/matrix.h"
#include "../include/timer.h"
#include <cstdio>
#include <iostream>
#include <iterator>
#include <vector>
#include <random>
#include <omp.h>

class BigMatrix {
public:
    std::vector<Matrix*> data;
    int rows;
    int cols;
    
    int small_rows;
    int small_cols;
    
    BigMatrix(int rows, int cols, int small_rows, int small_cols) : 
        rows(rows), cols(cols), small_rows(small_rows), small_cols(small_cols) {
        data.reserve(rows * cols);
        for (int i = 0; i < rows * cols; ++i) {
            data.push_back(new Matrix(small_rows, small_cols));
        }
    }
    
    ~BigMatrix(){
        for (auto& matrix : data){
            delete matrix;
        }
    }
    
    bool is_equal(const BigMatrix* other, float epsilon = 1e-6f) const {
        if (rows != other->rows || cols != other->cols ||
            small_rows != other->small_rows || small_cols != other->small_cols) {
            std::cout << "Matrix dimensions mismatch!" << std::endl;
            return false;
        }
        
        for (int i = 0; i < rows * cols; i++) {
            if (!data[i]->is_equal(other->data[i], epsilon)) {
                return false;
            }
        }
        
        return true;
    }
    
    static BigMatrix* multiply(const BigMatrix& a, const BigMatrix& b, bool use_simd = false) {
        if (a.cols != b.rows) {
            std::cerr << "Error: Incompatible BigMatrix dimensions for multiplication." << std::endl;
            return nullptr;
        }
        if (a.small_cols != b.small_rows) {
            std::cerr << "Error: Incompatible small matrix dimensions for multiplication." << std::endl;
            return nullptr;
        }
    
        BigMatrix* result = new BigMatrix(a.rows, b.cols, a.small_rows, b.small_cols);
        Timer t;
        t.start();
        
        for (int i = 0; i < a.rows; ++i) {
            for (int j = 0; j < b.cols; ++j) {
                Matrix* sum = new Matrix(a.small_rows, b.small_cols);
                std::fill(sum->data.begin(), sum->data.end(), 0.0f);
    
                for (int k = 0; k < a.cols; ++k) {
                    Matrix* Aik = a.data[i * a.cols + k];
                    Matrix* Bkj = b.data[k * b.cols + j];
    
                    Matrix* mult = nullptr;
                    if (use_simd && a.small_rows == 4 && a.small_cols == 2 && 
                        b.small_rows == 2 && b.small_cols == 4) {
                        mult = Matrix::multiply_simd(Aik, Bkj);
                    } else {
                        mult = Matrix::multiply(Aik, Bkj);
                    }
                    
                    if (mult == nullptr) {
                        std::cerr << "Error: Matrix multiplication failed at block (" << i << "," << j << ")" << std::endl;
                        delete sum;
                        delete result;
                        return nullptr;
                    }
    
                    for (int idx = 0; idx < sum->data.size(); ++idx) {
                        sum->data[idx] += mult->data[idx];
                    }
    
                    delete mult;
                }
    
                result->data[i * result->cols + j] = sum;
            }
        }
        
        uint64_t milliseconds = 0;
        uint64_t cycles = 0;
        t.stop(milliseconds, cycles);
        std::cout << "Time: " << milliseconds << "ms, Cycles: " << cycles << std::endl;
        return result;
    }

    static BigMatrix* multiply_openmp(const BigMatrix& a, const BigMatrix& b) {
        omp_set_num_threads(omp_get_max_threads());
        if (a.cols != b.rows) {
            std::cerr << "Error: Incompatible BigMatrix dimensions for multiplication." << std::endl;
            return nullptr;
        }
        if (a.small_cols != b.small_rows) {
            std::cerr << "Error: Incompatible small matrix dimensions for multiplication." << std::endl;
            return nullptr;
        }
    
        BigMatrix* result = new BigMatrix(a.rows, b.cols, a.small_rows, b.small_cols);
        Timer t;
        t.start();
        bool error = false;
        std::string error_msg;
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        #pragma omp parallel for collapse(2) 
        for (int i = 0; i < a.rows; ++i) {
            for (int j = 0; j < b.cols; ++j) {
                if (error) continue; // Пропускаем итерации, если уже найдена ошибка
                
                Matrix* sum = new Matrix(a.small_rows, b.small_cols);
                std::fill(sum->data.begin(), sum->data.end(), 0.0f);
    
                for (int k = 0; k < a.cols; ++k) {
                    Matrix* Aik = a.data[i * a.cols + k];
                    Matrix* Bkj = b.data[k * b.cols + j];
    
                    Matrix* mult = Matrix::multiply(Aik, Bkj);
                    
                    if (mult == nullptr) {
                        //запись об ошибках будет взаписываться только одним потоком /защита переменных
                        #pragma omp critical
                        {
                            error = true;
                            error_msg = "Error: Matrix multiplication failed at block (" + std::to_string(i) + "," + std::to_string(j) + ")";
                        }
                        delete sum;
                        break;
                    }
    
                    for (int idx = 0; idx < sum->data.size(); ++idx) {
                        sum->data[idx] += mult->data[idx];
                    }
    
                    delete mult;
                }
    
                if (!error) {
                    result->data[i * result->cols + j] = sum;
                } else {
                    delete sum;
                }
            }
        }
        
        if (error) {
            std::cerr << error_msg << std::endl;
            delete result;
            return nullptr;
        }
        
        uint64_t milliseconds = 0;
        uint64_t cycles = 0;
        t.stop(milliseconds, cycles);
        std::cout << "Time: " << milliseconds << "ms, Cycles: " << cycles << std::endl;
        return result;
    }

    static BigMatrix* multiply_simd_openmp(const BigMatrix& a, const BigMatrix& b) {
        omp_set_num_threads(omp_get_max_threads());
        if (a.cols != b.rows) {
            std::cerr << "Error: Incompatible BigMatrix dimensions for multiplication." << std::endl;
            return nullptr;
        }
        if (a.small_cols != b.small_rows) {
            std::cerr << "Error: Incompatible small matrix dimensions for multiplication." << std::endl;
            return nullptr;
        }
    
        BigMatrix* result = new BigMatrix(a.rows, b.cols, a.small_rows, b.small_cols);
        Timer t;
        t.start();
        bool error = false;
        std::string error_msg;
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        #pragma omp parallel for collapse(2)
        for (int i = 0; i < a.rows; ++i) {
            for (int j = 0; j < b.cols; ++j) {
                if (error) continue; // Пропускаем итерации, если уже найдена ошибка
                
                Matrix* sum = new Matrix(a.small_rows, b.small_cols);
                std::fill(sum->data.begin(), sum->data.end(), 0.0f);
    
                for (int k = 0; k < a.cols; ++k) {
                    Matrix* Aik = a.data[i * a.cols + k];
                    Matrix* Bkj = b.data[k * b.cols + j];
    
                    Matrix* mult = nullptr;
                    if (a.small_rows == 4 && a.small_cols == 2 && 
                        b.small_rows == 2 && b.small_cols == 4) {
                        mult = Matrix::multiply_simd(Aik, Bkj);
                    } else {
                        mult = Matrix::multiply(Aik, Bkj);
                    }
                    
                    if (mult == nullptr) {
                        #pragma omp critical
                        {
                            error = true;
                            error_msg = "Error: Matrix multiplication failed at block (" + std::to_string(i) + "," + std::to_string(j) + ")";
                        }
                        delete sum;
                        break;
                    }
    
                    for (int idx = 0; idx < sum->data.size(); ++idx) {
                        sum->data[idx] += mult->data[idx];
                    }
    
                    delete mult;
                }
    
                if (!error) {
                    result->data[i * result->cols + j] = sum;
                } else {
                    delete sum;
                }
            }
        }
        
        if (error) {
            std::cerr << error_msg << std::endl;
            delete result;
            return nullptr;
        }
        
        uint64_t milliseconds = 0;
        uint64_t cycles = 0;
        t.stop(milliseconds, cycles);
        std::cout << "Time: " << milliseconds << "ms, Cycles: " << cycles << std::endl;
        return result;
    }
};

#endif
