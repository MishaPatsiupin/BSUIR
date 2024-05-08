#ifndef FRONT_SFC_MENU_H
#define FRONT_SFC_MENU_H

#include <cstdlib>
#include <libconfig.h>

#include "config.h"
#include "menu_func.h"




int menu_arg_main(int argc, char* argv[]);
void configure_protection();
void process_command_line_options(int argc, char* argv[]);
void process_menu_options();




#endif //FRONT_SFC_MENU_H
