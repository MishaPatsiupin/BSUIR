#include "fun.h"


WordCount *count_words(char *filename, int *num_words) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file %s\n", filename);
        exit(1);
    }я

    WordCount *word_counts = malloc(sizeof(WordCount) * 1000);
    *num_words = 0;

    char word[MAX_WORD_LENGTH];
    while (fscanf(file, "%s", word) != EOF) {
        int found = 0;
        for (int i = 0; i < *num_words; i++) {
            if (strcmp(word_counts[i].word, word) == 0) {
                word_counts[i].count++;
                found = 1;
                break;
            }
        }
        if (!found) {
            strcpy(word_counts[*num_words].word, word);
            word_counts[*num_words].count = 1;
            (*num_words)++;
        }
    }

    fclose(file);
    return word_counts;
}

char *find_most_common_word(WordCount *word_counts, int num_words, int *dl) {
    int max_count = 0;
    char *most_common_word = NULL;
    for (int i = 0; i < num_words; i++) {
        if (word_counts[i].count > max_count) {
            max_count = word_counts[i].count;
            most_common_word = word_counts[i].word;
            *dl = (word_counts[i].count * strlen(most_common_word));
        }
    }
    return most_common_word;
}

char *find_least_common_word(WordCount *word_counts, int num_words, int *dl) {
    int min_count = 1000000;
    char *least_common_word = NULL;
    for (int i = 0; i < num_words; i++) {
        if (word_counts[i].count < min_count) {
            min_count = word_counts[i].count;
            least_common_word = word_counts[i].word;
            *dl = word_counts[i].count * strlen(least_common_word);
        }
    }
    return least_common_word;
}


void replace_words(char *text, char *word1, char *word2) {
    char *pos = strstr(text, word1);
    while (pos != NULL) {
        memmove(pos + strlen(word2), pos + strlen(word1), strlen(pos + strlen(word1)) + 1);
        memcpy(pos, word2, strlen(word2));
        pos = strstr(pos + strlen(word2), word1);
    }
}

void save_compressed_file(char *filename, char *text, char **word_pairs, int num_pairs) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file %s\n", filename);
        exit(1);
    }
    for (int i = 0; i < num_pairs; i++) {
        fprintf(file, "%s %s ", word_pairs[i][0], word_pairs[i][1]);
    }
    fprintf(file, "\n");
    fprintf(file, "%s", text);
    fclose(file);
}

char *read_file(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file %s\n", filename);
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = malloc(length + 1);
    fread(buffer, 1, length, file);
    buffer[length] = '\0';

    fclose(file);
    return buffer;
}

















