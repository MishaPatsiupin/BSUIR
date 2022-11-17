#include "fun.h"

//В одномерном массиве выполнить сортировку четных элементов методом выбора
int main() {
    int size = 1;
    input_size (&size);
    // printf("Size is %d\n", size); //проверка длинны массива

    int *arr, *arr2;
    arr = arrays(size);
    arr2 = arrays(size);
    input_arr(arr, size);
   // print_arr(arr, size);
    copy_arr(arr2, arr, size);



    double t1 = clock();
    sorting_by_choice(arr,size);
    double t2 = clock() - t1;
   // print_arr(arr, size);


    double t3 = clock();
    bubble_sort(arr2, size);
    double t4 = clock() - t3;
    printf("Time is %lfs for chs and %lfs for bubble\n", t2 / 1000, t4 / 1000);
    getc(stdin);
    return 0;
}



