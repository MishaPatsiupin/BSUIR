#ifndef BACK_MAIN_PROC_H
#define BACK_MAIN_PROC_H

#include "../configuration/config.h"

#include <filesystem>
#include <cstdlib>
#include <iostream>
#include <sys/stat.h>
#include <atomic>

void to_decrypt_file(const std::string& filename, const std::string& password);
void to_encrypt_file(const std::string& filename, const std::string& password);

void begin_backup();
void restore_backup();
void full_backup(const std::string& source_directory, const std::string& backup_directory);
void full_restore_backup(const std::string& source_directory, const std::string& backup_directory);
void differential_restore_backup(const std::string& source_directory, const std::string& backup_directory);

void force_restore_if_needed();
void force_full_restore(const std::string& source_directory, const std::string& backup_directory);
void force_differential_restore(const std::string& source_directory, const std::string& backup_directory);



extern std::unordered_map<std::string, std::pair<std::string, std::string>> file_hashes;
extern std::unordered_map<std::string, std::pair<std::string, std::string>> new_file_hashes;

void begin_hash();
void calculate_and_store_hashes(const std::string &directory, std::unordered_map<std::string, std::pair<std::string, std::string>> &map_hashes);
std::pair<std::string, std::string> calculate_hashes(const std::string& path);
std::string calculate_md5_hash(const std::string& path);
std::string calculate_sha256_hash(const std::string& path);


extern std::atomic<bool> check_interval;

void check_file(const std::string &directory);
void stop_check_file();




#endif //BACK_MAIN_PROC_H
