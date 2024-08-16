#include "worker.h"
#include <thread>

worker worker::create_worker() {

    worker worker;
    worker.ID = ID_inzilizete();
    char input;

    std::cout << "\nEnter the name worker: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    while (std::cin.get(input) && input != '\n')
        worker.name_name.push_back(input);

    return worker;
}

std::ostream &operator<<(std::ostream &ostream, const worker &worker) {
    ostream << "\nWorker, ";
    worker.print_name_id();
    return ostream;
}

worker worker::first_worker() {

    worker worker;
    worker.ID = ID_inzilizete();
    worker.name_name = {'W', 'o', 'r', 'k', 'e', 'r', '1'};
    return worker;
}
int worker::next_id = 0;
int worker::ID_inzilizete() const {
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
    return next_id + (next * 8);
}
