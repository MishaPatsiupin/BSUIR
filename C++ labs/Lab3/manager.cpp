#include "manager.h"
#include <thread>

manager manager::create_manager() {
    manager manager;

    manager.ID = ID_inzilizete();
    char input;

    std::cout << "\nEnter the name manager: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    while (std::cin.get(input) && input != '\n')
        manager.name_name.push_back(input);


    return manager;
}

manager manager::first_manager() {
    manager manager;
    manager.ID = ID_inzilizete();
        manager.name_name = {'K', 'o', 'v', 'a', 'l', 'c', 'h', 'u', 'k', ' ', 'A', '.', 'M', '.'};
    return manager;
}

std::ostream &operator<<(std::ostream &ostream, const manager &manager) {
    ostream << "\nManager, ";
    manager.print_name_id();
    return ostream;
}

int manager::next_id = 0;
int manager::ID_inzilizete() const {
    next_id++;
    int next = next_id;
    int count = 0;
    while (next != 0){
        next = next /=10;
        count++;
    }
    next = 1;
    while (count != 0){
        next *= 10;
        count--;
    }
    return next_id + (next * 5);
}



