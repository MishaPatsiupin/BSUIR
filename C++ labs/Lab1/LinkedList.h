#ifndef LAB1_TEST_LINKEDLIST_H
#define LAB1_TEST_LINKEDLIST_H

#include <iostream>
#include <stdexcept>
#include "matrix.h"


template <typename T>
concept MatrixClass = std::is_same_v<T, matrix>;

template <MatrixClass T>

class LinkedList {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& value) : data(value), next(nullptr) {}
    };

    Node* head;
    int size;

public:
    LinkedList();
    ~LinkedList();

    void insert(const T& value);
    void remove(const T& value);
    bool contains(const T& value) const;
    int getSize() const;
    void print() const;
};

template <typename T>
LinkedList<T>::LinkedList() : head(nullptr), size(0) {}

template <typename T>
LinkedList<T>::~LinkedList() {
    Node* current = head;
    while (current != nullptr) {
        Node* temp = current;
        current = current->next;
        delete temp;
    }
}

template <typename T>
void LinkedList<T>::insert(const T& value) {
    Node* newNode = new Node(value);
    if (head == nullptr) {
        head = newNode;
    } else {
        Node* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
    }
    size++;
}

template <typename T>
void LinkedList<T>::remove(const T& value) {
    if (head == nullptr) {
        throw std::runtime_error("List is empty");
    }

    if (head->data == value) {
        Node* temp = head;
        head = head->next;
        delete temp;
        size--;
        return;
    }

    Node* current = head;
    while (current->next != nullptr && current->next->data != value) {
        current = current->next;
    }

    if (current->next == nullptr) {
        throw std::runtime_error("Value not found in list");
    }

    Node* temp = current->next;
    current->next = current->next->next;
    delete temp;
    size--;
}

template <typename T>
bool LinkedList<T>::contains(const T& value) const {
    Node* current = head;
    while (current != nullptr) {
        if (current->data == value) {
            return true;
        }
        current = current->next;
    }
    return false;
}

template <typename T>
int LinkedList<T>::getSize() const {
    return size;
}

template <typename T>
void LinkedList<T>::print() const {
    Node* current = head;
    while (current != nullptr) {
        std::cout << current->data << " ";
        current = current->next;
    }
    std::cout << std::endl;
}



#endif //LAB1_TEST_LINKEDLIST_H
