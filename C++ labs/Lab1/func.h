#ifndef LAB1_TEST_FUNC_H
#define LAB1_TEST_FUNC_H
// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include <vector>
#include <iostream>
#include <ctime>
#include <fstream>

int get_choize(int min, int max);
int menu();
int menu_change_data();
int whot_create_matrix (int i);
int get_number(int min);
int random(int min, int max);
extern std::ofstream logFile;
std::vector<int> transposing(std::vector<int> matrix_data, int cols);


#endif //LAB1_TEST_FUNC_H
