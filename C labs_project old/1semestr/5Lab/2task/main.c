#include "functions.h"
//В матрице размером NxM удалить все элементы на побочной диагонали.



int main() {
    srand(time(NULL));

    int rows = 0, cols = 0;
    inputRowsCols(&rows, &cols);

    int *arr = (int *) malloc(rows * cols * sizeof(int));;
    randomFullMatrix(arr, rows, cols);

    printf("\n\nYour matrix -> \n");
    printMatrix(arr, rows, cols);


    delint_diagonal(arr, &rows, cols);


    printf("\n\nNew matrix -> \n");
    printMatrix(arr, rows, cols);


    free(arr);
return 0;
}









