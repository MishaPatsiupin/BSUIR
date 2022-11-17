#include <stdio.h>

int main()
{
	int number;

	printf("Enter number:\n");
	if (scanf_s("%d", &number) != 1 ) {
		printf("Error \a");
		return 0;
	}



	switch (number)
	{
	default:

	case 0: { printf("This number - Zero "); return 0; }
		  break;
	case 1: { printf("This number - One"); return 0; }
		  break;
	case 2: { printf("This number - Two"); return 0; }
		  break;
	case 3: { printf("This number - Three "); return 0; }
		  break;
	case 4: { printf("This number - Four"); return 0; }
		  break;
	case 5: { printf("This number - Five"); return 0; }
		  break;
	case 6: { printf("This number - Six");  return 0; }
		  break;
	case 7: { printf("This number - Seven"); return 0; }
		  break;
	case 8: { printf("This number - Eight"); return 0; }
		  break;
	case 9: { printf("This number - Nine"); return 0; }
		  break;
	case 10: { printf("This number - Ten"); return 0; }
		   break;
	}

	return 0;
}
