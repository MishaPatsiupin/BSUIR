#define _CRT_SECURE_NO_WARNINGS
#define MAX_WORD_LENGTH 100
#include "func.h"

long int get_file_size_by_path(const char* path) {
    struct stat st;
    if (stat(path, &st) == 0) {
        return st.st_size;
    }
    else {
        return -1;
    }
}

void replace_words_in_file(char* filename, char* word_a, char* word_b) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error8\n");
        exit(1);
    }

    FILE* new_file = fopen("C:/Users/misha/Desktop/OAiP/Lab_ty_ty_ty/compr.txt", "w");
    if (new_file == NULL) {
        printf("Error14\n");
        exit(1);
    }

    size_t buffer_size = 1024;
    char* buffer = malloc(buffer_size);
    while (fgets(buffer, buffer_size, file) != NULL) {
        size_t len = strlen(buffer);
        while (len > 0 && buffer[len - 1] != '\n') {
            buffer_size *= 2;
            buffer = realloc(buffer, buffer_size);
            if (fgets(buffer + len, buffer_size - len, file) == NULL) break;
            len += strlen(buffer + len);
        }
        char* word = strtok(buffer, " \n");
        while (word != NULL) {
            if (strcmp(word, word_a) == 0) {
                fprintf(new_file, "%s", word_b);
            }
            else if (strcmp(word, word_b) == 0) {
                fprintf(new_file, "%s", word_a);
            }
            else {
                fprintf(new_file, "%s", word);
            }
            word = strtok(NULL, " \n");
            if (word != NULL)
                fprintf(new_file, " ");
        }
        fprintf(new_file, "\n");
    }

    fclose(file);
    fclose(new_file);

    remove(filename);
    rename("C:/Users/misha/Desktop/OAiP/Lab_ty_ty_ty/compr.txt", filename);

    free(buffer);
}
void remove_stupid_space(char* filename) {
    FILE* file = fopen(filename, "r+");
    if (file == NULL) {
        printf("Error57\n", filename);
        exit(1);
    }
    fseek(file, -1L, SEEK_END);
    char last_char;
    fread(&last_char, 1, 1, file);
    if (last_char == ' ') {
        long pos = ftell(file);
        int fd = _fileno(file);
        _chsize_s(fd, pos - 1);
    }
    fclose(file);
}