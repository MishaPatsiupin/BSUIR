#ifndef INC_6_2_FONK_H
#define INC_6_2_FONK_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void input_rows_cols(int *rows, int *cols);
int random();
int **matrix(int rows, int cols);
void printMatrix(int **arr, int rows, int cols);
int sum_even(int **arr, int row, int cols);
int* array_characteristic(int **arr, int rows, int cols);
void merge_sort(int **mas, int *arr, int l, int r);
void swap(int **arr, int dest, int src);





#endif //INC_6_2_FONK_H
