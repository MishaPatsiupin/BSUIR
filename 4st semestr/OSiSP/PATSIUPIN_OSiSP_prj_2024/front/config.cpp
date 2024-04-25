#include "config.h"

void update_config_file() {
    // Создание директории
    std::string home_dir = getenv("HOME");
    std::string config_dir = home_dir + "/control/config";
    mkdir(config_dir.c_str(), 0777);

    // Создание JSON объекта
    Json::Value root;

    // Проверка существования файла и чтение его, если он существует
    std::string config_file_path = config_dir + "/config.json";
    std::ifstream config_file_in(config_file_path, std::ios::in);
    if (config_file_in) {
        config_file_in >> root;
        config_file_in.close();
    }

    // Добавление информации в конфигурационный файл, если флаг установлен
    if (flagMenu.flag_protection) root["protection"] = flagMenu.protection;
    if (flagMenu.flag_path) root["path"] = flagMenu.path;
    if (flagMenu.flag_interval) root["interval"] = flagMenu.interval;
    if (flagMenu.flag_backup_type) root["backup_type"] = flagMenu.backup_type;
    if (flagMenu.flag_hash_algorithm) root["hash_algorithm"] = flagMenu.hash_algorithm;
    if (flagMenu.flag_access_permissions) root["access_permissions"] = flagMenu.access_permissions;
    if (flagMenu.flag_notification_channel) root["notification_channel"] = flagMenu.notification_channel;

    // Запись обновленного JSON объекта обратно в файл
    std::ofstream config_file_out(config_file_path, std::ios::out | std::ios::trunc);
    if (!config_file_out) {
        std::cerr << "I/O error while opening file: " << config_file_path << std::endl;
        return;
    }

    config_file_out << root;
    config_file_out.close();
}