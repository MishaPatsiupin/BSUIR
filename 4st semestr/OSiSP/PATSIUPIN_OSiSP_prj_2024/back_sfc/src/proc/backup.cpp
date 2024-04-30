#include "main_proc.h"
#include <sys/wait.h>
#include <unistd.h>

void begin_backup() {
    full_backup(flagMenu.path, flagMenu.directory);
}

// Создание полного бэкапа
void full_backup(const std::string &source_directory, const std::string &backup_directory) {
    std::filesystem::path backup_dir(backup_directory);
    if (!std::filesystem::exists(backup_dir)) {
        std::filesystem::create_directories(backup_dir);
    }

    std::string source = source_directory;
    std::string destination = backup_directory;
    std::string backup_file = destination + "backup.tar";
    std::string compressed_file = backup_file + ".gz";
    std::string encrypted_file = compressed_file + ".gpg";

    //архивация директории
    std::string tar_command = "tar -cf " + backup_file + " -C " + source + " .";
    system(tar_command.c_str());

    //чтение архива только владельцем
    chmod(backup_file.c_str(), 0400);

    //сжатие
    std::string gzip_command = "gzip " + backup_file;
    system(gzip_command.c_str());

    //шифрование
if (std::filesystem::exists(encrypted_file)) {
        std::filesystem::remove(encrypted_file);
    }
    std::string gpg_command = "gpg --batch --passphrase " + flagMenu.password + " -c " + compressed_file;
    system(gpg_command.c_str());

    //удаление сжатого архива
    std::filesystem::remove(compressed_file);

    //ток чтение владельцем
    chmod(encrypted_file.c_str(), S_IRUSR);
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

// Восстановление полного бэкапа
void full_restore_backup(const std::string &source_directory, const std::string &backup_directory) {
    BOOST_LOG_TRIVIAL(info) << "Restoring full backup";
    std::string destination = backup_directory;
    std::string encrypted_file = destination + "backup.tar.gz.gpg";
    std::string decrypted_file = destination + "backup.tar.gz";
    std::string extracted_file = destination + "backup.tar";

    // Проверка существования зашифрованного файла перед его расшифровкой
    if (!std::filesystem::exists(encrypted_file)) {
        BOOST_LOG_TRIVIAL(error) << "Encrypted backup file does not exist: " << encrypted_file;
    }

    int gpg_status = system(("gpg --batch --passphrase " + flagMenu.password + " -d " + encrypted_file + " > " +
                             decrypted_file).c_str());

    // Проверка успешности выполнения команды gpg
    if (WIFEXITED(gpg_status) && WEXITSTATUS(gpg_status) == 0) {
        // Удаление зашифрованного файла после его расшифровки
        std::filesystem::remove(encrypted_file);
    }

    // Проверка существования расшифрованного файла перед его извлечением
    if (!std::filesystem::exists(decrypted_file)) {
        BOOST_LOG_TRIVIAL(error) << "Decrypted backup file does not exist: " << decrypted_file;
    }

    std::string gunzip_command = "gunzip " + decrypted_file;
    system(gunzip_command.c_str());

    // Создание исходного каталога, если он не существует
    if (!std::filesystem::exists(source_directory)) {
        std::filesystem::create_directories(source_directory);
    } else {
        // Удаление всех файлов и подкаталогов в исходном каталоге
        std::filesystem::remove_all(source_directory);
        std::filesystem::create_directories(source_directory);
    }

    std::string tar_command = "tar -oxf " + extracted_file + " -C \"" + source_directory + "\"";
    system(tar_command.c_str());

    // Удаление расшифрованных и извлеченных файлов после восстановления
    std::filesystem::remove(decrypted_file);
    std::filesystem::remove(extracted_file);
}

// Восстановление дифференциального бэкапа
void differential_restore_backup(const std::string &source_directory, const std::string &backup_directory) {
    BOOST_LOG_TRIVIAL(info) << "Restoring differential backup";
    std::string destination = backup_directory;
    std::string encrypted_file = destination + "backup.tar.gz.gpg";
    std::string decrypted_file = destination + "backup.tar.gz";
    std::string extracted_file = destination + "backup.tar";

    // Проверка существования зашифрованного файла перед его расшифровкой
    if (!std::filesystem::exists(encrypted_file)) {
        BOOST_LOG_TRIVIAL(error) << "Encrypted backup file does not exist: " << encrypted_file;
    }

    system(("gpg --batch --passphrase " + flagMenu.password + " -d " + encrypted_file + " > " +
            decrypted_file).c_str());

    // Проверка существования расшифрованного файла перед его извлечением
    if (!std::filesystem::exists(decrypted_file)) {
        BOOST_LOG_TRIVIAL(error) << "Decrypted backup file does not exist: " << decrypted_file;
    }

    std::string gunzip_command = "gunzip " + decrypted_file;
    system(gunzip_command.c_str());

    // Создание исходного каталога, если он не существует
    if (!std::filesystem::exists(source_directory)) {
        std::filesystem::create_directories(source_directory);
    }

    std::string tar_command = "tar -oxf " + extracted_file + " -C \"" + source_directory + "\"";
    system(tar_command.c_str());

    // Удаление расшифрованных и извлеченных файлов после восстановления
    std::filesystem::remove(decrypted_file);
    std::filesystem::remove(extracted_file);
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
    std::string encrypted_file = destination + "backup.tar.gz.gpg";
    std::string decrypted_file = destination + "backup.tar.gz";
    std::string extracted_file = destination + "backup.tar";

    // Проверка существования зашифрованного файла перед его расшифровкой
    if (!std::filesystem::exists(encrypted_file)) {
        BOOST_LOG_TRIVIAL(error) << "Encrypted backup file does not exist: " << encrypted_file;
        send_notification(flagMenu.notification_channel, "Encrypted backup file does not exist");
        return;
    }

    // Расшифровка файла
    pid_t pid = fork();
    if (pid == 0) {
        execl("/bin/sh", "sh", "-c", ("gpg --batch --passphrase " + flagMenu.password + " -d " + encrypted_file + " > " +
                                      decrypted_file).c_str(), nullptr);
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        BOOST_LOG_TRIVIAL(error) << "Failed to fork process";
        return;
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
            BOOST_LOG_TRIVIAL(error) << "Failed to decrypt backup file: " << encrypted_file;
            return;
        }
    }

    // Проверка существования расшифрованного файла перед его извлечением
    if (!std::filesystem::exists(decrypted_file)) {
        BOOST_LOG_TRIVIAL(error) << "Decrypted backup file does not exist: " << decrypted_file;
        return;
    }

    // Извлечение файла
    pid = fork();
    if (pid == 0) {
        execl("/bin/sh", "sh", "-c", ("gunzip " + decrypted_file).c_str(), nullptr);
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        BOOST_LOG_TRIVIAL(error) << "Failed to fork process";
        return;
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
            BOOST_LOG_TRIVIAL(error) << "Failed to extract backup file: " << decrypted_file;
            return;
        }
    }

    // Создание исходного каталога, если он не существует
    if (!std::filesystem::exists(source_directory)) {
        std::filesystem::create_directories(source_directory);
    } else {
        // Удаление всех файлов и подкаталогов в исходном каталоге
        std::filesystem::remove_all(source_directory);
        std::filesystem::create_directories(source_directory);
    }

    // Восстановление бэкапа
    pid = fork();
    if (pid == 0) {
        execl("/bin/sh", "sh", "-c", ("tar -oxf " + extracted_file + " -C \"" + source_directory + "\"").c_str(), nullptr);
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        BOOST_LOG_TRIVIAL(error) << "Failed to fork process";
        return;
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
            BOOST_LOG_TRIVIAL(error) << "Failed to restore backup: " << extracted_file;
            return;
        }
    }

    // Удаление расшифрованных и извлеченных файлов после восстановления
    std::filesystem::remove(decrypted_file);
    std::filesystem::remove(extracted_file);
}

// Принудительное восстановление дифференциального бэкапа
void force_differential_restore(const std::string &source_directory, const std::string &backup_directory) {
    BOOST_LOG_TRIVIAL(info) << "Forcing differential backup restore";
    std::string destination = backup_directory;
    std::string encrypted_file = destination + "backup.tar.gz.gpg";
    std::string decrypted_file = destination + "backup.tar.gz";
    std::string extracted_file = destination + "backup.tar";

    // Проверка существования зашифрованного файла перед его расшифровкой
    if (!std::filesystem::exists(encrypted_file)) {
        BOOST_LOG_TRIVIAL(error) << "Encrypted backup file does not exist: " << encrypted_file;
        send_notification(flagMenu.notification_channel, "Encrypted backup file does not exist");
    }

    system(("gpg --batch --passphrase " + flagMenu.password + " -d " + encrypted_file + " > " +
            decrypted_file).c_str());

    // Проверка существования расшифрованного файла перед его извлечением
    if (!std::filesystem::exists(decrypted_file)) {
        BOOST_LOG_TRIVIAL(error) << "Decrypted backup file does not exist: " << decrypted_file;
    }

    std::string gunzip_command = "gunzip " + decrypted_file;
    system(gunzip_command.c_str());

    // Создание исходного каталога, если он не существует
    if (!std::filesystem::exists(source_directory)) {
        std::filesystem::create_directories(source_directory);
    }

    std::string tar_command = "tar -oxf " + extracted_file + " -C \"" + source_directory + "\"";
    system(tar_command.c_str());

    // Удаление расшифрованных и извлеченных файлов после восстановления
    std::filesystem::remove(decrypted_file);
    std::filesystem::remove(extracted_file);
}