#define _CRT_SECURE_NO_WARNINGS
#include "tree.h"
Node* newNode(const char* name, int index) {
    Node* node = malloc(sizeof(Node));
    strncpy(node->name, name, MAX_NAME_LENGTH);
    node->index = index;
    node->yes = NULL;
    node->no = NULL;
    return node;
}
void addCharacter(Node* current_node) {
    char new_character[MAX_NAME_LENGTH];
    char question[MAX_NAME_LENGTH];
    printf("\nWho is it? ");
    fgets(new_character, MAX_NAME_LENGTH, stdin);
    new_character[strcspn(new_character, "\n")] = 0;
    printf("Write a question that distinguishes %s from %s: ", new_character, current_node->name);
    fgets(question, MAX_NAME_LENGTH, stdin);
    question[strcspn(question, "\n")] = 0;
    Node* new_question = newNode(question, current_node->index);
    Node* new_ch_node = newNode(new_character, current_node->index * 2 + 1);
    Node* old_ch_node = newNode(current_node->name, current_node->index * 2);
    new_question->yes = new_ch_node;
    new_question->no = old_ch_node;
    *current_node = *new_question;
}
void traverseTree(Node* current_node, FILE* log_file) {
    if (current_node->yes == NULL && current_node->no == NULL) {
        printf("I think it's %s.\n", current_node->name);
        logAction(log_file, "Program output: I think it's %s", current_node->name);
        
        printf("Did I guess correctly? (yes/no)");
        char answer[MAX_NAME_LENGTH];
        fgets(answer, MAX_NAME_LENGTH, stdin);
        logAction(log_file, "User input: %s", answer);
        if (strncmp(answer, "yes", 2) == 0) {
            printf("I won!\n");
            logAction(log_file, "Program output: I won!");
        }
        else {
            addCharacter(current_node);
        }
        return;
    }
    printf("%s (yes/no) ", current_node->name);
    logAction(log_file, "Program output: %s (yes/no)", current_node->name);
    char answer[MAX_NAME_LENGTH];
    fgets(answer, MAX_NAME_LENGTH, stdin);
    logAction(log_file, "User input: %s", answer);
    if (strncmp(answer, "yes", 2) == 0) {
        traverseTree(current_node->yes, log_file);
    }
    else {
        traverseTree(current_node->no, log_file);
    }
}
int getMaxIndex(Node* current_node) {
    if (current_node == NULL) return 0;
    int max_index = current_node->index;
    int left_max_index = getMaxIndex(current_node->yes);
    int right_max_index = getMaxIndex(current_node->no);
    if (left_max_index > max_index) max_index = left_max_index;
    if (right_max_index > max_index) max_index = right_max_index;
    return max_index;
}
void saveTree(FILE* file, Node* current_node, int max_index) {
    if (current_node == NULL) return;
    if (current_node->index <= max_index) {
        fprintf(file, "%d %s\n", current_node->index, current_node->name);
        saveTree(file, current_node->yes, max_index);
        saveTree(file, current_node->no, max_index);
    }
}
Node* loadTree(FILE* file, int index) {
    int current_index;
    char name[MAX_NAME_LENGTH];
    fseek(file, 0, SEEK_SET);
    while (fscanf(file, "%d %[^\n]", &current_index, name) != EOF) {
        if (current_index == index) {
            Node* node = newNode(name, index);
            node->yes = loadTree(file, index * 2 + 1);
            node->no = loadTree(file, index * 2);
            return node;
        }
    }
    return NULL;
}
void freeTree(Node* root) {
    if (root == NULL) return;
    freeTree(root->yes);
    freeTree(root->no);
    free(root);
}

void logAction(FILE* log_file, const char* format) {
    time_t rawtime;
    struct tm* timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", timeinfo);
    fprintf(log_file, "[%s] ", buffer);

    va_list args;
    va_start(args, format);
    vfprintf(log_file, format, args);
    va_end(args);

    fprintf(log_file, "\n");
}
