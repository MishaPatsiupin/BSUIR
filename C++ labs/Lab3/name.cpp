#include "name.h"

std::ostream &operator<<(std::ostream &ostream, const name &name) {
    name.print_name_id();
    return ostream;
}

void name::print_name_id() const{
        std::cout << "ID: " << ID << "\n";
        std::cout << "Name: ";
        for (char i : name_name) {
            std::cout << i;
        }
        std::cout << std::endl;
}





















