#include <iostream>
#include "menu.h"

/** Тэкс
* Что может пойти не так:
* - безопасность при использованиии файла конфигурации
* - не может создать директорию в HOME
* - немного стремный дизайн
*
* Что работает:
* - проверочки, выводы всякие, сохранение - все норм
* */

 int main(int argc, char* argv[]) {
        int help_me;
        help_me = menu_arg_main(argc, argv);
    return 0;
}