#include "fonk.h"


void input_rows_cols(int *rows, int *cols) {
    printf("Enter the number of elements of the first array 'Row Col' ");
    while (scanf_s("%d %d", rows, cols) != 2 || 0 >= *rows || 0 >= *cols || getchar() != ('\n')) {
        printf("The main diagonal, as well as the side diagonal, by definition exists only in a square matrix\n");
        printf("Enter the number of elements of the first array 'Row Col' \a");
        rewind(stdin);
    }
}

int **matrix(int rows, int cols) {
    int **arr;
    arr = (int **) calloc(rows, sizeof(int *));//+
    for (int i = 0; i < rows; i++) {
        arr[i] = (int *) calloc(rows, sizeof(int));
    }
    if (arr == NULL) {
        printf("Memory allocation error");//ошибка выделения памяти
        return 0;
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            arr[i][j] = random();
        }
    }

    return arr;
}

int random() {

    int temp = rand() % 100;
    return temp;
}

void printMatrix(int **arr, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%5d", arr[i][j]);
        }
        printf("\n");
    }
}

int sum_even(int **arr, int row, int cols) {
    int temp_sum = 0;
    for (int j = 0; j < cols; j++) {
        if (arr[row][j] % 2 == 0) {
            temp_sum += arr[row][j];
        }
    }
    return temp_sum;
}

void swap(int **arr, int dest, int src){
    int *temp = arr[dest];
    arr[dest] = arr[src];
    arr[src] = temp;
}

void sort(int **arr, int rows, int cols) {
    for (int i = 0; i < rows - 1; ++i) {
        int sum1 = sum_even(arr, i, cols);
        int sum2 = sum_even(arr, i + 1, cols);
        if (sum1 > sum2){
            swap(arr, i, i + 1);
        }
    }
}
int* array_characteristic(int **arr, int rows, int cols) {
    int *characteristic = (int *) malloc(rows * sizeof(int));
    for (int i = 0; i < rows; i++) {
        characteristic[i] = sum_even(arr, i, cols);
    }
    return characteristic;
}




void merge_sort(int **arr, int i, int j, int *a, int *aux) {
    if (j <= i) {
        return;
    }
    int mid = (i + j) / 2;

    merge_sort(arr, i, mid, a, aux);
    merge_sort(arr, mid+ 1, j, a, aux);

    int pointer_left = i;
    int pointer_right = mid + 1;
    int k;

    for (k = i; k <= j; k++) {
        if (pointer_left == mid + 1) {      // left pointer has reached the limit
            aux[k] = a[pointer_right];
            swap(arr, k, pointer_right);
            pointer_right++;
        } else if (pointer_right == j + 1) {        // right pointer has reached the limit
            aux[k] = a[pointer_left];
            swap(arr, k, pointer_right);
            pointer_left++;
        } else if (a[pointer_left] < a[pointer_right]) {        // pointer left points to smaller element
            aux[k] = a[pointer_left];
            swap(arr, k, pointer_right);
            pointer_left++;
        } else {        // pointer right points to smaller element
            aux[k] = a[pointer_right];
            swap(arr, k, pointer_right);
            pointer_right++;
        }
    }

    for (k = i; k <= j; k++) {      // copy the elements from aux[] to a[]
        a[k] = aux[k];
    }
}
