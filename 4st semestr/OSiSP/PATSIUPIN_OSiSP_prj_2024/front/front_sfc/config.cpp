#include "config.h"

bool file_exists(const std::string& name) {
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

void update_config_file() {
    // Создание директории
    std::string home_dir = getenv("HOME");
    std::string config_dir = home_dir + "/control/config";
    std::string mkdir_command = "mkdir -p " + config_dir;
    system(mkdir_command.c_str());

    // Создание JSON объекта
    Json::Value root;

    // Проверка существования файла и чтение его, если он существует
    std::string config_file_path = config_dir + "/config.json";
    if (!file_exists(config_file_path)) {
        std::string touch_command = "touch " + config_file_path;
        system(touch_command.c_str());
    }
    if (file_exists(config_file_path)) {
        std::ifstream config_file_in(config_file_path, std::ios::in);
        if (config_file_in && config_file_in.peek() != std::ifstream::traits_type::eof()) {
            config_file_in >> root;
            config_file_in.close();
        } else{
            root["protection"] = false;
            root["path"] = home_dir;
            root["interval"] = 3;
            root["backup_type"] = "full";
            root["hash_algorithm"] = "MD5";
            root["notification_channel"] = "system";
            root["force_restore"] = false;
        }
    }
    // Добавление информации в конфигурационный файл, если флаг установлен
    if (flagMenu.flag_protection) root["protection"] = flagMenu.protection;
    if (flagMenu.flag_path) root["path"] = flagMenu.path;
    if (flagMenu.flag_interval) root["interval"] = flagMenu.interval;
    if (flagMenu.flag_backup_type) root["backup_type"] = flagMenu.backup_type;
    if (flagMenu.flag_hash_algorithm) root["hash_algorithm"] = flagMenu.hash_algorithm;
    if (flagMenu.flag_notification_channel) root["notification_channel"] = flagMenu.notification_channel;
    if (flagMenu.flag_force_restore) root["force_restore"] = flagMenu.force_restore;
    // Запись обновленного JSON объекта обратно в файл
    std::ofstream config_file_out(config_file_path, std::ios::out | std::ios::trunc);
    if (!config_file_out) {
        std::cerr << "I/O error while opening file: " << config_file_path << std::endl;
        return;
    }

    config_file_out << root;
    config_file_out.close();
}

void read_config_file(){
    std::string home_dir = getenv("HOME");
    std::string config_dir = home_dir + "/control/config/config.json";

    std::ifstream config_file(config_dir, std::ifstream::binary);
    if (!config_file.is_open()) {
        return;
    }

    Json::Value root;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse(config_file, root);
    if (!parsingSuccessful) {
        return;
    }

    bufferMenu.protection = root["protection"].asBool();
    bufferMenu.force_restore = root["force_restore"].asBool();
    bufferMenu.path = root["path"].asString();
    bufferMenu.interval = root["interval"].asInt();
    bufferMenu.backup_type = root["backup_type"].asString();
    bufferMenu.hash_algorithm = root["hash_algorithm"].asString();
    bufferMenu.notification_channel = root["notification_channel"].asString();


}