#include "functions.h"

int main() {
    void (*func[3])() = {task1, task2, task3};
    int choice = -1;
    printMenu();
    while (choice != 0) {
        choice = getChoice();
        if (choice != 0) {
            func[choice - 1]();
        }
    }
    return 0;
}
