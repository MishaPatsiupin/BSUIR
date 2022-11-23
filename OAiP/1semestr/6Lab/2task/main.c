#include "fonk.h"
//В матрице размером NxM выполнить сортировку строк по возрастанию суммы четных элементов методом слияния

int main() {
srand(time(NULL));
    int rows = 0, cols = 0;
    input_rows_cols(&rows, &cols);

    int **arr = matrix(rows, cols);
    printf("\n\nYour matrix -> \n");
    printMatrix(arr, rows, cols);

    int *characteristic =  array_characteristic(arr, rows, cols);
    int *characteristic2 = array_characteristic(arr, rows, cols);
for (int i = 0; i < rows; i++)
    printf("%5d", characteristic[i]);

   // merge_sort(arr, 0, rows - 1, characteristic, characteristic2);

    sort(arr, rows, cols);
    printf("\n\nNew matrix -> \n");
    printMatrix(arr, rows, cols);

//    printf("\n\nNew matrix -> \n");
//    for (int i = 0; i < rows; i++)
//        printf("%5d", characteristic[i]);
//    printf("\n\nNew matrix -> \n");
//    for (int i = 0; i < rows; i++)
//        printf("%5d", characteristic2[i]);
    free(arr);
return 0;
}
