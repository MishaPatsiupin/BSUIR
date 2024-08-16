//1.	В массиве из n элементов удалить все элементы расположенные после последнего четного
#include <stdio.h>
#include <stdlib.h>  
#include <time.h>

int main() {
	srand(time(NULL));

	int mas[100] = { 0 };
	int n;

	printf("Enter the number of elements of the  array ");
	while (!scanf_s("%d", &n) || n < 0 || n > 100 || getchar() != '\n') {
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
				printf("%d element = ", i + 1);
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
				mas[i] = 0 - znah_i;
		}
	}

	/*for (int i = 0; i <= n; i++) {
		printf("%d\n", mas[i]);
	}*/
	printf("\n");

	int k = 0;
	for (int i = n - 1; i >= 0; i--) {
		if (mas[i] % 2 == 0) {
			k = i;
			break;
		}
	}
	for (int i = 0; i <= k; i++) {
			printf("%d elemetn = %d\n", i + 1, mas[i]);
		}

	return 0;
}
