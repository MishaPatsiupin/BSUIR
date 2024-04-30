#include <iostream>
#include <thread>


#include "configuration/chek_conf_file.h"
#include "configuration/config.h"
#include "daemon/deamon_works.h"


flag_menu flagMenu;


int main() {
    setup_logger();
    BOOST_LOG_TRIVIAL(info) << "Start";

    flagMenu.password  = generate_random_string(20);

 //   daemonize();
    BOOST_LOG_TRIVIAL(info) << "Daemonize";

    std::string home = getenv("HOME");
    flagMenu.directory = home + "/control/";
    std::string config_dir = flagMenu.directory + "/config/config.json";
    std::thread watcher_thread(watch_config_file, config_dir);
    BOOST_LOG_TRIVIAL(info) << "Watcher";


    watcher_thread.join();
    return 0;
}
