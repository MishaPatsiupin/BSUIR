#include "developer.h"
#include <thread>

int developer::next_id = 0;
int developer::ID_inzilizete() const {
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
    return next_id + (next * 3);
}

std::ostream &operator<<(std::ostream &ostream, const developer &developer) {
    ostream << "\nDeveloper, ";
    developer.print_name_id();
    return ostream;
}


developer developer::create_developer() {
    developer developer;

    developer.ID = ID_inzilizete();
    char input;

    std::cout << "\nEnter the name developer: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    while (std::cin.get(input) && input != '\n')
        developer.name_name.push_back(input);


    return developer;
}

developer developer::first_developer() {
    developer developer;

    developer.ID = ID_inzilizete();
    developer.name_name = {'G', 'l', 'e', 'c', 'e', 'v', 'i', 'c', 'h', ' ', 'I', '.', 'I', '.'};
    return developer;
}





















