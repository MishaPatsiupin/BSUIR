#ifndef PRACTICA_FUNC_H
#define PRACTICA_FUNC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_DIGITS 1000
#define DECIMAL_PLACES 15

typedef struct {
    int digits[MAX_DIGITS];
    int length;
} bignum;

void init_bignum(bignum *num);
void for_testing();
void shift(int *mas, int sh);
void add_bignum(bignum *a, bignum *b, bignum *c);
int cmp_bignum(bignum *a, bignum *b);
void sub_bignum(bignum *a, bignum *b, bignum *c);
void mul_bignum(bignum *a, bignum *b, bignum *c);
void div_bignum(bignum *a, bignum *b, bignum *c);
void div_one_bignum(bignum *b, bignum *c);

void pi_leibniz(bignum *pi, int iterations);
void pi_nilakantha(bignum *pi, int iterations);
void pi_wallis(bignum *pi, int iterations);

void compare_pi(bignum *pi, const char *constant);
void print_bignum(bignum *num);
void for_testing();






#endif //PRACTICA_FUNC_H
