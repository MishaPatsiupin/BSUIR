#ifndef LAB3_TASK_TIME_H
#define LAB3_TASK_TIME_H
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <string>
#include <windows.h>


class task_time {
public:
    int time;
    std::string task;
    int flag_work;

    task_time(); // Конструктор по умолчанию
    void new_time();
    void countdown();
    static std::shared_ptr<task_time> create_task();
};




#endif //LAB3_TASK_TIME_H
