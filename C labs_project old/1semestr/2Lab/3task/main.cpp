/*10.	Дано целое число N (> 1),
являющееся числом Фибоначчи: N = FK
Найти целые числа FK–1 и FK+1 — предыдущее и последующее числа Фибоначчи.*/

#include <stdio.h>

int main()
{
    int N = 2, FKK = 0;
    int F = 1;
    int b = 1;
    int i = 2;

    printf("Enter the number N (> 1): ");
    if (scanf_s("%d", &N) == 1)
    {
        while (i <= N) {
            i = F + b;
            b = F;
            F = i;
            i++;
        }
        FKK = F + b;
        printf("FK-1 = %d\nFK+1 = %d\n", F, FKK);
        return 0;
    }
    else {
        printf("Error");
        return 0;
    }
}
