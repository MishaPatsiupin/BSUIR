#include "director.h"


director director::create_director() {
    director director;

    director.ID = ID_inzilizete();
    char input;

    std::cout << "\nEnter the name director: ";
    while (std::cin.get(input) && input != '\n')
        director.name_name.push_back(input);


    return director;
}

std::ostream &operator<<(std::ostream &ostream, const director &director) {
    ostream << "\nDirector, ";
    director.print_name_id();
    return ostream;
}

director director::first(){
    director first;
    first.name_name = {'L', 'U', 'T', 'S', 'I', 'K', ' ', 'Y', 'r', 'i', 'i'};
first.ID = ID_inzilizete();
    return first;
}

int director::next_id = 0;
int director::ID_inzilizete() const {
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
    return next_id + (next * 4);
}


















