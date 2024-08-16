#ifndef INC_5_1_FUNCTIONS_H
#define INC_5_1_FUNCTIONS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <sys/stat.h>

typedef struct {
    char word[MAX_WORD_LENGTH];
    int count;
    int was_used;
} Word;

Word* read_words_from_file(char* filename, int* num_words);
void find_and_save_words(Word words[], int num_words, Word** most_frequent_long_word_ptr, Word** least_frequent_short_word_ptr);
char* read_line(FILE* file);
void replace_words_in_file(char* filename, Word* word_a, Word* word_b);
int find_old_size(Word* word1, Word* word2);
int find_new_size(Word* word1, Word* word2);

void print_words(Word words[], int size);
#endif //INC_5_1_FUNCTIONS_H