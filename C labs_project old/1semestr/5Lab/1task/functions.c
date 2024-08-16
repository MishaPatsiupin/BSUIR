#include "functions.h"


void filling_array(float *arr, int size) {
    int fl = 0;
    printf("Enter 0 for automatic completion, otherwise any number for manual entry ");
    while (scanf_s("%d", &fl) != 1 || getchar() != ('\n')) {
        printf("Enter 0 for automatic completion, otherwise any number for manual entry ");
        rewind(stdin);
    }

    if (fl == 0)
        auto_filling_array(arr, size);
    else {
        manual_filling_array(arr, size);
    }
}

void auto_filling_array(float *arr, int size) {
    srand(time(NULL));

    for (int i = 0; i < size; i++) {
        arr[i] = (rand() % 100 - 50) * 0.25;
    }
}

void manual_filling_array(float *arr, int size) {
    for (int i = 0; i < size; i++) {
        printf("Enter element %d -> ", i + 1);
        while (scanf_s("%f", &arr[i]) != 1 || getchar() != ('\n')) {
            printf("Enter element %d -> ", i + 1);
            rewind(stdin);
        }
    }
}

void print_arr(float *arr, int size) {
    for (int i = 0; i < size; i++) {
        printf("arr[%d] = %2.2f\n", i + 1, arr[i]);
    }

}

void serchAndDeleteFElements(float *arr, int *size) {
    for (int i = 0; i < (*size); i++) {
        if ((int) arr[i] == arr[i]) {
            delete_element_i(i, arr, (int *) size);
            i--;
        }
    }

}

void delete_element_i(int i, float *arr, int *size) {
 arr = (float *) realloc (arr, (*size)--);
    //(*size)--;

    for (; i < (*size); i++) {
        arr[i] = arr[i + 1];
    }
}

void input_size(int *size) {
    printf("Enter the size array ");
    while (scanf_s("%d", size) != 1 || getchar() != ('\n') || *size < 1) {
        printf("Enter the size array ");
        rewind(stdin);
    }
}
