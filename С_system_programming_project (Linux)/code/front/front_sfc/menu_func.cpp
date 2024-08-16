#include "menu_func.h"

void enable_protection() {
    flagMenu.protection = true;
    flagMenu.flag_protection = 1;
    std::cout << "Enable protection \n";
    ignore_cin();
}

void disable_protection() {
    flagMenu.protection = false;
    flagMenu.flag_protection = 1;
    std::cout << "Disable protection \n";
    ignore_cin();
}

void get_file_info() {
    std::cout << "Get information about protected files \n";
    file_info();
}

void force_file_restore() {
    std::cout << "Force file restoration.\n";
    flagMenu.force_restore = true;
    flagMenu.flag_force_restore = 1;
    ignore_cin();
}

void show_help_info(po::options_description& desc) {
    std::cout << desc << std::endl;
    ignore_cin();
}

void exit_program() {
    std::cout << "Exit the program\n";
    std::exit(0);
}

void set_file_path(po::variables_map& vm) {
    std::string buffer = vm["path"].as<std::string>();
    if (std::filesystem::exists(buffer)) {
        flagMenu.path = buffer;
        flagMenu.flag_path = 1;
        std::cout << "Path: " << flagMenu.path << "\n";
    } else {
        std::cout << "The file or directory does not exist.\n";
    }
    ignore_cin();
}

void set_file_check_interval(po::variables_map& vm) {
    int buffer = vm["interval"].as<int>();
    if (buffer >= 1 && buffer <= 180) {
        flagMenu.interval = buffer;
        flagMenu.flag_interval = 1;
        std::cout << "Interval: " << flagMenu.interval << " minute \n";
    } else {
        std::cout << "Invalid interval. Please enter a number between 1 and 180.\n";
    }
    ignore_cin();
}

void set_backup_type(po::variables_map& vm) {
    std::string buffer = vm["backup-type"].as<std::string>();
    if (buffer == "full" || buffer == "differential") {
        flagMenu.backup_type = buffer;
        flagMenu.flag_backup_type = 1;
        std::cout << "Backup Type: " << flagMenu.backup_type << "\n";
    } else {
        std::cout << "Invalid input. Please enter 'full' or 'differential'.\n";
    }
    ignore_cin();
}

void set_hash_algorithm(po::variables_map& vm) {
    std::string buffer = vm["hash-algorithm"].as<std::string>();
    if (buffer == "MD5" || buffer == "SHA256") {
        flagMenu.hash_algorithm = buffer;
        flagMenu.flag_hash_algorithm = 1;
        std::cout << "Hash Algorithm: " << flagMenu.hash_algorithm << "\n";
    } else {
        std::cout << "Invalid input. Please enter 'MD5' or 'SHA256'.\n";
    }
    ignore_cin();
}

void set_notification_channel(po::variables_map& vm) {
    std::string buffer = vm["notification-channel"].as<std::string>();
    if (buffer == "no" || buffer == "system") {
        flagMenu.notification_channel = buffer;
        flagMenu.flag_notification_channel = 1;
        std::cout << "Notification Channel: " << flagMenu.notification_channel << "\n";
    } else {
        std::cout << "Invalid input. Please enter 'no' or 'system'.\n";
    }
    ignore_cin();
}

void show_help_menu() {
    system("clear");
    show_help();
    ignore_cin();
}



void file_info() {
    read_config_file();
    // Вывод информации о файлах
std::cout << "\n************************************\n";
std::cout << "* File information:                *\n";
std::cout << "* Path: " << bufferMenu.path << std::endl;

// Вывод состояний из структуры flagMenu
std::cout << "* Protection: " << (bufferMenu.protection ? "Enabled" : "Disabled") << std::endl;
std::cout << "* Interval: " << bufferMenu.interval << std::endl;
std::cout << "* Backup Type: " << bufferMenu.backup_type << std::endl;
std::cout << "* Hash Algorithm: " << bufferMenu.hash_algorithm << std::endl;
std::cout << "* Notification Channel: " << bufferMenu.notification_channel << std::endl;
std::cout << "************************************\n";

    ignore_cin();
}

void show_help() {

    std::cout << "*****************************************************************************************\n";
    std::cout << "*                                 Help Information:                                     *\n";
    std::cout << "* Enable protection: This will enable the file protection.                              *\n";
    std::cout << "* Disable protection: This will disable the file protection.                            *\n";
    std::cout << "* Get information about protected files: This will display information about the        *\n";
    std::cout << "*                                        protected files.                               *\n";
    std::cout << "* Force file restoration: This will force the restoration of the protected files.       *\n";
    std::cout << "* Configure protection:                                                                 *\n";
    std::cout << "** Specify the file path: Specify the path to the file or directory to be protected.    *\n";
    std::cout << "** Specify the file check interval: Specify the interval in minute (1-180) at which the *\n";
    std::cout << "**                                  file or directory should be checked.                *\n";
    std::cout << "** Specify the backup type: The type of backup to be performed (full/differential).     *\n";
    std::cout << "** Specify the hashing algorithm: Specify the hashing algorithm to be used (MD5/SHA256).*\n";
    std::cout << "** Specify the notification channel: Specify the channel through which notifications    *\n";
    std::cout << "**                                   should be sent (no/system).                        *\n";
    std::cout << "*****************************************************************************************\n";
    ignore_cin();
}


void ignore_cin(){
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    getchar();
}