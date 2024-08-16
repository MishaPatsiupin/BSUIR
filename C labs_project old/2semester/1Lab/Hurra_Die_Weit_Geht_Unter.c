#include "Hurra_Die_Weit_Geht_Unter.h"

int amount = 5;

//не работает добовление новой структуры, но работтает изменение,
// !!не работает изменение строки


//изночальное меню
void print_menu() {
    printf("0 - Exit\n");
    printf("1 - If you want to display the full list of toasters\n");
    printf("2 - If you want to display a sorted list\n");
    printf("3 - If you want to add a new toaster to the list\n");
    printf("4 - If you want to change the characteristics of the toaster from the list\n");
    printf("5 - If you want to remove the toaster from the list\n");
}

int get_choice(int kolichestvo) { //прием циферки
    char num;
    fflush(stdin);
    printf("Enter your choice: ");
    do {
        scanf("%c", &num);
        num -= '0';
        if (num < 0 || num > kolichestvo) {
            printf("Wrong number, try again\n");
            rewind(stdin);
          num = -1;
        }
    } while (num < 0 || num > kolichestvo);
    fflush(stdin);
    printf("\n");
    return (int) num;
}

/*
int get_choice(int kolichestvo) {
    printf("Enter your choice: ");
    int temp = -1;
    while (scanf_s("%d", &temp) != 1 || getchar() != ('\n') || temp < 0 || temp > kolichestvo) {
        printf("Wrong number, try again\n");
        rewind(stdin);
    }
    printf("\n");
    return temp;
}*/

void print(struct toaster *toast) { //напечатать весь список структур
    for (int i = 0; i < amount; ++i) {
        print_str(toast, i);
        printf("\n");
    }

}

void print_str(struct toaster *toast, int i) { //печать стркутуры [i]

    printf("\nPrice: %d\n", toast[i].chr[0]);
    printf("Power: %d\n", toast[i].chr[1]);
    printf("Number of compartments of bread slices: %d\n", toast[i].chr[2]);
    printf("How to cook frozen toast: %s\n", toast[i].frozen_toast);
    printf("Removable crumb tray: %s\n", toast[i].tray);
    printf("Material is: %s\n", toast[i].material == 0 ? "plastic" : "metal");

    printf("%s\n", toast[i].name);
}


//
void swap(pToaster toast, int dest, int src) {
    Toaster temp = toast[dest];
    toast[dest] = toast[src];
    toast[src] = temp;
}

int get_mode() {
    printf("Enter the mode sort, 1 - po vozrastaniu, 0 - po ybivaniu");
    return get_choice(1);
}

void sort_by_fields(pToaster toast, int first_field, int second_field) {
    int mode = 0;
    mode = get_mode(); //get mode , 1 - возрастание, 0 - убывание
    for (int i = 0; i < amount - 1; ++i) {
        for (int j = 0; j < amount - i - 1; ++j) {
            if (toast[j].chr[first_field] < toast[j + 1].chr[first_field]) {
                if (mode == 0) {
                    swap(toast, j, j + 1);
                }
            } else if (toast[j].chr[first_field] == toast[j + 1].chr[first_field]) {
                if (second_field != 0) {
                    if (toast[j].chr[second_field] < toast[j + 1].chr[second_field]) {
                        if (mode == 0) {
                            swap(toast, j, j + 1);
                        }
                    }
                }
            }
            //тоже самое, но вслучае, когда больше  и mode == 1;
            if (toast[j].chr[first_field] > toast[j + 1].chr[first_field]) {
                if (mode == 1) {
                    swap(toast, j, j + 1);
                }
            } else if (toast[j].chr[first_field] == toast[j + 1].chr[first_field]) {
                if (second_field != 0) {
                    if (toast[j].chr[second_field] > toast[j + 1].chr[second_field]) {
                        if (mode == 1) {
                            swap(toast, j, j + 1);
                        }
                    }
                }
            }
        }
    }
}


void sort(struct toaster *toast) {
    int choice = -1;
    printf("Choose which MAIN parameter you want to sort by:\n 1 - price\n 2 - power \n 3 - number of compartments of bread slices\n 0 - exit \n\n");
    choice = get_choice(4);
    if (choice != 0) {
        int first_field = choice;
        //print choose second field

        printf("Choose which SECOND parameter you want to sort by:\n 1 - price\n 2 - power \n 3 - number of compartments of bread slices\n 0 - nothing \n\n");
        int second_field = get_choice(3);
        //if second field = 0, оставляем как есть
        sort_by_fields(toast, first_field - 1, second_field - 1);  //-1
    }
}

//добавление и инциализация новой структуры++
void add(struct toaster *toast) {
    amount++;
    toast = (pToaster) realloc(toast, amount * sizeof(struct toaster));
    int n = amount - 1;
    int time_int = 0;
    char time_char[41];


    fflush(stdin);
    printf("Enter a value [Price:]- ");
    scanf("%d", &toast[n].chr[0]/*time_int*/);
   // toast[n].chr[0] = time_int;

    fflush(stdin);
    printf("Enter a value [Power:]- ");
    scanf("%d", &time_int);
    toast[n].chr[1] = time_int;

    fflush(stdin);
    printf("Enter a value [Number of compartments of bread slices:]- ");
    scanf("%d", &time_int);
    toast[n].chr[2] = time_int;

    fflush(stdin);
    printf("Enter a choize(yes or no) [How to cook frozen toast:]- ");
    gets(time_char);
    strcpy(toast[n].frozen_toast, time_char);

    fflush(stdin);
    printf("Enter a choize(yes or no) [Removable crumb tray:]- ");
    gets(time_char);
    strcpy(toast[n].tray, time_char);

    fflush(stdin);
    printf("Enter a material, 0 - plastic, 1 - metal [Body material:] - ");
    int fl = get_choice(2);
    if (fl == 0) {
        toast[n].material = plastic;
    } else {
        toast[n].material = metal;
    }

    fflush(stdin);
    printf("Enter a name - ");
    gets(time_char);
    strcpy(toast[n].name, time_char);
    printf("\n");
}

//изменение даных в какой-либо структуре++
void change(struct toaster *toast) {
    print(toast);
    int n = -1;
    printf("\nEnter the number toster the change\n");
    n = get_choice(amount) - 1;

    print_str(toast, n);
    printf("\nEnter the number of the line you want to change 1 - 7\n");
    int str_change = get_choice(7);

    if (str_change == 0) {
        return;
    }

    int time_int = 0;
    char time_char[41];
    switch (str_change) {
        case 1: {
            fflush(stdin);
            printf("Enter a new value [Price:]- ");
            scanf("%d", &time_int);
            toast[n].chr[0] = time_int;
            break;
        }

        case 2: {
            fflush(stdin);
            printf("Enter a new value [Power:]- ");
            scanf("%d", &time_int);
            toast[n].chr[1] = time_int;
            break;
        }

        case 3: {
            fflush(stdin);
            printf("Enter a new value [Number of compartments of bread slices:]- ");
            scanf_s("%d", &time_int);
            toast[n].chr[2] = time_int;
            break;
        }

        case 4: {
            fflush(stdin);
            printf("Enter a new choize(yes or no) [How to cook frozen toast:]- ");
            gets(time_char);
            strcpy(toast[n].frozen_toast, time_char);
            break;
        }

        case 5: {
            fflush(stdin);
            printf("Enter a new choize(yes or no) [Removable crumb tray:]- ");
            gets(time_char);
            strcpy(toast[n].tray, time_char);
            break;
        }

        case 6: {
            fflush(stdin);
            printf("Enter a new material, 0 - plastic, 1 - metal [Body material:] - ");
            int fl = get_choice(2);
            if (fl == 0) {
                toast[n].material = plastic;
            } else {
                toast[n].material = metal;
            }
            char mater[10];
            gets(mater);
            break;
        }

        case 7: {
            fflush(stdin);
            printf("Enter a new name - ");
            gets(time_char);
            strcpy(toast[n].name, time_char);
            break;
        }
    }
}

//удаление структуры++
void remove_(struct toaster *toast) {
    print(toast);
    int n = -1;
    printf("Enter the number toster the delete\n");
    n = get_choice(amount) - 1;
    for (; n < amount - 1; n++) {
        toast[n] = toast[n + 1];
    }

    amount--;
    realloc(toast, sizeof(Toaster) * amount);
}

//база даннфх структур по умолчанию++ User has already been invited
void baza(struct toaster *toast) {
    toast[0].chr[1] = 850;
    toast[1].chr[1] = 930;  //power
    toast[2].chr[1] = 950;
    toast[3].chr[1] = 600;
    toast[4].chr[1] = 450;

    toast[0].chr[2] = 2;
    toast[1].chr[2] = 1;  //number
    toast[2].chr[2] = 1;
    toast[3].chr[2] = 2;
    toast[4].chr[2] = 4;

    toast[0].chr[0] = 90;
    toast[1].chr[0] = 80; //price
    toast[2].chr[0] = 80;
    toast[3].chr[0] = 99;
    toast[4].chr[0] = 99;


    strcpy(toast[0].name, "ProfiCook PC-TA 1170");
    strcpy(toast[1].name, "Polaris PET 0923");
    strcpy(toast[2].name, "Tefal TT520D10");
    strcpy(toast[3].name, "Sencor STS 5050SS");
    strcpy(toast[4].name, "Gorenje T1100CLI");

    toast[0].material = plastic;
    toast[1].material = metal;
    toast[2].material = plastic;
    toast[3].material = metal;
    toast[4].material = plastic;


    char chouse[4] = "yes";
    char chouse1[3] = "no";
    strcpy(toast[0].frozen_toast, chouse);
    strcpy(toast[1].frozen_toast, chouse);
    strcpy(toast[2].frozen_toast, chouse1);
    strcpy(toast[3].frozen_toast, chouse1);
    strcpy(toast[4].frozen_toast, chouse1);

    strcpy(toast[0].tray, chouse1);
    strcpy(toast[1].tray, chouse);
    strcpy(toast[2].tray, chouse);
    strcpy(toast[3].tray, chouse);
    strcpy(toast[4].tray, chouse1);
}

