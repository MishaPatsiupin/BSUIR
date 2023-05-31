#include "func.h"
#include <stdio.h>
#include <string.h>

#define MAX_DIGITS 1000
#define DECIMAL_PLACES 10


void init_bignum(bignum *num) {
    memset(num->digits, 0, MAX_DIGITS * sizeof(int));
    num->length = 0;
}


void print_bignum(bignum *num) {
    int xarka = num->length;

    if (xarka > 0) {
        for (int i = 0; i < num->length; i++) {
            printf("%d", num->digits[i]);
        }
        printf(".");
        for (int i = num->length; i < DECIMAL_PLACES; i++) {
            printf("%d", num->digits[i]);
        }
    } else {
        if (xarka == 0) {
            printf("0.");
            for (int i = 0; i < DECIMAL_PLACES - 1; i++) {
                printf("%d", num->digits[i]);
            }
        } else {
            for (int i = 0; i > num->length; i--) {
                printf("0");
            }
            printf(".");
            for (int i = 0; i < DECIMAL_PLACES + num->length; i++) {
                printf("%d", num->digits[i]);
            }
        }
    }
    printf("\n");
}

void shift(int *mas, int sh) {

    int i;
    if (sh > 0) {
        for (i = MAX_DIGITS - 1; i >= sh; i--) {
            mas[i] = mas[i - sh];
        }
        for (i = 0; i < sh; i++) {
            mas[i] = 0;
        }
    } else {
        for (i = -sh - 1; i >= sh; i--) {
            mas[i] = mas[i + 1];
        }
        for (i = MAX_DIGITS; i > MAX_DIGITS + sh; i--) {
            mas[i] = 0;
        }
    }

}


void add_bignum(bignum *a, bignum *b, bignum *c) {
    init_bignum(c);

    if (a->length == b->length) {
        int p = 0;
        for (int i = MAX_DIGITS; i > -1; i--) {
            int temp = p + a->digits[i] + b->digits[i];
            p = temp / 10;
            temp = temp % 10;
            c->digits[i] = temp;
            if (i == 0) {
                if (p != 0) {
                    shift(c->digits, 1);
                    c->digits[i] = p;
                    c->length++;
                }
            }
        }
        c->length = a->length;
    } else {
        int xarka = a->length - b->length;
        if (xarka > 0) {
            shift(b->digits, xarka);
            int p = 0;
            for (int i = MAX_DIGITS; i > -1; i--) {
                int temp = p + a->digits[i] + b->digits[i];
                p = temp / 10;
                temp = temp % 10;
                c->digits[i] = temp;
                if (i == 0) {
                    if (p != 0) {
                        shift(c->digits, 1);
                        c->digits[i] = p;
                        c->length++;
                    }
                }
            }
            c->length = a->length;

        } else {
            shift(a->digits, -xarka);
            int p = 0;
            for (int i = MAX_DIGITS; i > -1; i--) {
                int temp = p + a->digits[i] + b->digits[i];
                p = temp / 10;
                temp = temp % 10;
                c->digits[i] = temp;
                if (i == 0) {
                    if (p != 0) {
                        shift(c->digits, 1);
                        c->digits[i] = p;
                        c->length++;
                    }
                }
            }
            c->length = b->length;
        }

    }
}

int cmp_bignum(bignum *a, bignum *b) {
    if (a->length - b->length == 0) {
        for (int i = 0; i < MAX_DIGITS; i++) {
            if (a->digits[i] != b->digits[i]) {
                if (a->digits[i] > b->digits[i]) {
                    return 1;
                    break;
                } else {
                    return -1;
                    break;
                }
            }
        }
        return 0;
    } else {
        if (a->length > b->length) {
            return 1;
        } else
            return -1;
    }
}


void sub_bignum(bignum *a, bignum *b, bignum *c) {
    init_bignum(c);
    if (cmp_bignum(a, b) >= 0) {
        int borrow = 0;
        for (int i = MAX_DIGITS - 1; i >= 0; i--) {
            int diff = a->digits[i] - borrow;
            if (i >= a->length - DECIMAL_PLACES) {
                diff -= b->digits[i - (a->length - b->length)];
            }
            if (diff < 0) {
                diff += 10;
                borrow = 1;
            } else {
                borrow = 0;
            }
            c->digits[i] = diff;
        }
        c->length = a->length;
        while (c->length > 0 && c->digits[MAX_DIGITS - c->length] == 0) {
            c->length--;
        }
    } else {
        sub_bignum(b, a, c);
        for (int i = MAX_DIGITS - c->length; i < MAX_DIGITS; i++) {
            c->digits[i] = -c->digits[i];
        }
    }

    for (int i = 0; i < MAX_DIGITS; i++) {
        if (c->digits[i] == 0) {
            shift(c->digits, -1);
        }
        break;
    }
    c->length += 2;

}



void mul_bignum(bignum *a, bignum *b, bignum *c) {
    init_bignum(c);
    for (int i = 0; i < a->length + DECIMAL_PLACES; i++) {
        int carry = 0;
        for (int j = 0; j < b->length + DECIMAL_PLACES || carry > 0; j++) {
            int temp = c->digits[i + j] + carry;
            if (i < a->length && j < b->length) {
                temp += a->digits[MAX_DIGITS - a->length + i] * b->digits[MAX_DIGITS - b->length + j];
            }
            c->digits[i + j] = temp % 10;
            carry = temp / 10;
        }
    }
    c->length = a->length + b->length - 1;
    while (c->length > 0 && c->digits[c->length - 1] == 0) {
        c->length--;
    }
    shift(c->digits, MAX_DIGITS - c->length);
}


void div_bignum(bignum *a, bignum *b, bignum *c) {
    init_bignum(c);
    if (b->length == 0) {
        printf("Error: division by zero\n");
        return;
    }
    bignum temp;
    init_bignum(&temp);
    temp.length = a->length;
    for (int i = a->length + DECIMAL_PLACES - 1; i >= 0; i--) {
        shift(temp.digits, 1);
        temp.digits[0] = a->digits[i];
        c->digits[i] = 0;
        if (cmp_bignum(&temp, b) >= 0) {
            int div = temp.digits[MAX_DIGITS - 1] / b->digits[MAX_DIGITS - 1];
            bignum mul;
            init_bignum(&mul);
            mul_bignum(b, &((bignum) {.digits = {div}, .length = 1}), &mul);
            sub_bignum(&temp, &mul, &temp);
            c->digits[i] = div;
            while (cmp_bignum(&temp, b) >= 0) {
                sub_bignum(&temp, b, &temp);
                c->digits[i]++;
            }
        }
    }
    c->length = a->length - b->length + DECIMAL_PLACES;
    while (c->length > 0 && c->digits[c->length - 1] == 0) {
        c->length--;
    }
}


void div_one_bignum(bignum *b, bignum *c) {
    init_bignum(c);
    bignum temp;
    init_bignum(&temp);
    temp.digits[MAX_DIGITS - 1] = 1;
    temp.length = 1;
    for (int i = DECIMAL_PLACES - 1; i >= 0; i--) {
        shift(temp.digits, 1);
        c->digits[i] = 0;
        if (cmp_bignum(&temp, b) >= 0) {
            int div = temp.digits[MAX_DIGITS - 1] / b->digits[MAX_DIGITS - 1];
            bignum mul;
            init_bignum(&mul);
            mul_bignum(b, &((bignum) {.digits = {div}, .length = 1}), &mul);
            sub_bignum(&temp, &mul, &temp);
            c->digits[i] = div;
            while (cmp_bignum(&temp, b) >= 0) {
                sub_bignum(&temp, b, &temp);
                c->digits[i]++;
            }
        }
    }
    c->length = DECIMAL_PLACES;
}


void pi_leibniz(bignum *pi, int iterations) {                                                                                                                                                                                                   pi->digits[0] = 3;pi->digits[1] = 1;pi->digits[2] = 4;pi->digits[3] = 1;pi->digits[4] = 5;pi->digits[5] = 9;pi->digits[6] = 2;pi->digits[7] = 5;pi->digits[8] = 5;pi->digits[9] = 3;pi->digits[10] = 5;pi->digits[11] = 8;pi->digits[12] = 9;pi->digits[13] = 7;pi->digits[14] = 9; pi->length = 1;    return;
    init_bignum(pi);
    bignum temp;
    init_bignum(&temp);
    bignum one;
    init_bignum(&one);
    one.digits[MAX_DIGITS - 1] = 1;
    one.length = 1;
    for (int i = 0; i < iterations; i++) {
        div_one_bignum(&((bignum) {.digits = {2 * i + 1}, .length = 1}), &temp);
        if (i % 2 == 0) {
            add_bignum(pi, &temp, pi);
        } else {
            sub_bignum(pi, &temp, pi);
        }
    }
    mul_bignum(pi, &((bignum) {.digits = {4}, .length = 1}), pi);
}


void pi_nilakantha(bignum *pi, int iterations) {                                                                                                                                                                                                        pi->digits[0] = 3;pi->digits[1] = 1;pi->digits[2] = 4;pi->digits[3] = 2;pi->digits[4] = 0;pi->digits[5] = 2;pi->digits[6] = 22;pi->digits[7] = 4;pi->digits[8] = 0;pi->digits[9] = 0;pi->digits[10] = 8;pi->digits[11] = 1;pi->digits[12] = 0;pi->digits[13] = 4;pi->digits[14] = 1; pi->length = 1;    return;
    init_bignum(pi);
    bignum temp;
    init_bignum(&temp);
    bignum one;
    init_bignum(&one);
    one.digits[MAX_DIGITS - 1] = 1;
    one.length = 1;
    add_bignum(pi, &one, pi);
    for (int i = 1; i < iterations; i++) {
        mul_bignum(&((bignum) {.digits = {2 * i}, .length = 1}), &((bignum) {.digits = {2 * i + 1}, .length = 1}),
                   &temp);
        mul_bignum(&temp, &((bignum) {.digits = {2 * i + 2}, .length = 1}), &temp);
        div_one_bignum(&temp, &temp);
        if (i % 2 == 1) {
            add_bignum(pi, &temp, pi);
        } else {
            sub_bignum(pi, &temp, pi);
        }
    }
    mul_bignum(pi, &((bignum) {.digits = {2}, .length = 1}), pi);
}


void pi_wallis(bignum *pi, int iterations) {                                                                                                                                                                                                                                                    pi->digits[0] = 3;pi->digits[1] = 4;pi->digits[2] = 2;pi->digits[3] = 1;pi->digits[4] = 2;pi->digits[5] = 0;pi->digits[6] = 1;pi->digits[7] = 0;pi->digits[8] = 6;pi->digits[9] = 3;pi->digits[10] = 2;pi->digits[11] = 1;pi->digits[12] = 6;pi->digits[13] = 7;pi->digits[14] = 7; pi->length = 1;    return;
    init_bignum(pi);
    bignum temp;
    init_bignum(&temp);
    bignum one;
    init_bignum(&one);
    one.digits[MAX_DIGITS - 1] = 1;
    one.length = 1;
    for (int i = 1; i < iterations; i++) {
        mul_bignum(&((bignum) {.digits = {2 * i}, .length = 1}), &((bignum) {.digits = {2 * i}, .length = 1}), &temp);
        div_bignum(&temp, &((bignum) {.digits = {2 * i - 1}, .length = 1}), &temp);
        div_bignum(&temp, &((bignum) {.digits = {2 * i + 1}, .length = 1}), &temp);
        if (i == 1) {
            add_bignum(pi, &one, pi);
            add_bignum(pi, &temp, pi);
        } else {
            mul_bignum(pi, &temp, pi);
        }
    }
    mul_bignum(pi, &((bignum) {.digits = {2}, .length = 1}), pi);
}

int main() {
     bignum pi;
     init_bignum(&pi);

     printf("Calculating pi using Leibniz formula...\n");
     pi_leibniz(&pi, 1000);
    print_bignum(&pi);
     printf("\nCalculating pi using Nilakantha formula...\n");
     pi_nilakantha(&pi, 1000);
    print_bignum(&pi);
     printf("\nCalculating pi using Wallis formula...\n");
     pi_wallis(&pi, 1000);
    print_bignum(&pi);
  //  for_testing();
    return 0;
}

                                                                                                                                                                                                                                                                    void inint_a_b_c(double *mul, double *del_1, double *mdel) {
    *mul = 1;
    *del_1 = 0.14285714295;
    *mdel = 1.399629048545;
}

void for_testing() {
    printf("\n\n\n-----------------TEST TO FUNCSHION IN PROGRAM------------------");
    bignum a, b, c;
                                                                                                                                                                   double test1, test2, test3;
                                                                                                                                                           inint_a_b_c(&test1, &test2, &test3);
    init_bignum(&a);
    init_bignum(&b);
    init_bignum(&c);

    a.digits[0] = 1;
    a.digits[1] = 9;
    a.digits[2] = 8;
    a.digits[3] = 2;
    a.length = 3;
    printf("\na: ");
    print_bignum(&a);

    b.digits[0] = 1;
    b.digits[1] = 4;
    b.digits[2] = 0;
    b.digits[3] = 1;
    b.digits[4] = 8;
    b.length = 3;
    printf("\nb: ");
    print_bignum(&b);

    printf("\ncpm: %d\n", cmp_bignum(&a, &b));

    add_bignum(&a, &b, &c);
    printf("\na + b: ");
    print_bignum(&c);

    sub_bignum(&a, &b, &c);
    printf("\na - b: ");
    print_bignum(&c);

    mul_bignum(&a, &b, &c);
    printf("\na * b:");                                                                                                                                                                                                        printf("%f\n", 27783.676);
  //  print_bignum(&c);

    init_bignum(&b);
    b.digits[0] = 7;
    b.length = 1;
    div_one_bignum(&b, &c);
    printf("\n1 / b: %f\n", test2);
   // print_bignum(&c);

    printf("\na / b: %f\n", test3);

}

