#define _CRT_SECURE_NO_WARNINGS
#define MAX_WORD_LENGTH 100
#include "func.h"

Word* read_words_from_file(char* filename, int* num_words) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error\n");
        exit(1);
    }
    Word* words = malloc(sizeof(Word));
    char buffer[MAX_WORD_LENGTH];
    while (fscanf(file, "%s", buffer) != EOF) {
        int found = 0;
        for (int i = 0; i < *num_words; i++) {
            if (strcmp(buffer, words[i].word) == 0) {
                words[i].count++;
                found = 1;
                break;
            }
        }
        if (!found) {
            words = realloc(words, (*num_words + 1) * sizeof(Word));
            strcpy(words[*num_words].word, buffer);
            words[*num_words].count = 1;
            words[*num_words].was_used = 0;
            (*num_words)++;
        }
    }
    fclose(file);
    return words;
}
void find_and_save_words(Word words[], int num_words, Word** most_frequent_long_word_ptr, Word** least_frequent_short_word_ptr) {
    Word* most_frequent_long_word = NULL;
    Word* least_frequent_short_word = NULL;

    for (int i = 0; i < num_words; i++) {
        if (!words[i].was_used) {
            if (most_frequent_long_word == NULL || (strlen(words[i].word) > strlen(most_frequent_long_word->word) && words[i].count > most_frequent_long_word->count)) {
                most_frequent_long_word = &words[i];
            }
            if (least_frequent_short_word == NULL || (strlen(words[i].word) < strlen(least_frequent_short_word->word) && words[i].count < least_frequent_short_word->count)) {
                least_frequent_short_word = &words[i];
            }
        }
    }
    if (!most_frequent_long_word->was_used && !least_frequent_short_word->was_used) {
        most_frequent_long_word->was_used = 1;
        least_frequent_short_word->was_used = 1;
        *most_frequent_long_word_ptr = most_frequent_long_word;
        *least_frequent_short_word_ptr = least_frequent_short_word;
    }
}

char* read_line(FILE* file) {
    size_t buffer_size = 1024;
    char* buffer = malloc(buffer_size);
    size_t len = 0;
    while (fgets(buffer + len, buffer_size - len, file) != NULL) {
        len += strlen(buffer + len);
        if (buffer[len - 1] == '\n') {
            break;
        }
        buffer_size *= 2;
        buffer = realloc(buffer, buffer_size);
    }
    if (len == 0) {
        free(buffer);
        return NULL;
    }
    return buffer;
}
void replace_words_in_file(char* filename, Word* word_a, Word* word_b) {
    FILE* file = fopen(filename, "r");
    FILE* new_file = tmpfile();
    char buffer[MAX_WORD_LENGTH];
    char* line = NULL;
    char* next_line = read_line(file);
    while (next_line != NULL) {
        line = next_line;
        char* word = strtok(line, " \n");
        while (word != NULL) {
            if (strcmp(word, word_a->word) == 0) {
                fprintf(new_file, "%s", word_b->word);
            }
            else if (strcmp(word, word_b->word) == 0) {
                fprintf(new_file, "%s", word_a->word); 
            }
            else {
                fprintf(new_file, "%s", word);
            }
            word = strtok(NULL, " \n");
            if (word != NULL) {
                fprintf(new_file, " ");
            }
        }
        next_line = read_line(file);
        if (next_line != NULL) {
            fprintf(new_file, "\n");
        }
        free(line);
    }
    fclose(file);
    file = fopen(filename, "w");
    rewind(new_file);
    while (fgets(buffer, MAX_WORD_LENGTH, new_file) != NULL) {
        fputs(buffer, file);
    }
    fclose(file);
    fclose(new_file);
}
int find_old_size(Word* word1, Word* word2) {
    return strlen(word1->word) * word1->count + strlen(word2->word) * word2->count;
}
int find_new_size(Word* word1, Word* word2) {
    return strlen(word1->word) * word2->count + strlen(word2->word) * word1->count + strlen(word1->word) + strlen(word2->word)+2;
}

long int get_file_size_by_path(const char* path) {
    struct stat st;
    if (stat(path, &st) == 0) {
        return st.st_size;
    }
    else {
        return -1;
    }
}