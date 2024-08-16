#include "config.h"

bool parse_config() {
    std::string home_dir = getenv("HOME");
    std::string config_dir = home_dir + "/control/config/config.json";

    std::ifstream config_file(config_dir, std::ifstream::binary);
    if (!config_file.is_open()) {
        BOOST_LOG_TRIVIAL(error) << "Failed to open config file";
        return false;
    }

    auto buffer_menu = flagMenu;
    flag_menu_changes changes;

    Json::Value root;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse(config_file, root);
    if (!parsingSuccessful) {
        BOOST_LOG_TRIVIAL(error) << "Failed to parse config file";
        return false;
    }

    flagMenu.protection = root["protection"].asBool();
    flagMenu.force_restore = root["force_restore"].asBool();
    flagMenu.path = root["path"].asString();
    flagMenu.interval = root["interval"].asInt();
    flagMenu.backup_type = root["backup_type"].asString();
    flagMenu.hash_algorithm = root["hash_algorithm"].asString();
    flagMenu.notification_channel = root["notification_channel"].asString();


    if (buffer_menu.protection != flagMenu.protection) {
        changes.protection_changed = true;
        BOOST_LOG_TRIVIAL(info) << "Protection: " << (flagMenu.protection ? "Enabled" : "Disabled");
    }
    if (buffer_menu.force_restore != flagMenu.force_restore) {
        changes.force_restore_changed = true;
        BOOST_LOG_TRIVIAL(info) << "Force Restore: " << (flagMenu.force_restore ? "Enabled" : "Disabled");
    }
    if (buffer_menu.path != flagMenu.path) {
        changes.path_changed = true;
        BOOST_LOG_TRIVIAL(info) << "Path: " << flagMenu.path;
    }
    if (buffer_menu.interval != flagMenu.interval) {
        changes.interval_changed = true;
        BOOST_LOG_TRIVIAL(info) << "Interval: " << flagMenu.interval;
    }
    if (buffer_menu.backup_type != flagMenu.backup_type) {
        changes.backup_type_changed = true;
        BOOST_LOG_TRIVIAL(info) << "Backup Type: " << flagMenu.backup_type;
    }
    if (buffer_menu.hash_algorithm != flagMenu.hash_algorithm) {
        changes.hash_algorithm_changed = true;
        BOOST_LOG_TRIVIAL(info) << "Hash Algorithm: " << flagMenu.hash_algorithm;
    }
    if (buffer_menu.notification_channel != flagMenu.notification_channel) {
        changes.notification_channel_changed = true;
        BOOST_LOG_TRIVIAL(info) << "Notification Channel: " << flagMenu.notification_channel;
    }
    file_info();

    if (changes.path_changed or changes.protection_changed or changes.interval_changed or changes.backup_type_changed or changes.hash_algorithm_changed or changes.notification_channel_changed) {
        send_notification(flagMenu.notification_channel, "Config file reloaded");
    }

    main_proc(changes);

    return true;
}

void file_info() {
    // Вывод информации о файлах
    BOOST_LOG_TRIVIAL(info) << "||----------------------------------------||";
    BOOST_LOG_TRIVIAL(info) << "||-------------- File Information --------||";
    BOOST_LOG_TRIVIAL(info) << "||----------------------------------------||";
    BOOST_LOG_TRIVIAL(info) << "|| Path: " << flagMenu.path;
    BOOST_LOG_TRIVIAL(info) << "|| Force Restore: " << (flagMenu.force_restore ? "Enabled" : "Disabled");

    BOOST_LOG_TRIVIAL(info) << "||----------------------------------------||";
    BOOST_LOG_TRIVIAL(info) << "|| Protection: " << (flagMenu.protection ? "Enabled" : "Disabled");
    BOOST_LOG_TRIVIAL(info) << "|| Interval: " << flagMenu.interval;
    BOOST_LOG_TRIVIAL(info) << "|| Backup Type: " << flagMenu.backup_type;
    BOOST_LOG_TRIVIAL(info) << "|| Hash Algorithm: " << flagMenu.hash_algorithm;
    BOOST_LOG_TRIVIAL(info) << "|| Notification Channel: " << flagMenu.notification_channel;
    BOOST_LOG_TRIVIAL(info) << "||----------------------------------------||";
}