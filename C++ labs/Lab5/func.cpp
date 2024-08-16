// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "func.h"
#include <limits>

int get_choize(int min, int max) {
    int choize = -1;

    while (choize == -1) {
        std::cout << "Enter the number from " << min << " to " << max << ": ";
        std::cin >> choize;

        if (std::cin.fail() || choize < min || choize > max) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Error input. Please try again." << std::endl;
            choize = -1;
        } else {
            return choize;
        }
    }
}

int get_number(int min) {
    int number = -1;

    while (number == -1) {
        std::cin >> number;

        if (std::cin.fail() || number < min) {
            std::cin.clear();
            std::cout << "Error input. Please try again." << std::endl;
            number = -1;
        } else {
            return number;
        }
    }
}

int menu(){
    int choize = -1;
    std::cout << "\n----------Matrix Calculator--------" << std::endl;
    std::cout << "Enter 1, to create a new matrix" << std::endl; //автоматич
    std::cout << "Enter 2, to view the matrix(s)" << std::endl;
    std::cout << "Enter 3, to view or minimize an arbitrary matrix element" << std::endl; //+строка, толбец
    std::cout << "Enter 4, for transposing a matrix" << std::endl;

/*Перегрузить операторы +, -, *, +=, -=, *=, =, операторы сравнения, new и delete.*/
    std::cout << "Enter 5, for addition of matrices +" << std::endl;
    std::cout << "Enter 6, for subtraction of matrices -" << std::endl;
    std::cout << "Enter 7, for increase by N(-10<=N<=10) each element of the matrix +=" << std::endl;
    std::cout << "Enter 8, for decrease by N(-10<=N<=10) each element of the matrix -=" << std::endl;
    std::cout << "Enter 9, for multiplying a matrix by a number *=" << std::endl;
    std::cout << "Enter 10, for Matrix comparisons on the first element </>" << std::endl;

    std::cout << "Enter 0, for exit program\n" << std::endl;
    choize = get_choize(0, 10);
    return choize;
}

int menu_change_data(){
    int choize = -1;
    std::cout << "\nEnter 1, to change one element" << std::endl; //автоматич
    std::cout << "Enter 2, to add one row" << std::endl;
    std::cout << "Enter 3, to delete one row" << std::endl;
    std::cout << "Enter 4, to add one col" << std::endl;
    std::cout << "Enter 5, to delete one col" << std::endl;

    std::cout << "Enter 0, for exit function\n" << std::endl;
    choize = get_choize(0, 5);
    return choize;
}

int whot_create_matrix (int i){
    int flag = -1;

    switch (i) {
        case 0:{
            std::cout << "\nEnter 0, to create new matrix" << std::endl;
            std::cout << "Enter 1, to coppy matrix" << std::endl;
            flag = get_choize(0, 1);
            break;
        }
        case 1:{
            std::cout << "\nEnter 0, to auto create new matrix" << std::endl;
            std::cout << "Enter 1, to set matrix parameters" << std::endl;
            flag = get_choize(0, 1);
            break;
        }
        case 2:{
            std::cout << "\nEnter to number of columns:" << std::endl;
            flag = get_number(1);
            break;
        }
        case 3:{
            std::cout << "Enter to number of rows:" << std::endl;
            flag = get_number(1);
            break;
        }
        case 4:{
            std::cout << "\nEnter to number of columns: ";
            flag = get_number(1);
            break;
        }
        case 5:{
            std::cout << "\nIf you vont enter the one more rows - enter the 1, else - 0\n";
            flag = get_number(0);
            break;
        }
        case 6:{
            std::cout << "In which line do you want to change the element?, i = ";
            flag = get_number(0);
            break;
        }
        case 7:{
            std::cout << "Which column does it belong to?, j = ";
            flag = get_number(0);
            break;
        }
        case 8:{
            std::cout << "\nEnter 1, to work with 1st matrix" << std::endl;
            std::cout << "Enter 2, to work with 2st matrix" << std::endl;
            flag = get_choize(1, 2);
            break;
        }
        case 9:{
            std::cout << "\nEnter the N(-10<=N<=10)" << std::endl;
            flag = get_choize(-10, 10);
            break;
        }
    }
    return flag;
}

int random(int min, int max) {
    static bool initialized = false;
    if (!initialized) {
        srand(time(NULL));
        initialized = true;
    }
    return rand() % (max - min + 1) + min;
}

std::vector<int> transposing (std::vector<int> matrix_data, int cols){
    int rows = matrix_data.size()/cols;

    std::vector<int> transposed_matrix(cols * rows);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            transposed_matrix[j * rows + i] = matrix_data[i * cols + j];
        }
    }
    return transposed_matrix;
}

std::ofstream logFile("memory_log.txt", std::ios::app);
//std::ofstream file("example.txt", std::ios::trunc);
void* operator new(std::size_t size) {
    if (logFile.is_open()) {
        logFile << "The new operator is called to select " << size << " bytes of memory." << std::endl;
    }
    return malloc(size);
}

void operator delete(void* ptr) noexcept {
    if (logFile.is_open()) {
        logFile << "The delete statement was called to free memory at the address: " << ptr << std::endl;
    }
    free(ptr);
}






