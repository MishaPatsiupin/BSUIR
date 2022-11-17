/*В квадратной матрице размером NxN найти сумму элементов в 1 четверти*/

#include <time.h>
#include <stdio.h>
#include <stdlib.h> 


void iput_mas(int mas1[100][100], int& size);
void print_2mas(int mas[100][100], int size);
void sum_1mas(int mas[100][100], int size);

int main() {
    srand(time(NULL));

    int size = 0;
    int mas1[100][100] = { 0 };
    iput_mas(mas1, size);

    print_2mas(mas1, size);

    sum_1mas(mas1, size);

    printf("\n");

    return 0;
}


void iput_mas(int mas1[100][100], int& size) {
    printf("Enter the number of elements of the first array 'N' ");
    while (scanf_s("%d", &size) != 1 or 0 >= size or size > 100 or getchar() != ('\n')) {
        printf("Enter the number of elements of the first array 'N' \a");
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
        for (; iRow < size; iRow++) {
            for (iCol = 0;iCol < size; iCol++) {
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
        while (iRow < size) {
            int iCol = 0;
            do {

                input = rand() % 100 + iRow + iCol - 50;
                mas1[iRow][iCol] = input;
                iCol++;
            } while ((iCol < size));
            iRow++;
        }
    }
}

void print_2mas(int mas[100][100], int size) {
    printf("\n\nmas ->\n");
    for (int iRow = 0; iRow < size; iRow++) {
        for (int iCol = 0; iCol < size; iCol++) {
            printf("%3d  ", mas[iRow][iCol]);
        }
        printf("\n");
    }
}

void sum_1mas(int mas[100][100], int size) {
    int sum_mas = 0, iRow = 0, Rowi = 0, iCol = 0,Coli = 0;
    if (size % 2 == 1) {
        iRow = 0;
        Rowi = size / 2 + 1;
        while (iRow < Rowi) {

            iCol = size / 2 ;
            Coli = size - 1;
            while (iCol <= Coli) {
                sum_mas += mas[iRow][iCol];
                iCol++;
            }
            printf("\n");
            iRow++;


        }
    }
    else {
        iRow = 0;
        Rowi = size / 2;
        while (iRow < Rowi) {

            iCol = size / 2;
            Coli = size - 1;
            while (iCol <= Coli) {

                sum_mas += mas[iRow][iCol];
                iCol++;
            }
        printf("\n");
        iRow++; 
        }
    }
    printf("Sum of the element of 1 quarter %d", sum_mas);

}

