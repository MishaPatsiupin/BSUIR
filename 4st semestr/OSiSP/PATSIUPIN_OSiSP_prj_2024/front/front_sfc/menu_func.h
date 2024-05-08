#ifndef FRONT_SFC_MENU_FUNC_H
#define FRONT_SFC_MENU_FUNC_H

#include <boost/program_options.hpp>
#include <iostream>
#include <filesystem>

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

    std::string notification_channel;
    int flag_notification_channel = 0;
};
extern flag_menu flagMenu;

struct buffer_menu {
    bool protection;
    bool force_restore = false;
    std::string path;
    int interval;
    std::string backup_type;
    std::string hash_algorithm;
    std::string access_permissions;
    std::string notification_channel;
};
extern buffer_menu bufferMenu;

namespace po = boost::program_options;

void enable_protection();
void disable_protection();
void get_file_info();
void force_file_restore();
void show_help_info(po::options_description& desc);
void exit_program();
void set_file_path(po::variables_map& vm);
void set_file_check_interval(po::variables_map& vm);
void set_backup_type(po::variables_map& vm);
void set_hash_algorithm(po::variables_map& vm);
void set_notification_channel(po::variables_map& vm);

void file_info();
void show_help();
void show_help_menu();

void read_config_file();
void ignore_cin();

#endif //FRONT_SFC_MENU_FUNC_H
