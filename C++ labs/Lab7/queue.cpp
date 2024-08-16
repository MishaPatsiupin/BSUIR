#include "queue.h"



queue::queue() {
    front = rear = nullptr;
}

queue_node *queue::get_last() {
    return rear;
}

void queue::enqueue(int id) {
    queue_node *new_node = new queue_node();

    new_node->id = id;
    new_node->next = nullptr;

    if (rear == nullptr) {
        front = rear = new_node;
        return;
    }

    rear->next = new_node;
    rear = new_node;
}


