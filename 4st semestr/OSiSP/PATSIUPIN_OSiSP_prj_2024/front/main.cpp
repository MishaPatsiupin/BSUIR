#include <iostream>
#include "menu.h"

/** Тэкс, недочетов дофига, фрон на уровне дерьма (плинтуса)
* Что может пойти не так:
 * -сохраняю не то что надо
* - безопасность при использованиии файла конфигурации
 * - не может создать директорию в HOME
 *
 * Что работает:
 * - нуууу эээ чет то то да
 * */

 int main(int argc, char* argv[]) {

        int h;
        h = menu_arg_main(argc, argv);
    std::cout << "Hello, World!" << std::endl;
    return 0;
}