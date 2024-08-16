#include <iostream>
#include "text_editing.h"


int main() {
    text_editing hello(0);

    hello.change_file_name();
    hello.change_window_size();


    hello.div_file();
    hello.save_to_file();

    std::string command = "C:\\Windows\\System32\\notepad.exe D:/OAiP_3/Lab7/cmake-build-debug/output.txt";
    system(command.c_str());

hello.change();


    return 0;
}

