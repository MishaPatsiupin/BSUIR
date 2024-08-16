//Заполнить массив с клавиатуры вещественными числами.+
// Удалить все целые числа.
#include "functions.h"


int main() {

    int size = 0;
    input_size(&size);

    float *arr  = (float*)malloc(size * sizeof(float));  //[size];
    filling_array(arr, size);
    printf("\nYour array:\n");
    print_arr(arr, size);

    serchAndDeleteFElements(arr, &size);





    printf("\nNew array:\n");
    print_arr(arr, size);

    free(arr);
    return 0;
}
