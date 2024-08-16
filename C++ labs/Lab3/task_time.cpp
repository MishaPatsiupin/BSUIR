#include "task_time.h"


task_time::task_time() {
    flag_work = 0;
}

void task_time::new_time() {
    task.clear();
    time = 0;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Enter the task: ";
    std::getline(std::cin, task);

    std::cout << "Enter the time to complete the task (sec): ";
    std::cin >> time;

    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    flag_work++;
}

void task_time::countdown() {
    std::this_thread::sleep_for(std::chrono::seconds(time));

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 10);
    std::cout << "\nYour task '" << task << "' completed\n";
    SetConsoleTextAttribute(hConsole, 7);
    flag_work--;
}

std::shared_ptr<task_time> task_time::create_task() {
    std::shared_ptr<task_time> task1 = std::make_shared<task_time>();
    task1->new_time();
    std::thread t1(&task_time::countdown, task1);
    t1.detach();
    return task1;
}






