#include "functions.h"
#include <stdio.h>
#include <malloc.h>

void printMenu() {
    printf("0 - Exit\n");
    printf("1 if you want to find most repeated word\n");
    printf("2 if you want to delete all words that start from symbol c \nfrom str s and which are not in s0\n");
}

int getChoice() {
    char num;
    printf("Enter your choice: ");
    do {
        scanf("%c", &num);
        num -= '0';
        if (num  < 0 || num  > 3) {
            printf("Wrong number, try again\n");
            rewind(stdin);
            num = -1;
        }
    } while (num < 0 || num > 3);

    return (int)num;
}

void inputString(char *str) {
    // read all line
    fflush(stdin);
    gets(str);
}

int findWordRepetitions(const char *str,const char *word) {
    int i = 0;
    int amount = 0;
    while (str[i] != '\0') {
        if (str[i] == word[0]) {
            int j = 0;
            while (word[j] != '\0') {
                if (str[i + j] != word[j]) {
                    break;
                }
                j++;
            }
            if (word[j] == '\0' && (str[i + j] == ' ' || str[i + j] == '\0')) {
                amount++;
            }
        }
        i++;
    }
    return amount;
}

//find most repeated word making own algorithm
void task1() {
    char *str = malloc(1000 * sizeof(char));
    printf("Enter string: ");
    inputString(str);
    int i = 0;
    int wordStart;
    int wordEnd = -2;
    int maxWordStart = 0;
    int maxWordEnd = 0;
    int maxWordAMount = 0;

    do {
        // finding word
        if (str[i] == ' ' || str[i] == '\0') {
            wordStart = wordEnd + 2;
            wordEnd = i - 1;
            char word[wordEnd - wordStart + 2];
            word[wordEnd - wordStart + 1] = '\0';
            for (int j = wordStart; j < wordEnd + 1; ++j) {
                word[j - wordStart] = str[j];
            }
            // finding amount of word
            int tempAmount = findWordRepetitions(str, word);
            if (tempAmount >  maxWordAMount) {
                maxWordStart = wordStart;
                maxWordEnd = wordEnd;
                maxWordAMount = tempAmount;
            }
        }
        i++;
    } while (str[i] != '\0');
    //print result
    printf("Most repeated word is: ");
    for (int j = maxWordStart; j < maxWordEnd + 1; ++j) {
        printf("%c", str[j]);
    }
    printf("\n");
    free(str);
}

void cleanBuf(char *buf, int size) {
    for (int i = 0; i < size; ++i) {
        buf[i] = '\0';
    }
}

void makeWithDelition(const char *s,const char *s0, const char c, char *res) {
    int i = 0;
    int pos = 0;
    int wordStart;
    int wordEnd = -2;
    do {
       if (s[i] == ' ' || s[i] == '\0'){
           wordStart = wordEnd + 2;
           wordEnd = i - 1;
           if (s[wordStart] != c){
               for (int j = 0; j < wordEnd - wordStart + 1; ++j) {
                   res[pos++] = s[wordStart + j];
               }
               res[pos++] = ' ';
           }else if (s[wordStart] == c){
               char word[wordEnd - wordStart + 1 + 1];
                word[wordEnd - wordStart + 1] = '\0';
               for (int j = 0; j < wordEnd - wordStart + 1; ++j) {
                   word[j] = s[wordStart + j];
               }
               if (findWordRepetitions(s0, word) != 0){
                   for (int j = 0; j < wordEnd - wordStart + 1; ++j) {
                       res[pos++] = s[wordStart + j];
                   }
                   res[pos++] = ' ';
               }
           }
       }

       i++;
    }while (s[i] != '\0');
    printf("\n");
}

// input strings s0, s and char C. delete all words from s that start with char C and not in s0
void task2() {
    char *s0 = (char*)malloc(1000 * sizeof(char));
    cleanBuf(s0, 1000);
    char *s = (char*)malloc(1000 * sizeof(char));
    cleanBuf(s, 1000);
    char c;

    printf("Enter string s: ");
    fflush(stdin);
    inputString(s);

    printf("Enter string s0: ");
    fflush(stdin);
    inputString(s0);

    printf("Enter char c: ");
    fflush(stdin);
    scanf("%c", &c);

    char *res = (char*)malloc(1000 * sizeof(char));
    cleanBuf(res, 1000);
    printf("Result:");
    makeWithDelition(s, s0, c, res);

    puts(res);
    free(s0);
    free(s);
    free(res);
    fflush(stdin);
}

int calcAmountOfWords(const char *str) {
    int i = 0;
    int amount = 0;
    while (str[i] != '\0') {
        if (str[i] == ' ') {
            amount++;
        }
        i++;
    }
    return amount;
}

// merge sort of strings by result of arr of int
void mergeSort(char **arr, int *arrInt, int left, int right) {
    if (left < right) {
        int middle = (left + right) / 2;
        mergeSort(arr, arrInt, left, middle);
        mergeSort(arr, arrInt, middle + 1, right);
        int i = left;
        int j = middle + 1;
        int k = 0;
        char **temp = (char**)malloc((right - left + 1) * sizeof(char*));
        int *tempInt = (int*)malloc((right - left + 1) * sizeof(int));
        while (i <= middle && j <= right) {
            if (arrInt[i] < arrInt[j]) {
                temp[k] = arr[i];
                tempInt[k] = arrInt[i];
                i++;
            } else {
                temp[k] = arr[j];
                tempInt[k] = arrInt[j];
                j++;
            }
            k++;
        }
        while (i <= middle) {
            temp[k] = arr[i];
            tempInt[k] = arrInt[i];
            i++;
            k++;
        }
        while (j <= right) {
            temp[k] = arr[j];
            tempInt[k] = arrInt[j];
            j++;
            k++;
        }
        for (int l = left; l <= right; ++l) {
            arr[l] = temp[l - left];
            arrInt[l] = tempInt[l - left];
        }
        free(temp);
        free(tempInt);
    }
}


void task3() {
    printf("Input amount of strings: ");
    int amount;
    scanf("%d", &amount);

    char **arr = (char**)malloc(amount * sizeof(char*));
    for (int i = 0; i < amount; ++i) {
        arr[i] = (char*)malloc(1000 * sizeof(char));
        cleanBuf(arr[i], 1000);
    }
    printf("\n");
    for (int i = 0; i < amount; ++i) {
        printf("Input string %d: ", i + 1);
        fflush(stdin);
        inputString(arr[i]);
    }

    int *arr2 = malloc(amount * sizeof(int));
    for (int i = 0; i < amount; ++i) {
        arr2[i] = calcAmountOfWords(arr[i]);
    }
    mergeSort(arr, arr2, 0, amount - 1);
    printf("\n");
    for (int i = 0; i < amount; ++i) {
        puts(arr[i]);
    }
}
