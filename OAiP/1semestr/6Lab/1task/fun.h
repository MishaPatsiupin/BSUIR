#ifndef INC_6_1_FUN_H
#define INC_6_1_FUN_H
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

int *arrays (int size);
void input_size (int *size);
void input_arr (int *arr, int size);
void print_arr (int *arr, int size);
//void array_output (int *fl, int *arr, int size);
void sorting_by_choice (int *arr, int size);
void copy_arr(int *dest, int *source, int size);
void bubble_sort(int *arr, int size);
void bed_array (int *arr, int size);



#endif //INC_6_1_FUN_H
