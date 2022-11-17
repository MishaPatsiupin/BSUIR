#include <time.h>
#include <stdio.h>
#include <stdlib.h> 

int iput_mas1(int* mas1) {
    int size1 = 100;
    printf("Enter the number of elements of the first array ");
    while (scanf_s("%d", &size1) != 1 or 0 > size1 > 100) {
        printf("Try agin\a\n");
    }

    int fl1;
    printf("If you want to fill in the array manually - enter 0, if you want autofill \n ");
    scanf_s("%d", &fl1);
    if (fl1 == 0) {
        int input, i = 0;
        while (i < size1) {
            do {
                printf("%d element = ", i + 1);
                scanf_s("%d", &input);
            } while ((i > 0) && (input >= mas1[i - 1]));
            mas1[i++] = input;
        }
    }
      
    return size1;
}


int iput_mas2(int* mas2) {
    int size2 = 100;
    printf("Enter the number of elements of the second array ");
    while (scanf_s("%d", &size2) != 1 and 0 < size2 < 100) {
        printf("Try agin\a\n");
    }

    int fl1;
    printf("If you want to fill in the array manually - enter 0, if you want autofill - 1\n ");
    scanf_s("%d", &fl1);
    if (fl1 == 0) {
        int input, i = 0;
        while (i < size2) {
            do {
                printf("%d element = ", i + 1);
                scanf_s("%d", &input);
            } while ((i > 0) && (input <= mas2[i - 1]));
            mas2[i++] = input;
        }
    }
    else {
        int input, i = 0;
        while (i < size2) {
            do {
                input = rand() % 200;
            } while ((i > 0) && (input <= mas2[i - 1]));
            mas2[i++] = input;
        }
    }
    return size2;
}


void shared_array(int size1, int size2, int* mas1, int* mas2, int* mas3) {
    int size3 = size1 + size2;
    int i3 = 0;
    int i1 = 0, i2 = size2 - 1;
    while (i1 < size1 && i2 > -1) {
        if (mas1[i1] >= mas2[i2]) {
            mas3[i3] = mas1[i1];
            i1++;
        }
        else {
            mas3[i3] = mas2[i2];
            i2--;
        }
        i3++;
    }

    //Если один из массивов закончился, то заполняем оставшегося
    while (i1 < size1 && i2 > -1) {
        if (mas1[i1] >= mas2[i2]) {
            mas3[i3] = mas1[i1];
            i1++;
        }
        else {
            mas3[i3] = mas2[i2];
            i2--;
        }
        i3++;
    }
    if (i1 == size1 && (i2 >= 0)) {
        while (i2 != 0) {
            mas3[size1 + (size2 - i2) + 1] = mas2[i2];
            i2--;
        }
    }
    if (i2 < 0 && (i1 < size1)) {
        while (i1 != size1) {
            mas3[size2 + i1] = mas1[i1];
            i1++;
        }
    }
}



int main() {
    srand(time(NULL));

    int mas1[100] = { 0 };
    int size1 = iput_mas1(mas1);

    int mas2[100] = { 0 };
    int size2 = iput_mas2(mas2);

    printf("\n");

    for (int i = 0; i < size1; i++) {
        printf("mas1[%d] = %d\n", i + 1, mas1[i]);
    }
    printf("\n");
    for (int i = 0; i < size2; i++) {
        printf("mas2[%d] = %d\n", i + 1, mas2[i]);
    }

    int mas3[200] = { 0 };
    shared_array(size1, size2, mas1, mas2, mas3);

    printf("\n");
    for (int i = 0; i < size1 + size2; i++) {
        printf("mas3[%d] = %d\n", i + 1, mas3[i]);
    }

    return 0;
}
