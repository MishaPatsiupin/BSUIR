#include "func.h"

int get_choize(int min, int max) {
    int choize = -1;

    while (choize == -1) {
        std::cout << "Enter the number from " << min << " to " << max << ": ";
        std::cin >> choize;

        if (std::cin.fail() || choize < min || choize > max) {
            std::cin.clear();
            std::cout << "Error input. Please try again." << std::endl;
            choize = -1;
        } else {
            std::cout << std::endl;
            return choize;
        }
    }
    std::cout << std::endl;
}


int get_number(int min) {
    int number = -1;

    while (number == -1) {
        if (!(std::cin >> number) || number < min) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Error input. Please try again." << std::endl;
        } else {
            std::cout << std::endl;
            return number;
        }
    }
    std::cout << std::endl;
    return -99;
}
