#include "fun.h"



int main() {
    // Чтение файла и подсчет частоты слов
    int num_words;
    WordCount *word_counts = count_words("Tol.txt", &num_words);

    char word_pairs[1000][2][MAX_WORD_LENGTH];
    int num_pairs = 0;

    while (1) {
        // Поиск самого популярного и самого непопулярного слова
        int dl_1, dll_2;
        char *most_common_word = find_most_common_word(word_counts, num_words, &dll_2);
        char *least_common_word = find_least_common_word(word_counts, num_words, &dl_1);

        if (dl_1 < dll_2) {
            break;
        }

        strcpy(word_pairs[num_pairs][0], most_common_word);
        strcpy(word_pairs[num_pairs][1], least_common_word);
        num_pairs++;

        // Замена слов в тексте
        char *text = read_file("input.txt");
        replace_words(text, most_common_word, least_common_word);

        // Сохранение результата в файл
        save_compressed_file("output.txt", text, ** word_pairs, num_pairs);
    }



    return 0;
}