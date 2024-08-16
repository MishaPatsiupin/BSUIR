#include "tester.h"
#include <thread>

std::ostream &operator<<(std::ostream &ostream, const tester &tester) {

    ostream << "\nTester, ";
    tester.print_name_id();
    return ostream;
}

tester tester::create_tester() {
    tester tester;

    tester.ID = ID_inzilizete();
    char input;

    std::cout << "\nEnter the name tester: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    while (std::cin.get(input) && input != '\n')
        tester.name_name.push_back(input);


    return tester;
}

tester tester::first_tester() {
    tester tester;

    tester.ID = ID_inzilizete();
    tester.name_name = {'P', 'r', 'i', 'l', 'o', 'v', 's', 'k', 'i', ' ', 'B', '.', 'B', '.'};
    return tester;
}

int tester::next_id = 0;
int tester::ID_inzilizete() const {
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
    return next_id + (next * 7);
}
