#include "menu.h"
flag_menu flagMenu;

int menu_arg_main(int argc, char* argv[]) {
    if (argc > 1) {
        process_command_line_options(argc, argv);
    } else {
        process_menu_options();
    }
    return 0;
}

void process_command_line_options(int argc, char* argv[]) {
    po::options_description desc("Options");
    desc.add_options()
            ("enable", "Enable protection")
            ("disable", "Disable protection")
            ("file-info", "Get information about protected files")
            ("force-restore", "Force file restoration")
            ("help", "Show help")
            ("exit", "Exit the program")
            ("path", po::value<std::string>(), "Specify the file path")
            ("interval", po::value<int>(), "Specify the file check interval")
            ("backup-type", po::value<std::string>(), "Specify the backup type")
            ("hash-algorithm", po::value<std::string>(), "Specify the hashing algorithm")
            ("access-permissions", po::value<std::string>(), "Specify the access permissions")
            ("notification-channel", po::value<std::string>(), "Specify the notification channel");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("enable")) {
        flagMenu.protection = true;
        flagMenu.flag_protection = 1;
        std::cout << "Enable protection\n";
    }
    if (vm.count("disable")) {
        flagMenu.protection = false;
        flagMenu.flag_protection = 1;
        std::cout << "Disable protection\n";
    }
    if (vm.count("file-info")) {
        std::cout << "Get information about protected files\n";
        file_info();
    }
    if (vm.count("force-restore")) {
        std::cout << "Force file restoration\n";
        flagMenu.force_restore = true;
        flagMenu.flag_force_restore = 1;
        force_restore();
    }
    if (vm.count("help")) {
        std::cout << desc << std::endl;
    }
    if (vm.count("exit")) {
        std::cout << "Exit the program\n";
        std::exit(0);
    }

    if (vm.count("path")) {
        flagMenu.path = vm["path"].as<std::string>();
        flagMenu.flag_path = 1;
        std::cout << "Path: " << flagMenu.path << "\n";
    }
    if (vm.count("interval")) {
        flagMenu.interval = vm["interval"].as<int>();
        flagMenu.flag_interval = 1;
        std::cout << "Interval: " << flagMenu.interval << "hours \n";
    }
    if (vm.count("backup-type")) {
        flagMenu.backup_type = vm["backup-type"].as<std::string>();
        flagMenu.flag_backup_type = 1;
        std::cout << "Backup Type: " << flagMenu.backup_type << "\n";
    }
    if (vm.count("hash-algorithm")) {
        flagMenu.hash_algorithm = vm["hash-algorithm"].as<std::string>();
        flagMenu.flag_hash_algorithm = 1;
        std::cout << "Hash Algorithm: " << flagMenu.hash_algorithm << "\n";
    }
    if (vm.count("access-permissions")) {
        flagMenu.access_permissions = vm["access-permissions"].as<std::string>();
        flagMenu.flag_access_permissions = 1;
        std::cout << "Access Permissions: " << flagMenu.access_permissions << "\n";
    }
    if (vm.count("notification-channel")) {
        flagMenu.notification_channel = vm["notification-channel"].as<std::string>();
        flagMenu.flag_notification_channel = 1;
        std::cout << "Notification Channel: " << flagMenu.notification_channel << "\n";
    }
}

void process_menu_options() {
    int choice;
    do {
        system("clear");
        std::cout << "====================================\n";
        std::cout << "|| 1. Enable protection           ||\n";
        std::cout << "|| 2. Disable protection          ||\n";
        std::cout << "|| 3. Get information about files ||\n";
        std::cout << "|| 4. Force file restoration      ||\n";
        std::cout << "|| 5. Configure protection        ||\n";
        std::cout << "|| 6. Show help                   ||\n";
        std::cout << "|| 7. Exit the program            ||\n";
        std::cout << "====================================\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1:
                flagMenu.protection = true;
                flagMenu.flag_protection = 1;
                std::cout << "Enable protection\n";
                break;
            case 2:
                flagMenu.protection = false;
                flagMenu.flag_protection = 1;
                std::cout << "Disable protection\n";
                break;
            case 3:
                std::cout << "Get information about protected files\n";
                file_info();
                break;
            case 4:
                std::cout << "Force file restoration\n";
                flagMenu.force_restore = true;
                flagMenu.flag_force_restore = 1;
                force_restore();
                break;
            case 5:
                configure_protection();
                break;
            case 6:
                system("clear");
                show_help();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
                getchar();
                break;
            case 7:
                std::cout << "Exit the program\n";
                update_config_file();
                std::exit(0);
                break;
            default:
                std::cout << "Invalid choice. Please enter a number between 1 and 7.\n";
                break;
        }
    } while (choice != 7);
}

void configure_protection() {
    int config_choice;
    do {
        system("clear");
        std::cout << "====================================\n";
        std::cout << "|| 1. Specify the file path        ||\n";
        std::cout << "|| 2. Specify the file check interval\n";
        std::cout << "|| 3. Specify the backup type      ||\n";
        std::cout << "|| 4. Specify the hashing algorithm||\n";
        std::cout << "|| 5. Specify the access permissions\n";
        std::cout << "|| 6. Specify the notification channel\n";
        std::cout << "|| 7. Return to main menu          ||\n";
        std::cout << "====================================\n";
        std::cout << "Enter your choice: ";
        std::cin >> config_choice;

        // Проверка на ввод
        if (std::cin.fail()) {
            std::cin.clear(); // очистка состояния ошибки
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // пропуск некорректного ввода
            continue; // возврат к началу цикла
        }

        switch (config_choice) {
            case 1:
                std::cout << "Enter the file path: ";
                std::cin >> flagMenu.path;
                flagMenu.flag_path = 1;
                break;
            case 2:
                std::cout << "Enter the file check interval: ";
                std::cin >> flagMenu.interval;
                flagMenu.flag_interval = 1;
                break;
            case 3:
                std::cout << "Enter the backup type: ";
                std::cin >> flagMenu.backup_type;
                flagMenu.flag_backup_type = 1;
                break;
            case 4:
                std::cout << "Enter the hashing algorithm: ";
                std::cin >> flagMenu.hash_algorithm;
                flagMenu.flag_hash_algorithm = 1;
                break;
            case 5:
                std::cout << "Enter the access permissions: ";
                std::cin >> flagMenu.access_permissions;
                flagMenu.flag_access_permissions = 1;
                break;
            case 6:
                std::cout << "Enter the notification channel: ";
                std::cin >> flagMenu.notification_channel;
                flagMenu.flag_notification_channel = 1;
                break;
            case 7:
                break;
            default:
                std::cout << "Invalid choice. Please enter a number between 1 and 7.\n";
                break;
        }
    } while (config_choice != 7);
}

void file_info() {
    // Вывод информации о файлах
    std::cout << "File information:" << std::endl;
    std::cout << "Path: " << flagMenu.path << std::endl;

    // Вывод состояний из структуры flagMenu
    std::cout << "Protection: " << (flagMenu.protection ? "Enabled" : "Disabled") << std::endl;
    std::cout << "Interval: " << flagMenu.interval << std::endl;
    std::cout << "Backup Type: " << flagMenu.backup_type << std::endl;
    std::cout << "Hash Algorithm: " << flagMenu.hash_algorithm << std::endl;
    std::cout << "Access Permissions: " << flagMenu.access_permissions << std::endl;
    std::cout << "Notification Channel: " << flagMenu.notification_channel << std::endl;
}

void show_help() {
    std::cout << "Help Information:\n";
    std::cout << "1. Enable protection: This will enable the file protection.\n";
    std::cout << "2. Disable protection: This will disable the file protection.\n";
    std::cout << "3. Get information about protected files: This will display information about the protected files.\n";
    std::cout << "4. Force file restoration: This will force the restoration of the protected files.\n";
    std::cout << "5. Update files: This will update the protected files.\n";
    std::cout << "6. Configure Protection: This will open the protection configuration menu.\n";
    std::cout << "7. Show help: This will display this help information.\n";
    std::cout << "8. Exit the program: This will exit the program.\n";
}

void force_restore(){

}
