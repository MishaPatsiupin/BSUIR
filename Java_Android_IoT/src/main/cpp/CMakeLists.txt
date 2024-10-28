cmake_minimum_required(VERSION 3.4.1)

add_library(moisture_data SHARED moisture_data.c)

find_library(log-lib log)

target_link_libraries(moisture_data ${log-lib})