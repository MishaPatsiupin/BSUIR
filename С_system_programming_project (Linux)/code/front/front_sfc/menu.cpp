#include "menu.h"

flag_menu flagMenu;
buffer_menu bufferMenu;

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
            ("interval", po::value<int>(), "Specify the time file check interval (1, 2, ...180 , minute)")
            ("backup-type", po::value<std::string>(), "Specify the backup type (full/differential")
            ("hash-algorithm", po::value<std::string>(), "Specify the hashing algorithm (MD5/SHA256)")
            ("notification-channel", po::value<std::string>(), "Specify the notification channel (no/system)");

    po::variables_map vm;
    try {
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("enable")) {
        enable_protection();
    } else if (vm.count("disable")) {
        disable_protection();
    } else if (vm.count("file-info")) {
        get_file_info();
    } else if (vm.count("force-restore")) {
        force_file_restore();
    } else if (vm.count("help")) {
        show_help_info(desc);
    } else if (vm.count("exit")) {
        exit_program();
    } else if (vm.count("path")) {
        set_file_path(vm);
    } else if (vm.count("interval")) {
        set_file_check_interval(vm);
    } else if (vm.count("backup-type")) {
        set_backup_type(vm);
    } else if (vm.count("hash-algorithm")) {
        set_hash_algorithm(vm);
    } else if (vm.count("notification-channel")) {
        set_notification_channel(vm);
    } else {
        std::cout << "Unrecognized command. Use --help to see available commands.\n";
    }

        update_config_file();

    } catch (boost::program_options::unknown_option& e) {
        std::cout << "Error: " << e.what() << "\n";
        std::cout << "Use --help to see available commands.\n";
        return;
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
                enable_protection();
                break;
            case 2:
                disable_protection();
                break;
            case 3:
                get_file_info();
                break;
            case 4:
                force_file_restore();
                break;
            case 5:
                configure_protection();
                break;
            case 6:
                show_help_menu();
                break;
            case 7:
                std::cout << "Exit the program\n";
                update_config_file();
                std::exit(0);
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
        std::cout << "=======================================\n";
        std::cout << "|| 1. Specify the file path           ||\n";
        std::cout << "|| 2. Specify the file check interval ||\n";
        std::cout << "|| 3. Specify the backup type         ||\n";
        std::cout << "|| 4. Specify the hashing algorithm   ||\n";
        std::cout << "|| 5. Specify the notification channel||\n";
        std::cout << "|| 6. Return to main menu             ||\n";
        std::cout << "=======================================\n";
        std::cout << "Enter your choice: ";
        std::cin >> config_choice;

        // Проверка на ввод
        if (std::cin.fail()) {
            std::cin.clear(); // очистка состояния ошибки
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // пропуск некорректного ввода
            continue; // возврат к началу цикла
        }

        switch (config_choice) {
            case 1: {
                std::cout << "Enter the file path: ";
                std::string buffer;
                std::cin >> buffer;
                if (std::filesystem::exists(buffer)) {
                    flagMenu.path = buffer;
                    flagMenu.flag_path = 1;
                } else {
                    std::cout << "The file or directory does not exist.\n";
                }
                ignore_cin();
                break;
            }
            case 2: {
                std::cout << "Enter the file check interval, 1 <= {} <= 180 minute: ";
                int buffer;
                std::cin >> buffer;
                if (buffer >= 1 && buffer <= 180) {
                    flagMenu.interval = buffer;
                    flagMenu.flag_interval = 1;
                } else {
                    std::cout << "Invalid interval. Please enter a number between 1 and 180.\n";
                }
                ignore_cin();
                break;
            }
            case 3: {
                std::cout << "Enter the backup type (full/differential): ";
                std::string buffer;
                std::cin >> buffer;
                if (buffer == "full" || buffer == "differential") {
                    flagMenu.backup_type = buffer;
                    flagMenu.flag_backup_type = 1;
                } else {
                    std::cout << "Invalid input. Please enter 'full' or 'differential'.\n";
                }
                ignore_cin();
                break;
            }
            case 4:{
                std::cout << "Enter the hashing algorithm (MD5/SHA256): ";
                std::string buffer;
                std::cin >> buffer;
                if (buffer == "MD5" || buffer == "SHA256") {
                    flagMenu.hash_algorithm = buffer;
                    flagMenu.flag_hash_algorithm = 1;
                } else {
                    std::cout << "Invalid input. Please enter 'MD5' or 'SHA256'.\n";
                }
                ignore_cin();
                break;
            }
            case 5:{
                std::cout << "Enter the notification channel (no/system): ";
                std::string buffer;
                std::cin >> buffer;
                if (buffer == "no" || buffer == "system") {
                    flagMenu.notification_channel = buffer;
                    flagMenu.flag_notification_channel = 1;
                } else {
                    std::cout << "Invalid input. Please enter 'no' or 'system'.\n";
                }
                ignore_cin();
                break;
            }
            case 6:
                break;
            default:
                std::cout << "Invalid choice. Please enter a number between 1 and 6.\n";
                ignore_cin();
                break;
        }
    } while (config_choice != 6);
}




