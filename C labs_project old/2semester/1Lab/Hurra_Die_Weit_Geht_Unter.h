#ifndef LAB1_HURRA_DIE_WEIT_GEHT_UNTER_H
#define LAB1_HURRA_DIE_WEIT_GEHT_UNTER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum material{
    plastic = 0,
    metal = 1
}material;

typedef  struct toaster {
    int chr[3]; // , 0 - price. 1 power, 2 - number
    char name[41];
    char frozen_toast [4];
    char tray [4];
    material material;
} Toaster;

typedef Toaster* pToaster;


void print_menu ();
int get_choice(int kolichestvo);
void print(struct toaster * toas);
void sort (pToaster);
void sort_by_number(pToaster toast);
void sort_by_price(pToaster toast);
void sort_by_power(pToaster toast);
void add (struct toaster * toas);
void change (struct toaster * toas);
void remove_ (struct toaster * toas);
void baza (struct toaster * toast);
void print_str(pToaster, int i);
extern int amount;
#endif //LAB1_HURRA_DIE_WEIT_GEHT_UNTER_H
