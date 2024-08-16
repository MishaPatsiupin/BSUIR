#define _CRT_SECURE_NO_WARNINGS
#include "tree.h"

int main() {
    Node* root = NULL;
    FILE* file = fopen("C:/Users/misha/Desktop/tree.txt", "r");
    FILE* log_file = fopen("C:/Users/misha/Desktop/log.txt", "w+");
    logAction(log_file, "Program started");
    printf("----------------Welcome to the territory of the TV series 'The Big Bang Theory'----------------\n");
    if (file != NULL) {
        root = loadTree(file, 1);
        fclose(file);
        logAction(log_file, "Tree loaded from file");
    }
    else {
        root = newNode("Does the character like science?", 1);
        root->yes = newNode("Does the character have a doctorate?", 3);
        root->no = newNode("Is this a close relative of one of the main characters?", 2);
        logAction(log_file, "New tree created");
    }
    while (true) {
        printf("Guess a character and I'll try to guess it.\n");
        logAction(log_file, "New game started");
        traverseTree(root, log_file);
        printf("Do you want to play again? (yes/no) ");
        char answer[MAX_NAME_LENGTH];
        fgets(answer, MAX_NAME_LENGTH, stdin);
        if (strncmp(answer, "yes", 2) != 0) break;
        logAction(log_file, "User chose to play again");
    }
    file = fopen("C:/Users/misha/Desktop/tree.txt", "w");
    int max_index = getMaxIndex(root);
    saveTree(file, root, max_index);
    fclose(file);
    freeTree(root);
    root = NULL;
    logAction(log_file, "Program exited");
    fclose(log_file);
    return 0;
}