#include "main_proc.h"

void main_proc(flag_menu_changes flagMenuChanges) {
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
            restore_backup();
            stop_check_file();
        }
        if (flagMenuChanges.path_changed){
            restore_backup();
            stop_check_file();

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


