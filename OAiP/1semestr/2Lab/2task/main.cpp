/*10.	Найти значение выражения
( для натуральных m и n, m<n): C=1/m+1/(m+2)+...+1/(m+n)*/

#include <stdio.h>


int main()
{
    float C=1;
    float m=0, n=0;
    

    printf("input 'm' and 'n' (m < n): \n");
    if ((scanf_s("%f%f", &m, &n) == 2) && (m < n)) {
        for (float i = 1; i <= n; i++) {
            C = C/(m + i);
        }
        printf("A=%f\a", C);

    }
    else { printf("Error"); return 0; }
    return 0;
