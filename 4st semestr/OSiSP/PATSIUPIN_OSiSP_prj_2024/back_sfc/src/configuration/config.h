#ifndef BACK_CONFIG_H
#define BACK_CONFIG_H


#include <string>
#include <fstream>
#include <json/json.h>
#include <ncurses.h>
#include <fstream>
#include <iostream>
#include <random>

#include "../other/logger.h"
#include "../other/notification.h"

struct flag_menu {
    bool protection; //+
    bool force_restore;
    std::string path; //
    int interval; //+
    std::string backup_type; //+
    std::string hash_algorithm; //+
    std::string notification_channel; //

    std::string password;
    std::string directory;
};
extern flag_menu flagMenu;

struct flag_menu_changes {
    bool protection_changed = false;
    bool force_restore_changed = false;
    bool path_changed = false;
    bool interval_changed = false;
    bool backup_type_changed = false;
    bool hash_algorithm_changed = false;
    bool notification_channel_changed = false;
};

bool parse_config();
void file_info();
void main_proc(flag_menu_changes flagMenuChanges);

std::string generate_random_string(size_t length);

#endif //BACK_CONFIG_H
