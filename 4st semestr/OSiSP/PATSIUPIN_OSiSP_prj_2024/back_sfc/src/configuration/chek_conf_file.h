#ifndef BACK_CHEK_CONF_FILE_H
#define BACK_CHEK_CONF_FILE_H

#include <sys/inotify.h>
#include <unistd.h>
#include <iostream>
#include <climits>

#include "config.h"


void watch_config_file(const std::string& config_path);





#endif //BACK_CHEK_CONF_FILE_H
