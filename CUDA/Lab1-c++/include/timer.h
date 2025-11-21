#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <cstdint>

// Простая структура для измерения времени
struct Timer {
    std::chrono::high_resolution_clock::time_point start_time;
    uint64_t start_cycles;
    
    void start();
    void stop(uint64_t& milliseconds, uint64_t& cycles);
};

#endif