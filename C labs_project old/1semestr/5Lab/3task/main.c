//3.	В двумерном массиве (количество чисел в строке может быть различным, последнее число - -100.)++
// в каждой четной строке продублировать нечетный элемент.
#include "funtions.h"



int main() {
    int rows = 1, cols = 1;
    inputRowsCols(&rows);

    int **arr = Matrix(rows, cols);
    input_Matrix(arr, rows, &cols);

    printf("\n\nYour matrix -> \n");
    printMatrix(arr, rows);

    coppi(&arr, rows);

    printf("\n\nNew matrix -> \n");
    printMatrix(arr, rows);

free(arr);
return 0;
}