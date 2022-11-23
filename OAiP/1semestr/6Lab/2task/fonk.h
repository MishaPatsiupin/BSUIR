#ifndef INC_6_2_FONK_H
#define INC_6_2_FONK_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void input_rows_cols(int *rows, int *cols);
int random();
int **matrix(int rows, int cols);
void printMatrix(int **arr, int rows, int cols);
void  sort(int **arr, int rows, int cols);
int sum_even(int **arr, int row, int cols);
int* array_characteristic(int **arr, int rows, int cols);
void merge_sort(int **arr, int i, int j, int *a, int *aux);







#endif //INC_6_2_FONK_H
