//	Определить, имеется ли среди трёх чисел a, b и c хотя бы одна  пара равных между собой чисел.

#include <stdio.h>

int main()
{
	float a = 0, b = 0, c = 0;
	printf("Enter number: a b c\n");
	if (scanf_s("%f %f %f", &a, &b, &c) != 3) { printf("Error"); return 0; }
	
		if (a == b)
			printf("a = b");
		else if (a == c)
			printf("a = c");
		else if (b == c)
			printf("b = c");
		else
			printf("The are no equal number");
		return 0;
	
}
