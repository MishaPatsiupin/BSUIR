#include "func.h"
#include <stdio.h>
#include <string.h>

#define MAX_DIGITS 1000
#define DECIMAL_PLACES 10


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

