#ifndef FIGHA_FUN_H
#define FIGHA_FUN_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_LENGTH 50

typedef struct {
    char word[MAX_WORD_LENGTH];
    int count;
} WordCount;

WordCount *count_words(char *filename, int *num_words);
char *find_most_common_word(WordCount *word_counts, int num_words, int *dl);
char *find_least_common_word(WordCount *word_counts, int num_words, int *dl);
void save_compressed_file(char *filename, char *text, char **word_pairs, int num_pairs);
void replace_words(char *text, char *word1, char *word2);
char *read_file(char *filename);



















#endif //FIGHA_FUN_H
