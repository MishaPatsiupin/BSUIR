/*	В одномерном массиве, состоящем из п вещественных элементов, вычислить:
- минимальный элемент массива;+
- сумму элементов массива, расположенных между первым и последним положительными элементами.*/
#include <stdio.h>
#include "Header.h"
#include <stdlib.h>  

int main() {
	float mas[100];
	int n;
	printf("Enter the number of elements of the  array ");
	while(!scanf_s("%d", &n) || n < 0 || n > 100 || getchar() != '\n') {
		printf("Enter the number of elements of the  array ");
		rewind(stdin);
	}


	int fl;
	printf("If you want to fill in the array manually - enter 0, otlihnoe ot 0 - autofill\n ");
	while (!scanf_s("%d", &fl) || getchar() != '\n') {
		printf("If you want to fill in the array manually - enter 0, otlihnoe ot 0 - autofill ");
		rewind(stdin);
	}

	if (fl == 0) {
		for (int i = 0; i < n; i++) {
			printf("%d element = ", i + 1);
			while (scanf_s("%d", &mas[i]) != 1 || getchar() != '\n') {
				printf("%d element = ", i + 1 );
				rewind(stdin);
			}
			
		}
	}
	else {
		for (int i = 0, flZnak = 0; i < n; i++) {
			float znah_i = 0;
			znah_i = rand() % 200;
			flZnak = rand() % 2;
			if (flZnak == 0)
				mas[i] = znah_i;
			else 
				mas[i] =0 - znah_i;
		}
	}

	for (int i = 0; i < n; i++) {
		printf("%d elemetn = %2.f\n", i+1, mas[i]);
	}

	int min = mas[0];
	for (int i = 0; i < n; i++) {
		if (min > mas[i])
			min = mas[i];
	}
	printf("\nMin element = %d\n", min);

	int firstElPol;
	for (int i = 0; i < n; i++) {
		if (mas[i] > 0) { firstElPol = i; break; }
	}

	int lastElPol;
	for (int i = n; i > 0; i--) {
		if (mas[i] > 0) { lastElPol = i; break; }
	}

	int sum = 0;
	if (n > 2) {
		for (int i = firstElPol + 1; i <= lastElPol - 1; i++) {
			sum += mas[i];
		}
		printf("The amount between the first and last positive element = %d\n", sum);
	}

return 0;
}
