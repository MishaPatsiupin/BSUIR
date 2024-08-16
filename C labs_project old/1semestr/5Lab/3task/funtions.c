#include "funtions.h"


void inputRowsCols(int *rows) {
    printf("Enter the number of elements of the first array 'Row' ");
    while (scanf_s("%d", rows) != 1 || *rows <= 0 || getchar() != ('\n')) {
        printf("Enter the number of elements of the first array 'Row' \a");
        rewind(stdin);
    }
}

int **Matrix(int rows, int cols) {
    int **arr;
    arr = (int **) calloc(rows, sizeof(int *));//+
    for (int i = 0; i < rows; i++) {
        arr[i] = (int *) calloc(rows, sizeof(int));
    }
    if (arr == NULL) {
        printf("Memory allocation error");//ошибка выделения памяти
        return 0;
    }
    return arr;
}

int cols(int **arr, int row) {
    int cols = 1;
    for (int i = 0;; i++) {
        if (arr[row][i] != -100) {
            cols++;
        } else {
            break;
        }
    }
    return cols;
}

void printMatrix(int **arr, int rows) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols(arr, i); j++) {
              if (arr[i][j] != -100)
                printf("%5d", arr[i][j]);
        }
        printf("\n");
    }
}

void input_Matrix(int **arr, int rows, int *cols) {
    for (int i = 0; i < rows; i++) {
        int breakk = 0;
        for (int j = 0; j < *cols && breakk == 0; j++) {
            printf("Input arr[%d][%d]: ", i + 1,j + 1);
            while (scanf_s("%d", &arr[i][j]) != 1 || getchar() != '\n') {
                if (arr[i][j] == -100) {
                    break;
                }
                printf("Enter element, row: %d, cols: %d ->", i + 1, j + 1);
                rewind(stdin);
            }
            if (arr[i][j] == -100) {
                breakk = 1;
                if (i != rows - 1) {
                    printf("Next row->\n");
                }
            } else {
                (*cols)++;
                arr[i] = (int *) realloc(arr[i], (*cols) * sizeof(int));
            }
        }
    }
}


void coppi(int ***arr, int rows) {
    for (int i = 0; i < rows; i++) {
        int col = cols(*arr, i);
        if ((i + 1) % 2 == 0 && col != 0) {
            for (int j = 0; j < col;) {
                if ((*arr)[i][j] % 2 != 0) {
                    col++;

                    (*arr)[i] = (int*)realloc((*arr)[i], col * sizeof(int));

                    for (int k = col - 1; k > j;k--) {
                        (*arr)[i][k] = (*arr)[i][k - 1];
                    }
                    j += 2;
                }
                else {
                    j++;
                }
            }
        }
    }
}
