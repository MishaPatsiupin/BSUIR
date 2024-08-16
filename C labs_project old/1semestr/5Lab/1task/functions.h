
#ifndef INC_5_1_FUNCTIONS_H
#define INC_5_1_FUNCTIONS_H
// Секция прототипов глобальных функций
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void filling_array (float *arr,int size);
void auto_filling_array (float *arr,int size);
void manual_filling_array (float *arr,int size);
void print_arr (float *arr, int size);
void serchAndDeleteFElements (float *arr, int *size);
void delete_element_i (int i, float *arr, int *size);
void input_size (int *size);

#endif //INC_5_1_FUNCTIONS_H