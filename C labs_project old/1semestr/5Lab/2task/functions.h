#ifndef UNTITLED_FUNCTIONS_H
#define UNTITLED_FUNCTIONS_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>


int random();
void inputRowsCols (int *rows, int *cols);
void randomFullMatrix (int *arr, int rows, int cols);
void printMatrix(int *arr, int rows, int cols);
void delint_diagonal (int *arr, int  *rows, int cols);
void delint_Diagonal (int *arr, int  *rows, int cols);


#endif //UNTITLED_FUNCTIONS_H
