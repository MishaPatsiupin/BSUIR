#define _CRT_SECURE_NO_WARNINGS
#define MAX_WORD_LENGTH 100
#include "func.h"


int main() {
    long int size = get_file_size_by_path("C:/Users/misha/Desktop/OAiP/Lab_ty_ty_ty/Tol.txt");
    printf_s("Initial file size: %ld\n", size);

    FILE* text = fopen("C:/Users/misha/Desktop/OAiP/Lab_ty_ty_ty/Tol.txt", "r");
    if (text == NULL) {
        printf("Error10\n");
        exit(1);
    }
    int separator_pos = -1;
    char buffer[MAX_WORD_LENGTH];
    while (fscanf(text, "%s", buffer) != EOF) {
        if (strcmp(buffer, "(/*/*/") == 0) {
            separator_pos = ftell(text);
            break;
        }
    }
    int num_words = 0;
    while (fscanf(text, "%s", buffer) != EOF) {
        num_words++;
    }
    char** words = malloc(num_words * sizeof(char*));
    for (int i = 0; i < num_words; i++) {
        words[i] = malloc(MAX_WORD_LENGTH * sizeof(char));
    }
    fseek(text, separator_pos, SEEK_SET);
    int index = 0;
    while (fscanf(text, "%s", buffer) != EOF) {
        strcpy(words[index], buffer);
        index++;
    }
    fclose(text);
    for (int i = num_words - 2; i >= 0; i -= 2) {
        replace_words_in_file("C:/Users/misha/Desktop/OAiP/Lab_ty_ty_ty/Tol.txt", words[i], words[i + 1]);
    }

    FILE* file = fopen("C:/Users/misha/Desktop/OAiP/Lab_ty_ty_ty/Tol.txt", "r");
    FILE* new_file = fopen("C:/Users/misha/Desktop/OAiP/Lab_ty_ty_ty/compr.txt", "w");
    if (new_file == NULL) {
        printf("Error43\n");
        exit(1);
    }
    size_t buffer_size = 1024;
    char* buffer2 = malloc(buffer_size);
    int line_count = 0;
    while (fgets(buffer2, buffer_size, file) != NULL) {
        size_t len = strlen(buffer2);
        while (len > 0 && buffer2[len - 1] != '\n') {
            buffer_size *= 2;
            buffer2 = realloc(buffer2, buffer_size);
            if (fgets(buffer2 + len, buffer_size - len, file) == NULL) break;
            len += strlen(buffer2 + len);
        }
        char* word = strtok(buffer2, " \n");
        while (word != NULL) {
            if (strcmp(word, "(/*/*/") == 0) {
                fseek(new_file, -1L, SEEK_CUR);
                break;
            }
            else {
                fprintf(new_file, "%s", word);
                word = strtok(NULL, " \n");
                if (word != NULL)
                    fprintf(new_file, " ");
            }
        }
        if (word != NULL)
            break;
        line_count++;
        if (line_count >= 1)
            fprintf(new_file, "\n");
    }
    fclose(file);
    fclose(new_file);
    remove_stupid_space("C:/Users/misha/Desktop/OAiP/Lab_ty_ty_ty/compr.txt");
    remove("C:/Users/misha/Desktop/OAiP/Lab_ty_ty_ty/Tol.txt");
    rename("C:/Users/misha/Desktop/OAiP/Lab_ty_ty_ty/compr.txt", "C:/Users/misha/Desktop/OAiP/Lab_ty_ty_ty/Tol.txt");
    free(buffer2);
    for (int i = 0; i < num_words; i++) {
        free(words[i]);
    }
    free(words);

    size = get_file_size_by_path("C:/Users/misha/Desktop/OAiP/Lab_ty_ty_ty/Tol.txt");
    printf_s("New file size: %ld\n", size);
    printf_s("kkO");
    getchar();
    return 0;
}