#include "../include/big_matrix.h"
#include <iostream>
#include <vector>

int main() {
    std::vector<uint64_t> times(4);
    uint64_t milliseconds, cycles;
    std::cout << "Number of OpenMP threads: " << omp_get_max_threads() << std::endl;
    std::cout << "\n=== Performance testing ===" << std::endl;
    BigMatrix A(300, 310, 4, 2);
    BigMatrix B(310, 300, 2, 4);

    std::cout << "Testing regular multiplication..." << std::endl;
    BigMatrix* C_regular = BigMatrix::multiply(A, B, false);
    if (C_regular != nullptr) {
        std::cout << "Regular multiplication succeeded." << std::endl;
    }

    std::cout << "\nTesting SIMD multiplication..." << std::endl;
    BigMatrix* C_simd = BigMatrix::multiply(A, B, true);
    if (C_simd != nullptr) {
        std::cout << "SIMD multiplication succeeded." << std::endl;
    }

    std::cout << "\nTesting OpenMP regular multiplication..." << std::endl;
    BigMatrix* C_omp_regular = BigMatrix::multiply_openmp(A, B);
    if (C_omp_regular != nullptr) {
        std::cout << "OpenMP regular multiplication succeeded." << std::endl;
    }

    std::cout << "\nTesting OpenMP SIMD multiplication..." << std::endl;
    BigMatrix* C_omp_simd = BigMatrix::multiply_simd_openmp(A, B);
    if (C_omp_simd != nullptr) {
        std::cout << "OpenMP SIMD multiplication succeeded." << std::endl;
    }

    // Сравниваем результаты
    if (C_regular != nullptr && C_simd != nullptr && C_omp_regular != nullptr && C_omp_simd != nullptr) {
        std::cout << "\n=== Comparing results ===" << std::endl;
        bool regular_simd_equal = C_regular->is_equal(C_simd);
        std::cout << "Regular and SIMD are " << (regular_simd_equal ? "EQUAL" : "DIFFERENT") << std::endl;
        
        bool regular_omp_regular_equal = C_regular->is_equal(C_omp_regular);
        std::cout << "Regular and OpenMP regular are " << (regular_omp_regular_equal ? "EQUAL" : "DIFFERENT") << std::endl;
        
        bool simd_omp_simd_equal = C_simd->is_equal(C_omp_simd);
        std::cout << "SIMD and OpenMP SIMD are " << (simd_omp_simd_equal ? "EQUAL" : "DIFFERENT") << std::endl;
        
        bool regular_omp_simd_equal = C_regular->is_equal(C_omp_simd);
        std::cout << "Regular and OpenMP SIMD are " << (regular_omp_simd_equal ? "EQUAL" : "DIFFERENT") << std::endl;
    }

    // Освобождаем память
    if (C_regular != nullptr) {
        delete C_regular;
    }
    if (C_simd != nullptr) {
        delete C_simd;
    }
    if (C_omp_regular != nullptr) {
        delete C_omp_regular;
    }
    if (C_omp_simd != nullptr) {
        delete C_omp_simd;
    }
    
    return 0;
}