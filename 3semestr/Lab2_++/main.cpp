/*Пересечение параллелепипедов. Даны по 4 точки каждого параллелепипеда(углы).
Необходимо проверить пересечение двух объектов.*/
#include "parallelepiped.h"
#include <iostream>

int main() {
    parallelepiped box1(
            {1.0, 2.0, 3.0}, {4.0, 2.0, 3.0},
            {4.0, 5.0, 3.0}, {1.0, 5.0, 5.0});
    parallelepiped box2(
            {2.0, 3.0, 4.0}, {5.0, 3.0, 4.0},
            {5.0, 6.0, 4.0}, {2.0, 6.0, -8.0});

    // Проверяем пересечение
    if (box1 & box2) {
        std::cout << "Parallelepipeds intersect." << std::endl;
    } else {
        std::cout << "Parallelepipeds do not intersect." << std::endl;
    }

    return 0;
}




