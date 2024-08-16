#include "fun.h"

int *arrays(int size) {
    int *arr = (int *) calloc(size, sizeof(int));
    if (arr == NULL) {
        printf("Memory allocation error");//ошибка выделения памяти
        return 0;
    }
    return arr;
}

void input_size(int *size) {
    printf("Enter the number of elements the array 'size' ");
    while (scanf_s("%d", size) != 1 || 0 >= *size || getchar() != ('\n')) {
        printf("Enter the number of elements the array 'size' \a");
        rewind(stdin);
    }
}

int random() {
    srand(time(NULL));
    int temp = rand() % 100 + 1;
    return temp;
}

void input_arr(int *arr, int size) {
    srand(time(NULL));
    for (int i = 0; i < size; ++i) {
        arr[i] = rand() % 100 + 1;
    }
}

void print_arr(int *arr, int size) {
    for (int i = 0; i < size; i++) {
        printf("%5d", arr[i]);
    }
    printf("\n");
}


//5 6 4 3
//4 6 5 3
//4 6 5 3
//4 6 5 3

//В одномерном массиве выполнить сортировку четных элементов методом выбора
void sorting_by_choice(int *arr, int size) {
    for (int i = 0; i < size; ++i) {
        int iMin = i;

        for (int j = i; j < size; ++j) {
            if (arr[j] % 2 == 0 && arr[iMin] > arr[j] && arr[i] % 2 == 0) {
                iMin = j;
            }
        }
        if (iMin > i) {
            int temp = arr[iMin];
            arr[iMin] = arr[i];
            arr[i] = temp;
        }
    }
}

void copy_arr(int *dest, int *source, int size) {
    for (int i = 0; i < size; ++i) {
        dest[i] = source[i];
    }
}

void bubble_sort(int *arr, int size) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size - 1; ++j) {
            if (arr[j] % 2 == 0) {
                int k = j;
                int ok = 1;
                while (k < size && ok) {
                    if (arr[j] > arr[k] && arr[k] % 2 == 0) {
                        int temp = arr[j];
                        arr[j] = arr[k];
                        arr[k] = temp;
                        ok = -1;
                    }
                    k++;
                }
            }
        }
    }
}

void bed_array(int *arr, int size) {
    int num = 1;
for (int i = size; i >= 0; i--){
    arr[i] = num + 1;
    num++;
}
}
