#include "main_proc.h"

#include <filesystem>
#include <unordered_map>
#include <iostream>
#include <thread>
#include <chrono>

std::unordered_map<std::string, std::pair<std::string, std::string>> file_hashes;
std::unordered_map<std::string, std::pair<std::string, std::string>> new_file_hashes;
std::atomic<bool> check_interval(true);

void begin_hash() {
    file_hashes.clear();
    calculate_and_store_hashes(flagMenu.path, file_hashes); //первичное
}

// обход директории и сохранение хешей в мапу
void calculate_and_store_hashes(const std::string &directory, std::unordered_map<std::string, std::pair<std::string, std::string>> &map_hashes) {
    for (const auto &entry: std::filesystem::recursive_directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            std::string path = entry.path().string();
            std::pair<std::string, std::string> hashes = calculate_hashes(path);
            map_hashes[path] = hashes; // assign the pair of hashes
        }
    }
    BOOST_LOG_TRIVIAL(info) << "Hashes calculated and stored";
}

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

std::string calculate_md5_hash(const std::string &path) {
    std::string command = "md5sum \"" + path + "\"";
    std::string result = exec(command.c_str());
    return result.substr(0, result.find(' ')); // md5sum returns the hash followed by the filename
}

std::string calculate_sha256_hash(const std::string &path) {
    std::string command = "sha256sum \"" + path + "\"";
    std::string result = exec(command.c_str());
    return result.substr(0, result.find(' ')); // sha256sum returns the hash followed by the filename
}

std::pair<std::string, std::string> calculate_hashes(const std::string &path) {
    std::string md5_hash = calculate_md5_hash(path);
    std::string sha256_hash = calculate_sha256_hash(path);
    return std::make_pair(md5_hash, sha256_hash);
}

void check_file(const std::string &directory) {
    check_interval = true;

    std::thread([directory]() {
        sleep(5);
        while (check_interval) {
            new_file_hashes.clear();
            calculate_and_store_hashes(directory, new_file_hashes);

            for (const auto &entry : new_file_hashes) {
                if (file_hashes.find(entry.first) == file_hashes.end()) {
                    BOOST_LOG_TRIVIAL(error) << "Новый файл обнаружен: " << entry.first << "\n";
                    send_critical_urgency_notification(flagMenu.notification_channel, "Attention! New file found: " + entry.first);

                } else {
                    if (flagMenu.hash_algorithm == "MD5") {
                        if (file_hashes[entry.first].first != entry.second.first) {
                            BOOST_LOG_TRIVIAL(error) << "Несоответствие MD5 хеша для файла: " << entry.first << "\n";
                            send_critical_urgency_notification(flagMenu.notification_channel, "Attention! File: " + entry.first + " changed (hash MD5 mismatch)");
                        }
                    } else {
                        if (file_hashes[entry.first].second != entry.second.second) {
                            BOOST_LOG_TRIVIAL(error) << "Несоответствие SHA256 хеша для файла: " << entry.first << "\n";
                            send_critical_urgency_notification(flagMenu.notification_channel, "Attention! File: " + entry.first + " changed (hash SHA256 mismatch)");
                        }
                    }
                }
            }

            for (const auto &entry : file_hashes) {
                if (new_file_hashes.find(entry.first) == new_file_hashes.end()) {
                    std::cout << "Файл не найден при новом сканировании: " << entry.first << "\n";
                    send_critical_urgency_notification(flagMenu.notification_channel, "Attention! File not found: " + entry.first);
                }
            }

            std::this_thread::sleep_for(std::chrono::minutes (flagMenu.interval)); // Check every minute
        }
    }).detach();
}

void stop_check_file() {
    std::cout << "Остановка проверки файлов\n";
    send_notification(flagMenu.notification_channel, "File check stopped");
    check_interval = false;
}



