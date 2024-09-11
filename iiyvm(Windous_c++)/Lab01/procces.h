#ifndef BACK_PROCCES_H
#define BACK_PROCCES_H

#include <iostream>
#include <windows.h>
#include <thread>
#include <chrono>
#include <string>
#include <vector>
#include <powrprof.h>
#include <atomic>
#include <sstream>


std::string GetChemistryInfo();
void hibernation();
void shutdown();
void displayPowerInfo();

#endif //BACK_PROCCES_H