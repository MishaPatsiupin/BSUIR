#ifndef INC_5_1_FUNCTIONS_H
#define INC_5_1_FUNCTIONS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <locale.h>
#include <sys/stat.h>

void replace_words_in_file(char* filename, char* word_a, char* word_b);
void remove_stupid_space(char* filename);
#endif //INC_5_1_FUNCTIONS_H