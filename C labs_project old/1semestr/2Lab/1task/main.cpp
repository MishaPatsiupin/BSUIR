/*10.	Сумма цифр двузначного числа равна 11.
Если к этому числу прибавить 27,
то получится число, записанное теми же цифрами, но в обратном порядке.
Составить алгоритм поиска этого числа, если оно существует.*/

#include <stdio.h>


int main()
{
	int number=10;
	int a=0, b=0;
	int rebem;

	while (((a + b)!= 11) || ((10 * b) + a != number + 27)) 
	{
		number++;
		a = number / 10;
		b = number % 10;

	}
	a = number / 10;
	b = number % 10;
	rebem = 10 * b + a;
		printf("%d", rebem);
	
	return 0;
} 
