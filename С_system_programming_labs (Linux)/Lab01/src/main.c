#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <string.h>
#include <getopt.h>
#include <stdbool.h>
#include <locale.h>

enum OPTIONS {
    IS_REG = 0,
    IS_LNK = 1,
    IS_DIR = 2,
    IS_SORT = 3,
    IS_ALL_OPT = 4
};

#define COUNT_OPTIONS 5
#define CURRENT_DIR "."
#define RELOAD_OPTIND optind = 1
#define FIND_PATH_DIR while (getopt(argc, argv, "ldfs") != -1) { }


bool DOES_THE_FILE_EXIST(const char *path);
int compare(const void *first, const void *second);
void alloc_memory_and_copy(char ***arr, size_t *arr_size, size_t *capacity, const char *fullpath);
void dirwalk(char *path, const bool options[], char ***arr, size_t *arr_size, size_t *capacity);
int lstat(const char *path, struct stat *buf);


/**
 *Функция, обрабатывает аргументы командной строки и опции
  проверяет существование указанного пути
  выполняет обход и сбор информации о файлах и директориях
  сортирует полученные данные (при необходимости)
  выводит результат пользователю
 *
*/
int main(int argc, char *argv[]) {

    char **arr_path = NULL;
    size_t size_arr = 0, capacity = 1;
    bool flag_free = false;

    FIND_PATH_DIR;
    char *path = NULL;
    if (optind < argc){
        const int str_len = strlen(argv[optind]);
        path = (char *)malloc(str_len + 1);
        strncpy(path, argv[optind], str_len + 1);
        flag_free = true;
    }
    else{
        path = CURRENT_DIR;
    }

    RELOAD_OPTIND;
    bool options[COUNT_OPTIONS] = {false, false, false, false, false};
    if (!DOES_THE_FILE_EXIST(path)){
        printf("Check if the directory or file actually exists!");
    }
    else{
        bool flag_are_there_any_options = false;
        int option_ch;
        while ((option_ch = getopt(argc, argv, "ldfs")) != -1){
            
            switch (option_ch){
            case 'l':{
                options[IS_LNK] = true;
                break;
            }
            case 'f':{
                options[IS_REG] = true;
                break;
            }
            case 'd':{
                options[IS_DIR] = true;
                break;
            }
            case 's':{
                options[IS_SORT] = true;
                break;
            }
            default:
                break;
            }

            if (option_ch != 's')
                flag_are_there_any_options = true;
        }
        if (!flag_are_there_any_options)
            options[IS_ALL_OPT] = true;
        dirwalk(path, options, &arr_path, &size_arr, &capacity);
    }
    if (arr_path){
        if (options[IS_SORT])
            qsort(arr_path, size_arr, sizeof(char *), compare);
        for (size_t j = 0; j < size_arr; ++j){
            printf("%s\n", arr_path[j]);
        }
        for (size_t i = 0; i < size_arr; ++i)
            free(arr_path[i]);
        free(arr_path);
    }
    if (flag_free)
        free(path);
    return 0;
}

// Проверяет, существует ли файл или директория с заданным путем
bool DOES_THE_FILE_EXIST(const char *path){
    DIR *d;
    if ((d = opendir(path)) == NULL)
        return false;
    closedir(d);
    return true;
}

// Сравнивает строки для использования в функции qsort()
int compare(const void *first, const void *second){
    return strcoll(*(char **)first, *(char **)second);
}

// Выделяет память и копирует строку в массив
void alloc_memory_and_copy(char ***arr, size_t *arr_size, size_t *capacity, const char *fullpath){
    if (*arr_size + 1 == *capacity){
        *capacity *= 2;
        *arr = (char **)realloc(*arr, *capacity * sizeof(char *));
    }
    int len_path = strlen(fullpath) + 1;
    (*arr)[*arr_size] = (char *)malloc(len_path);
    strncpy((*arr)[(*arr_size)++], fullpath, len_path);
}

// Рекурсивная функция для обхода директории и сбора путей файлов и директорий
void dirwalk(char *path, const bool options[], char ***arr, size_t *arr_size, size_t *capacity){
    if (!path)
        return;

    struct dirent *dir;
    struct stat sb;
    DIR *d = opendir(path);

    while ((dir = readdir(d))){
        char fullpath[MAXPATHLEN];
        if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0){
            continue;
        }

        sprintf(fullpath, "%s/%s", path, dir->d_name);

        if (lstat(fullpath, &sb) == -1){
            continue;
        }

        if ((sb.st_mode & __S_IFMT) == __S_IFDIR){//дир
            if (options[IS_DIR] || options[IS_ALL_OPT])
                alloc_memory_and_copy(arr, arr_size, capacity, fullpath);
            dirwalk(fullpath, options, arr, arr_size, capacity);
        }

        else if ((sb.st_mode & __S_IFMT) == __S_IFLNK && (options[IS_LNK] || options[IS_ALL_OPT])) //ссылка
            alloc_memory_and_copy(arr, arr_size, capacity, fullpath);
        else if ((sb.st_mode & __S_IFMT) == __S_IFREG && (options[IS_REG] || options[IS_ALL_OPT])) //файл
            alloc_memory_and_copy(arr, arr_size, capacity, fullpath);
    }
    closedir(d);
}
