cmake_minimum_required(VERSION 3.27)
project(back_sfc)

set(CMAKE_CXX_STANDARD 17)

# Find packages jsoncpp, Boost, and OpenSSL
find_package(jsoncpp REQUIRED)
find_package(Boost REQUIRED COMPONENTS log_setup log filesystem regex thread date_time system chrono atomic filesystem)
find_package(OpenSSL REQUIRED)

add_executable(back_sfc src/main.cpp
        src/configuration/config.cpp
        src/configuration/config.h
        src/configuration/chek_conf_file.cpp
        src/configuration/chek_conf_file.h
        src/daemon/deamon_works.cpp
        src/daemon/deamon_works.h
        src/other/logger.h
        src/other/logger.cpp
        src/proc/main_proc.cpp
        src/proc/main_proc.h
        src/proc/backup.cpp
        src/proc/hash.cpp
        src/other/notification.cpp
        src/other/notification.h
        src/proc/hash.cpp
        src/proc/encrypted.cpp)

# Link libraries jsoncpp, Boost, and OpenSSL with your executable
target_link_libraries(back_sfc jsoncpp_lib Boost::log_setup Boost::log Boost::filesystem Boost::regex Boost::thread Boost::date_time Boost::system Boost::chrono Boost::atomic OpenSSL::Crypto OpenSSL::SSL)