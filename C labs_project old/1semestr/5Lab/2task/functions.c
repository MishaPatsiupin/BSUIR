#include "functions.h"


void delintDiagonal (int *arr, int *rows, int cols) {
    for (int i = 0; i < *rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (j + i == *rows - 1) { //i == j если главная диагональ

                for (int ii = i; ii < *rows; ++ii) {
                    *(arr + ii * cols + j) = *(arr + (ii + 1) * (cols) + j);   //   arr[ii][j] = arr[ii + 1][j];
                }
            }
        }
    }
    arr = (int *) realloc (arr, (*rows--));
   // *rows = *rows - 1;
}

int random() {

    int temp = rand() % 100 - 49;
    return temp;
}

void inputRowsCols(int *rows, int *cols) {
    printf("Enter the number of elements of the first array 'Row Col' ");
    while (scanf_s("%d %d", rows, cols) != 2 || 0 >= *rows || 0 >= *cols  || getchar() != ('\n') || *rows != *cols) {
        printf("The main diagonal, as well as the side diagonal, by definition exists only in a square matrix\n");
        printf("Enter the number of elements of the first array 'Row Col' \a");
        rewind(stdin);
    }
}

void randomFullMatrix(int *arr, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            *(arr + i*cols + j) = random();
        }
    }
}
void delint_diagonal (int *arr, int *rows, int cols) {
    for (int i = 0; i < *rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (j + i == *rows - 1) { //i == j если главная диагональ

                for (int ii = i; ii < *rows; ++ii) {
                    *(arr + ii * cols + j) = *(arr + (ii + 1) * (cols) + j);   //   arr[ii][j] = arr[ii + 1][j];
                }
            }
        }
    }
    //arr = (int *) realloc (arr, (*rows--));
     *rows = *rows - 1;
}
void printMatrix(int *arr, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            printf("%5d", *(arr + i*cols + j));
        }
        printf("\n");
    }
}





