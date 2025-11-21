#include "../include/timer.h"
#include <immintrin.h>

void Timer::start() {
    start_time = std::chrono::high_resolution_clock::now();
    start_cycles = __rdtsc();
}

void Timer::stop(uint64_t& milliseconds, uint64_t& cycles) {
    auto end_time = std::chrono::high_resolution_clock::now();
    uint64_t end_cycles = __rdtsc();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        end_time - start_time);
    
    milliseconds = duration.count();
    cycles = end_cycles - start_cycles;
}