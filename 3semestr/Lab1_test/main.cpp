// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com


#include "matrix.h"
#include "func.h"

/*
использованием шаблонов
перегрузку операторов ввода и вывода элемента структуры и всей структуры.  Ограничить использование типов данных (не все
типы могут использоваться в шаблоне.
Однонаправленный список*/



int main() {

    matrix matrix_my;
    int func_choize;

    matrix second_matrix;

    int num_of_matrix = 0;
    while (true) {
        func_choize = menu();

        switch (func_choize) {
            case 0: {
                logFile.close();
                return 0;
            }
            case 1: {//умничка
                if (num_of_matrix == 0) {
                    matrix_my.create_matrix();
                    num_of_matrix++;
                } else {
                    if (whot_create_matrix(0) == 0) {
                        second_matrix.create_matrix();
                    } else {
                        second_matrix = (matrix_my);
                    }
                    num_of_matrix++;
                    break;
                }
                break;
            }
            case 2: {//нормас
                if (num_of_matrix == 2) {
                    std::cout << "First " << matrix_my;
                    std::cout << "Second " << second_matrix;
                } else {
                    std::cout << matrix_my;
                }
                break;
            }
            case 3: {//крут
                if (num_of_matrix == 1) {
                    matrix_my.change_data();
                } else {
                    if (whot_create_matrix(8) == 1) {
                        matrix_my.change_data();
                    } else {
                        second_matrix.change_data();
                    }
                }
                break;
            }
            case 4: {//не так и муторно
                if (num_of_matrix == 1) {
                    matrix_my.transposing_matrix();
                    break;
                } else {
                    if (whot_create_matrix(8) == 1) {
                        matrix_my.transposing_matrix();
                    } else {
                        second_matrix.transposing_matrix();
                    }
                    break;
                }
            }


            case 5: {//+
                matrix_my = (matrix_my + second_matrix);
                break;
            }
            case 6: {//-
                matrix_my = (matrix_my - second_matrix);
                break;
            }
            case 7: {//+=
                if (num_of_matrix != 1) {
                    if (whot_create_matrix(8) == 1) {
                        int N = whot_create_matrix(9);
                        matrix_my += N;
                    } else {
                        int N = whot_create_matrix(9);
                        second_matrix += N;
                    }
                } else {
                    int N = whot_create_matrix(9);
                    matrix_my += N;
                }
                break;
            }
            case 8: {//-=
                if (num_of_matrix != 1) {
                    if (whot_create_matrix(8) == 1) {
                        int N = whot_create_matrix(9);
                        matrix_my -= N;
                    } else {
                        int N = whot_create_matrix(9);
                        second_matrix -= N;
                    }
                } else {
                    int N = whot_create_matrix(9);
                    matrix_my -= N;
                }
                break;
            }
            case 9: {//*=
                if (num_of_matrix != 1) {
                    if (whot_create_matrix(8) == 1) {
                        int N = whot_create_matrix(9);
                        matrix_my *= N;
                    } else {
                        int N = whot_create_matrix(9);
                        second_matrix *= N;
                    }
                } else {
                    int N = whot_create_matrix(9);
                    matrix_my *= N;
                }
                break;
            }
            case 10: {//<>
                switch (matrix_my == second_matrix) {
                    case 1:{
std::cout << "The first element of the first matrix is larger than the corresponding element of the second matrix, >\n";
                        break;
                    }
                    case 0:{
                        std::cout << "The first element of the first matrix is equal to the corresponding element of the second matrix, = \n";
                        break;
                    }
                    case -1:{
                        std::cout << "The first element of the first matrix is smaller than the corresponding element of the second matrix, <\n";
                        break;
                    }
                }

                break;
            }

        }
    }

    std::cout << "Hello, World!" << std::endl;
    return 0;
}


