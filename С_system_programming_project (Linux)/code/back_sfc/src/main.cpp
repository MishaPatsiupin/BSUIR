#include <iostream>
#include <thread>

#include "configuration/chek_conf_file.h"
#include "configuration/config.h"
#include "daemon/deamon_works.h"

flag_menu flagMenu;

int main() {
    setup_logger();
    BOOST_LOG_TRIVIAL(info) << "Application start";

    flagMenu.password  = generate_random_string(20);
    BOOST_LOG_TRIVIAL(info) << "Generated random password";

    daemonize();
    BOOST_LOG_TRIVIAL(info) << "Daemonized the process";

    std::string home = getenv("HOME");
    flagMenu.directory = home + "/control/";
    BOOST_LOG_TRIVIAL(info) << "Set control directory to: " << flagMenu.directory;

    std::string config_dir = flagMenu.directory + "/config/config.json";
    BOOST_LOG_TRIVIAL(info) << "Set config directory to: " << config_dir;

    BOOST_LOG_TRIVIAL(info) << "Starting config file watcher thread";
    std::thread watcher_thread(watch_config_file, config_dir);

    watcher_thread.join();
    BOOST_LOG_TRIVIAL(info) << "Config file watcher thread joined";

    BOOST_LOG_TRIVIAL(info) << "Application end";
    return 0;
}