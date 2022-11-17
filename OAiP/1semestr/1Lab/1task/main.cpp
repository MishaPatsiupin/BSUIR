/*Вычислить расстояние между двумя точками с координатами (х1, y1) и (х2, y2).
Координаты точек вводятся с клавиатуры.
Результат вывести с точностью до двух знаков после запятой.*/
#include <stdio.h>

int main()
{
	float x1 = 0, x2 = 0, y1 = 0, y2 = 0;
	float dist, fakeSqrt = 0;
	
	printf("Enter the coordinates of the points in the formst (x1, y1)(x2, y2)\n");
	if (scanf_s("%f %f %f %f", &x1, &y1, &x2, &y2) != 4) { printf("Error"); return 0; };


		dist = (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1); //квадрат растояния

		for (float i = 0.0001; i < dist; i += 0.0001)
		{
			if (i * i - dist < 0)
			{
				if (-(i * i - dist) < 0.01)
				{
					fakeSqrt = i;
				}
			}
			else
			{
				if (i * i - dist < 0.01)
				{
					fakeSqrt = i;
				}
			}
		}

		printf("Distance: %.2f", fakeSqrt);

		return 0;
}
