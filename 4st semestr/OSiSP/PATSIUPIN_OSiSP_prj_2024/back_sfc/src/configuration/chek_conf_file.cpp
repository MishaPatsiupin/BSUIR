#include "chek_conf_file.h"

#include <chrono>
#include <thread>
#include <sys/stat.h>


void watch_config_file(const std::string& config_path) {
    BOOST_LOG_TRIVIAL(info) << "start watch_config_file";

    int fd = inotify_init();
    if (fd < 0) {
        BOOST_LOG_TRIVIAL(error) << "Failed to initialize inotify";
    }



    int wd = inotify_add_watch(fd, config_path.c_str(), IN_MODIFY);
    while (wd < 0) {
        BOOST_LOG_TRIVIAL(error) << "Failed to add watch for " + config_path;
        BOOST_LOG_TRIVIAL(info) << "Retrying in 3 seconds...";
        std::this_thread::sleep_for(std::chrono::seconds(3)); // Задержка на 3 секунды
        wd = inotify_add_watch(fd, config_path.c_str(), IN_MODIFY);
    }

    char buffer[sizeof(struct inotify_event) + NAME_MAX + 1];

    // Parse the config file and execute main_proc at the start
    if (parse_config()) {
        BOOST_LOG_TRIVIAL(info) << "Config file first loaded";
    }

    time_t last_modification_time = 0;
    while (true) {
        ssize_t len = read(fd, buffer, sizeof(buffer));
        if (len < 0) {
            BOOST_LOG_TRIVIAL(error) << "Failed to read inotify events";
            break;
        }

        auto* event = reinterpret_cast<struct inotify_event*>(buffer);
        if (event->mask & IN_MODIFY) {
            struct stat attr;
            stat(config_path.c_str(), &attr);
            time_t new_modification_time = attr.st_mtime;

            if (new_modification_time != last_modification_time) {
                last_modification_time = new_modification_time;

                BOOST_LOG_TRIVIAL(error) << "Config file modified, reloading...";
                std::this_thread::sleep_for(std::chrono::seconds(1)); // Задержка на 1 секунду

                if (parse_config()) {
                    BOOST_LOG_TRIVIAL(info) << "Config file reloaded, mabye";

                } else {
                    BOOST_LOG_TRIVIAL(error) << "Failed to parse config file";
                }
            }
        }
    }

    inotify_rm_watch(fd, wd);
    close(fd);
}