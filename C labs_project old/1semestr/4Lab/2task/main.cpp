/*Проверить,
все ли строки матрицы содержат хотя бы один положительный элемент.
Если да,
то изменить знаки всех элементов матрицы на обратные.*/

#include <time.h>
#include <stdio.h>
#include <stdlib.h> 

void iput_mas(int mas1[100][100], int& size1Row, int& size1Col);
void print_2mas(int mas[100][100], int sizeRow, int sizeCol);
int searchPosElement(int mas[100][100], int sizeRow, int sizeCol);
void reverse(int mas[100][100], int sizeRow, int sizeCol);


int main() {
    srand(time(NULL));

    int size1Row = 0; //строки
    int size1Col = 0;
    int mas1[100][100] = { 0 };
    iput_mas(mas1, size1Row, size1Col);

    print_2mas(mas1, size1Row, size1Col);

    printf("\n");

    int fl = searchPosElement (mas1, size1Row, size1Col);
   
    if (fl == 1) 
     reverse(mas1, size1Row, size1Col);

     print_2mas(mas1, size1Row, size1Col);
     return 0;
}

void reverse(int mas[100][100], int sizeRow, int sizeCol) {
    for (int iRow = 0; iRow < sizeRow; iRow++) {
        for (int iCol = 0; iCol < sizeCol; iCol++) {
            mas[iRow][iCol] = -mas[iRow][iCol];     
        }
    }
}
int searchPosElement(int mas[100][100], int sizeRow, int sizeCol) {
    int fl = 0;
    for (int iRow = 0; iRow < sizeRow; iRow++) {
        for (int iCol = 0; iCol < sizeCol; iCol++) {
            if (mas[iRow][iCol] > 0) {
                fl = 1;
                break;
            }
        }
    }
return fl;
}

void iput_mas(int mas1[100][100], int& size1Row, int& size1Col) {
    printf("Enter the number of elements of the first array 'Row Col' ");
    while (scanf_s("%d %d", &size1Row, &size1Col) != 2 or 0 >= size1Row or size1Row > 100 or 0 >= size1Col or size1Col > 100 or getchar() != ('\n')) {
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
        for (; iRow < size1Row; iRow++) {
            for (iCol = 0;iCol < size1Col; iCol++) {
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
        while (iRow < size1Row) {
            int iCol = 0;
            do {

                input = rand() % 100 + iRow + iCol - 50;
                mas1[iRow][iCol] = input;
                iCol++;
            } while ((iCol < size1Col));
            iRow++;
        }
    }
}

void print_2mas(int mas[100][100], int sizeRow, int sizeCol) {
    printf("\n\nmas ->\n");
    for (int iRow = 0; iRow < sizeRow; iRow++) {
        for (int iCol = 0; iCol < sizeCol; iCol++) {
            printf("%3d  ", mas[iRow][iCol]);
        }
        printf("\n");
    }
}
