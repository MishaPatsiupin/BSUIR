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

void swap(int **arr, int dest, int src) {
    int *temp = arr[dest];
    arr[dest] = arr[src];
    arr[src] = temp;
}


int *array_characteristic(int **arr, int rows, int cols) {
    int *characteristic = (int *) malloc(rows * sizeof(int));
    for (int i = 0; i < rows; i++) {
        characteristic[i] = sum_even(arr, i, cols);
    }
    return characteristic;
}

void merge_sort(int **mas, int *arr, int l, int r) {
    if (l == r) return;
    int mid = (l + r) / 2;
    merge_sort(mas, arr, l, mid);
    merge_sort(mas, arr, mid + 1, r);
    int i = l;
    int j = mid + 1;
    int *tmp = (int *) malloc(r * sizeof(int));
    int **tmp2 = (int **) malloc(r * sizeof(int *));

    for (int step = 0; step < r - l + 1; step++) {
        if ((j > r) || ((i <= mid) && (arr[i] < arr[j]))) {
            tmp[step] = arr[i];
            tmp2[step] = mas[i];
            i++;
        } else {
            tmp[step] = arr[j];
            tmp2[step] = mas[j];
            j++;
        }
    }

    for (int step = 0; step < r - l + 1; step++) {
        arr[l + step] = tmp[step];
        mas[l + step] = tmp2[step];
    }
}

