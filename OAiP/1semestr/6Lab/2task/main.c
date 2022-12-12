#include "fonk.h"
//В матрице размером NxM выполнить сортировку строк по возрастанию суммы четных элементов методом слияния

int main() {
    srand(time(NULL));
    int rows = 0, cols = 0;
    input_rows_cols(&rows, &cols);

    int **arr = matrix(rows, cols);
    printf("\n\nYour matrix -> \n");
    printMatrix(arr, rows, cols);

    int *characteristic = array_characteristic(arr, rows, cols);
    printf("\n");
    for (int i = 0; i < rows; i++)
        printf("%5d", characteristic[i]);
    printf("\n");

    merge_sort(arr, characteristic, 0, rows - 1);
    printf("\n\nNew matrix -> \n");
    printMatrix(arr, rows, cols);

    printf("\n");
    for (int i = 0; i < rows; i++)
        printf("%5d", characteristic[i]);
    printf("\n");

    free(arr);

    return 0;
}

