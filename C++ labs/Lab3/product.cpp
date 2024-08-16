#include "product.h"
#include "sting.h"

std::ostream &operator<<(std::ostream &ostream, const product &product) {
    ostream << "\n  Product, ID: " << product.ID << "\n";
    ostream << "  1) Name: ";
    for (char i: product.name_name) {
        ostream << i;
    }
    if(product.quantity == -2){
        ostream << "\n  2) Product in development";
    }
    if(product.quantity == -1){
        ostream << "\n  2) The product is in production";
    }
    if(product.quantity == 0){
        ostream << "\n  2) Out of Stock";
    }
    if(product.quantity > 0){
        ostream << "\n  2) In stock, "<< product.quantity <<" left";
    }

    ostream << "\n  3) Developer: ";
    for (char i: product.developer) {
        ostream << i;
    }

    ostream << "\n  4) Manager: ";
    for (char i: product.manager) {
        ostream << i;
    }

    if (!product.primitives.empty()){
        ostream << "\n  Primitives: ";
        for (char i: product.primitives) {
            ostream << i;
        }
    }
    return ostream;
}

product product::create_product() {
    product product;
    product.ID = ID_inzilizete();
    char input;
    std::cin.clear();

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очистка символа новой строки
    std::cout << "\nEnter the name product: ";
    while (std::cin.get(input) && input != '\n')
        product.name_name.push_back(input);


    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очистка символа новой строки
    std::cout << "Enter quantity product: ";
    std::cin >> product.quantity;


    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очистка символа новой строки
    std::cout << "\nEnter the name developer: ";
    while (std::cin.get(input) && input != '\n')
        product.developer.push_back(input);


    std::cout << "\nEnter the name manager: ";
    while (std::cin.get(input) && input != '\n')
        product.manager.push_back(input);



    std::cout << "Enter 1 to add primitives ->";
    if (get_number(-10) == 1) {
        std::cout << "\nEnter the name primitives: ";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        while (std::cin.get(input) && input != '\n')
            product.primitives.push_back(input);
    }

    return product;
}

product product::first_product() {
    product product;
    product.ID = ID_inzilizete();

    product.name_name = {'C', 'o', 'm', 'p', ' ', 'c', 'l', 's', '2', '3', '7'};
product.quantity = 7;
    product.developer = {'L', 'y', 'k', 'a', 'n', 'o', 'v', 'a', ' ', 'I', '.', 'V', '.'};
    product.manager = {'N', 'i', 'k', ' ', 'B', '.', 'V', '.'};
    return product;
}

int product::next_id = 0;
int product::ID_inzilizete() const {
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
    return next_id + (next * 6);
}

