#ifndef FRONT_SFC_CONFIG_H
#define FRONT_SFC_CONFIG_H

#include <json/json.h>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>

#include "menu.h"

void update_config_file();
bool file_exists(const std::string& name);

#endif //FRONT_SFC_CONFIG_H
