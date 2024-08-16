#ifndef LAB7_QUEUE_H
#define LAB7_QUEUE_H
#include <iostream>
#include <fstream>
#include <vector>
#include "func.h"

class queue_node {
public:
    int id;
    std::vector<char> str;

    ~queue_node(){}
    queue_node* next;
};

class queue {
public:
    queue_node *front;
    queue_node *rear;
    queue();
    queue_node* get_last();
    void enqueue(int id);

};

#endif //LAB7_QUEUE_H