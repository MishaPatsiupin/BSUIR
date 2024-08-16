#include "main_proc.h"

#include <boost/process.hpp>
#include <boost/filesystem.hpp>

void begin_backup() {
    full_backup(flagMenu.path, flagMenu.directory);
}

// Создание полного бэкапа. +
void full_backup(const std::string &source_directory, const std::string &backup_directory) {
    boost::filesystem::path backup_dir(backup_directory);
    if (!boost::filesystem::exists(backup_dir)) {
        boost::filesystem::create_directories(backup_dir);
    }

    std::string source = source_directory;
    std::string destination = backup_directory;
    std::string backup_file = destination + "backup.tar";
    std::string compressed_file = backup_file + ".gz";
    std::string encrypted_file = compressed_file + ".enc";

    //архивация директории
    std::string tar_command = "tar -cf " + backup_file + " -C " + source + " .";
    boost::process::system(tar_command);


    //сжатие
    std::string gzip_command = "gzip " + backup_file;
    boost::process::system(gzip_command);

    //шифрование
    if (boost::filesystem::exists(encrypted_file)) {
        boost::filesystem::remove(encrypted_file);
    }
    to_encrypt_file(compressed_file, flagMenu.password);

    //удаление сжатого архива
    boost::filesystem::remove(compressed_file);

    //ток чтение владельцем
    boost::filesystem::permissions(encrypted_file, boost::filesystem::perms::owner_read);

    send_notification(flagMenu.notification_channel, "Full backup created");
}

// Восстановление бэкапа
void restore_backup() {
    if (flagMenu.backup_type == "full") {
        BOOST_LOG_TRIVIAL(info) << "Performing full backup restore";
        full_restore_backup(flagMenu.path, flagMenu.directory);
    } else if (flagMenu.backup_type == "differential") {
        differential_restore_backup(flagMenu.path, flagMenu.directory);
    }
}

// Восстановление полного бэкапа. +
void full_restore_backup(const std::string &source_directory, const std::string &backup_directory) {
    BOOST_LOG_TRIVIAL(info) << "Restoring full backup";
    std::string destination = backup_directory;
    std::string encrypted_file = destination + "backup.tar.gz.enc";
    std::string decrypted_file = destination + "backup.tar.gz";
    std::string extracted_file = destination + "backup.tar";

    if (!boost::filesystem::exists(encrypted_file)) {
        BOOST_LOG_TRIVIAL(error) << "Encrypted backup file does not exist: " << encrypted_file;

    }
    boost::filesystem::permissions(encrypted_file, boost::filesystem::perms::all_all);

    to_decrypt_file(encrypted_file, flagMenu.password);

    boost::filesystem::remove(encrypted_file);

    if (!boost::filesystem::exists(decrypted_file)) {
        BOOST_LOG_TRIVIAL(error) << "Decrypted backup file does not exist: " << decrypted_file;

    }

    std::string gunzip_command = "gunzip " + decrypted_file;
    int gunzip_status = boost::process::system(gunzip_command);

    if (gunzip_status != 0) {
        BOOST_LOG_TRIVIAL(error) << "Failed to extract the backup file: " << decrypted_file;

    }

    if (!boost::filesystem::exists(source_directory)) {
        boost::filesystem::create_directories(source_directory);
    } else {
        boost::filesystem::remove_all(source_directory);
        boost::filesystem::create_directories(source_directory);
    }

    std::string tar_command = "tar -oxf " + extracted_file + " -C \"" + source_directory + "\"";
    int tar_status = boost::process::system(tar_command);

    if (tar_status != 0) {
        BOOST_LOG_TRIVIAL(error) << "Failed to extract the backup file to the source directory: " << source_directory;

    }

    boost::filesystem::remove(decrypted_file);
    boost::filesystem::remove(extracted_file);

    send_notification(flagMenu.notification_channel, "Full backup restored");
}

// Восстановление дифференциального бэкапа.
void differential_restore_backup(const std::string &source_directory, const std::string &backup_directory) {
    BOOST_LOG_TRIVIAL(error) << "differential_restore_backup start";
    std::string destination = backup_directory;
    std::string encrypted_file = destination + "backup.tar.gz.enc";
    std::string decrypted_file = destination + "backup.tar.gz";
    std::string extracted_file = destination + "backup.tar";

    // Проверка существования зашифрованного файла перед его расшифровкой
    if (!boost::filesystem::exists(encrypted_file)) {
        BOOST_LOG_TRIVIAL(error) << "Encrypted backup file does not exist: " << encrypted_file;
    }

    boost::filesystem::permissions(encrypted_file, boost::filesystem::perms::all_all);

    to_decrypt_file(encrypted_file, flagMenu.password);

    std::string gunzip_command = "gunzip " + decrypted_file;
    boost::process::system(gunzip_command);

    // Создание исходного каталога, если он не существует
    if (!boost::filesystem::exists(source_directory)) {
        boost::filesystem::create_directories(source_directory);
    }
    std::string tar_command = "tar -oxf " + extracted_file + " -C \"" + source_directory + "\"";
    boost::process::system(tar_command);

    // Удаление расшифрованных и извлеченных файлов после восстановления
    boost::filesystem::remove(decrypted_file);
    boost::filesystem::remove(extracted_file);

    send_notification(flagMenu.notification_channel, "Differential backup restored");
}

// Принудительное восстановление, если требуется
void force_restore_if_needed() {
    if (flagMenu.protection) {
        if (flagMenu.force_restore) {
            if (flagMenu.backup_type == "full") {
                force_full_restore(flagMenu.path, flagMenu.directory);
            } else if (flagMenu.backup_type == "differential") {
                force_differential_restore(flagMenu.path, flagMenu.directory);
            }
            flagMenu.force_restore = false;
        }
    } else {
        send_notification(flagMenu.notification_channel, "You cannot recover files without protection enabled");
    }
}


// Принудительное восстановление полного бэкапа
void force_full_restore(const std::string &source_directory, const std::string &backup_directory) {
    BOOST_LOG_TRIVIAL(info) << "Forcing full backup restore";
    std::string destination = backup_directory;
    std::string encrypted_file = destination + "backup.tar.gz.enc";
    std::string decrypted_file = destination + "backup.tar.gz";
    std::string extracted_file = destination + "backup.tar";

    // Проверка существования зашифрованного файла перед его расшифровкой
    if (!boost::filesystem::exists(encrypted_file)) {
        BOOST_LOG_TRIVIAL(error) << "Encrypted backup file does not exist: " << encrypted_file;
    }

    boost::filesystem::permissions(encrypted_file, boost::filesystem::perms::all_all);

    to_decrypt_file(encrypted_file, flagMenu.password);


    // Проверка существования расшифрованного файла перед его извлечением
    if (!boost::filesystem::exists(decrypted_file)) {
        BOOST_LOG_TRIVIAL(error) << "Decrypted backup file does not exist: " << decrypted_file;
    }

    std::string gunzip_command = "gunzip " + decrypted_file;
    boost::process::system(gunzip_command);

    // Создание исходного каталога, если он не существует
    if (!boost::filesystem::exists(source_directory)) {
        boost::filesystem::create_directories(source_directory);
    } else {
        // Удаление всех файлов и подкаталогов в исходном каталоге
        boost::filesystem::remove_all(source_directory);
        boost::filesystem::create_directories(source_directory);
    }

    std::string tar_command = "tar -oxf " + extracted_file + " -C \"" + source_directory + "\"";
    boost::process::system(tar_command);

    //ток чтение владельцем
    boost::filesystem::permissions(encrypted_file, boost::filesystem::perms::owner_read);

    // Удаление расшифрованных и извлеченных файлов после восстановления
    boost::filesystem::remove(decrypted_file);
    boost::filesystem::remove(extracted_file);

    send_notification(flagMenu.notification_channel, "Full backup restored");
}

// Принудительное восстановление дифференциального бэкапа
void force_differential_restore(const std::string &source_directory, const std::string &backup_directory) {
    BOOST_LOG_TRIVIAL(info) << "Forcing differential backup restore";
    std::string destination = backup_directory;
    std::string encrypted_file = destination + "backup.tar.gz.enc";
    std::string decrypted_file = destination + "backup.tar.gz";
    std::string extracted_file = destination + "backup.tar";

    // Проверка существования зашифрованного файла перед его расшифровкой
    if (!boost::filesystem::exists(encrypted_file)) {
        BOOST_LOG_TRIVIAL(error) << "Encrypted backup file does not exist: " << encrypted_file;
    }

    boost::filesystem::permissions(encrypted_file, boost::filesystem::perms::all_all);


    to_decrypt_file(encrypted_file, flagMenu.password);

    // Проверка существования расшифрованного файла перед его извлечением
    if (!boost::filesystem::exists(decrypted_file)) {
        BOOST_LOG_TRIVIAL(error) << "Decrypted backup file does not exist: " << decrypted_file;
    }

    std::string gunzip_command = "gunzip " + decrypted_file;
    boost::process::system(gunzip_command);

    // Создание исходного каталога, если он не существует
    if (!boost::filesystem::exists(source_directory)) {
        boost::filesystem::create_directories(source_directory);
    }
    std::string tar_command = "tar -oxf " + extracted_file + " -C \"" + source_directory + "\"";
    boost::process::system(tar_command);

    //ток чтение владельцем
    boost::filesystem::permissions(encrypted_file, boost::filesystem::perms::owner_read);

    // Удаление расшифрованных и извлеченных файлов после восстановления
    boost::filesystem::remove(decrypted_file);
    boost::filesystem::remove(extracted_file);

    send_notification(flagMenu.notification_channel, "Differential backup restored");
}