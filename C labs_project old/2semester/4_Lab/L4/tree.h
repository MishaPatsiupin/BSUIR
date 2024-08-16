#ifndef INC_5_1_FUNCTIONS_H
#define INC_5_1_FUNCTIONS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <time.h>
#define MAX_NAME_LENGTH 100

typedef struct node {
    char name[MAX_NAME_LENGTH];
    int index;
    struct node* yes;
    struct node* no;
} Node;
Node* newNode(char* name, int index);
void addCharacter(Node* current_node);
void traverseTree(Node* current_node);
int getMaxIndex(Node* current_node);
void saveTree(FILE* file, Node* current_node, int max_index);
Node* loadTree(FILE* file, int index);
void freeTree(Node* root);
void logAction(FILE* log_file, const char* format);
#endif //INC_5_1_FUNCTIONS_H
