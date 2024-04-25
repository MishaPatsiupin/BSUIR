#ifndef COURSE_03_MENU_H
#define COURSE_03_MENU_H

#include <cstdlib>
#include <iostream>
#include <boost/program_options.hpp>
#include <libconfig.h>

#include "config.h"

struct flag_menu {
    bool protection;
    int flag_protection = 0;

    bool force_restore = false;
    int flag_force_restore = 0;

    std::string path;
    int flag_path = 0;

    int interval;
    int flag_interval = 0;

    std::string backup_type;
    int flag_backup_type = 0;

    std::string hash_algorithm;
    int flag_hash_algorithm = 0;

    std::string access_permissions;
    int flag_access_permissions = 0;

    std::string notification_channel;
    int flag_notification_channel = 0;
};
extern flag_menu flagMenu;
namespace po = boost::program_options;



int menu_arg_main(int argc, char* argv[]);
void file_info();
void force_restore();
void show_help();
void configure_protection();
void process_command_line_options(int argc, char* argv[]);
void process_menu_options();




#endif //COURSE_03_MENU_H
