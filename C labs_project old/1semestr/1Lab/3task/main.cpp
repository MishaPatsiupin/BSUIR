/*10.	Даны три числа a, b и c.
Составить алгоритм,
определяющий среднее геометрическое этих чисел,
если все они отличны от нуля,
и среднее  арифметическое в противном случае.*/
#include <stdio.h>

int main()
{
	float a = 0, b = 0, c = 0;
	float cube=0, fakeCube=0;
	printf("Enter number: a b c\n");
	if (scanf_s("%f %f %f", &a, &b, &c) != 3) { printf("Error"); return 0; }

	cube = a * b * c;
	if ((a != 0) && (b != 0) && (c != 0)) {

		for (float i = 0.001; i < cube; i += 0.001)
		{
			if (i * i * i - cube < 0)
			{
				if (-(i * i * i - cube) < 0.01)
				{
					fakeCube = i;
				}
			}
			else
			{
				if (i * i * i - cube < 0.01)
				{
					fakeCube = i;
				}
				printf("Geometric mean %.2f\a", fakeCube); return 0;
			}
		}
		
	
	}
		
	
	else 

	printf("Arihmetic mean %.2f\a", (a + b + c) / 3);
	
	
	return 0;
}
