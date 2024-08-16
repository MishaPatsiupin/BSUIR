#include <boost/filesystem.hpp>
#include "main_proc.h"

void main_proc(flag_menu_changes flagMenuChanges) {
    std::string encrypted_file = flagMenu.directory + "backup.tar.gz.enc";
    BOOST_LOG_TRIVIAL(info) << "start main_proc";
    if (flagMenuChanges.force_restore_changed){
        if (flagMenu.force_restore){
            force_restore_if_needed();
        }
    }
    if (flagMenuChanges.protection_changed or flagMenuChanges.path_changed){
        if (flagMenu.protection){
            begin_hash();
            begin_backup();
            check_file(flagMenu.path);
        } else{
            if (boost::filesystem::exists(encrypted_file)) {
                restore_backup();
                stop_check_file();
            }
        }
        if (flagMenuChanges.path_changed){
            if (boost::filesystem::exists(encrypted_file)){
                restore_backup();
                stop_check_file();
                sleep(1);
            }
            begin_hash();
            begin_backup();
            check_file(flagMenu.path);
        }
    }
}


std::string generate_random_string(size_t length) {
    auto randchar = []() -> char {
        const char charset[] =
                "0123456789"
                "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[rand() % max_index];
    };
    std::string str(length, 0);
    std::generate_n(str.begin(), length, randchar);
    return str;
}


