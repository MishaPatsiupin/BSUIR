main.cpp/*Характеристикой столбца целочисленной матрицы назовем
сумму модулей его отрицательных нечетных элементов.
Переставляя столбцы заданной матрицы,
расположить их в соответствии с ростом характеристик.
Найти сумму элементов в тех столбцах,
которые содержат хотя бы один отрицательный элемент.*/

#include <stdlib.h>
#include <stdio.h>

int calc_sum(int arr[100][100], const int curCol, int rows) {
    int sum = 0;
    for (int curRow = 0; curRow < rows; ++curRow) {
        if (arr[curRow][curCol] < 0) {
            if ((-arr[curRow][curCol]) % 2 == 1) {
                sum += -arr[curRow][curCol];
            }
        }
    }
    printf("%3d", sum);
    return sum;
}

void swap_cols(int arr[100][100], int leftCol, int rightCol, int rows) {
    for (int curRow = 0; curRow < rows; ++curRow) {
        int temp = arr[curRow][leftCol];
        arr[curRow][leftCol] = arr[curRow][rightCol];
        arr[curRow][rightCol] = temp;
    }
}

void sort(int arr[100][100], int rows, int cols) {
    //int arr2[cols];
    int* arr2 = NULL;
    arr2 = (int*)malloc(cols * sizeof(int));
    for (int i = 0; i < cols; ++i) {
        arr2[i] = 0;
    }

    for (int curCol = 0; curCol < cols; ++curCol) {
        arr2[curCol] = calc_sum(arr, curCol, rows);
    }

    for (int i = 0; i < cols; ++i) {
        for (int j = 0; j < cols - i - 1; ++j) {
            if (arr2[j] > arr2[j + 1]) {
                swap_cols(arr, j, j + 1, rows);
                int tmp = arr2[j];
                arr2[j] = arr2[j + 1];
                arr2[j + 1] = tmp;
            }
        }
    }
}

void new_sum(int arr[100][100], int rows, int cols) {

    for (int iCol = 0; iCol < cols; iCol++) {
        for (int iRow = 0; iRow < rows; iRow++) {
            if (arr[iRow][iCol] < 0) {
                int sum = 0;
                for (int row = 0; row < rows; row++) {
                    sum += arr[row][iCol];
                }
                printf("%d Col, sum = %d\n", iCol + 1, sum);
                break;
            }
        }
    }

}

void iput_mas(int mas1[100][100], int rows, int cols) {
    printf("Enter the number of elements of the first array 'Row Col' ");
    while (scanf_s("%d %d", &rows, &cols) != 2 || 0 >= rows || rows > 100 || 0 >= cols || cols > 100 || getchar() != ('\n')) {
        printf("Enter the number of elements of the first array 'Row Col' \a");
        rewind(stdin);
    }
    int fl1;
    printf("If you want to fill in the array manually - enter 0, and for autoful other than 0 -- ");
    while (scanf_s("%d", &fl1) != 1) {
        printf("If you want to fill in the array manually - enter 0, and for autoful other than 0 -- ");
        rewind(stdin);
    }
    if (fl1 == 0) {
        int input, iRow = 0, iCol = 0;
        for (; iRow < rows; iRow++) {
            for (iCol = 0;iCol < cols; iCol++) {
                printf("%d element %d Row= ", iCol + 1, iRow + 1);
                while (!scanf_s("%d", &mas1[iRow][iCol]) || getchar() != '\n') {
                    printf("%d element %d Row= ", iCol + 1, iRow + 1);
                    rewind(stdin);
                }
            }
        }
    }
    else {

        int input = 0, iRow = 0;
        while (iRow < rows) {
            int iCol = 0;
            do {

                input = rand() % 100 + iRow + iCol - 50;
                if (input == 0) input++;
                mas1[iRow][iCol] = input;
                iCol++;
            } while ((iCol < cols));
            iRow++;
        }
    }
}


int main() {
    int mas[100][100] = { 0 };
    int cols = 1;
    int rows = 1;
    iput_mas(mas, rows, cols);
    printf("\n");

    for (int iRow = 0; iRow < 100; iRow++) {
        if (mas[0][iRow] == 0) {
            cols = iRow;
            break;
        }
    }
    for (int iCol = 0; iCol < 100; iCol++) {
        if (mas[iCol][0] == 0) {
            rows = iCol;
            break;
        }
    }
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            printf("%3d ", mas[i][j]);
        }
        printf("\n");
    }

    printf("\n\nCharakteristic\n");
    sort(mas, rows, cols);

    printf("\n\n\n");

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            printf("%3d ", mas[i][j]);
        }
        printf("\n");
    }

    printf("\n");
    new_sum(mas, rows, cols);

    return 0;
}
