#define f_name "C:/Users/misha/Desktop/OAiP/Lab_ty_ty_ty/Tol.txt"
#define _CRT_SECURE_NO_WARNINGS
#define MAX_WORD_LENGTH 100
#include "func.h"


int main() {

    long int size;
    size = get_file_size_by_path(f_name);
    printf_s("Initial file size: %ld\n", size);

    int num_words = 0;
    Word* words = read_words_from_file(f_name, &num_words);
    Word* most_frequent_long_word;
    Word* least_frequent_short_word;
    int max_pairs = num_words / 2;
    char** pairs = malloc(max_pairs * sizeof(char*));
    for (int i = 0; i < max_pairs; i++) {
        pairs[i] = malloc(MAX_WORD_LENGTH * 2 + 1);
        pairs[i][0] = '\0';
    }
    int pair_count = 0;
    while (1) {
        find_and_save_words(words, num_words, &most_frequent_long_word, &least_frequent_short_word);
        int old_size = find_old_size(most_frequent_long_word, least_frequent_short_word);
        //
        int new_size = find_new_size(most_frequent_long_word, least_frequent_short_word);
        //
        if (new_size + strlen(most_frequent_long_word) + 1 + strlen(least_frequent_short_word) >= old_size) {//+ добавить место под свои слова для припащения
            break;
        }
        replace_words_in_file(f_name, most_frequent_long_word, least_frequent_short_word);
        sprintf(pairs[pair_count], "%s %s", most_frequent_long_word->word, least_frequent_short_word->word);
        pair_count++;
    }
    FILE* file = fopen(f_name, "a");
    fprintf(file, " (/*/*/ ");
    for (int i = 0; i < pair_count; i++) {
        fprintf(file, "%s\n", pairs[i]);
    }
    fclose(file);
    for (int i = 0; i < max_pairs; i++) {
        free(pairs[i]);
    }
    free(pairs);
    free(words);

    size = get_file_size_by_path(f_name);
    printf_s("New file size: %ld\n", size);
    printf_s("Okk");
    getchar();
    return 0;
}