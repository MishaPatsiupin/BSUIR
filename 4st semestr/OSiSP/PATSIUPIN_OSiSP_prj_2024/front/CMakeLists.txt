cmake_minimum_required(VERSION 3.27)
project(course_03)

set(CMAKE_CXX_STANDARD 17)

# Find Boost and the program_options component
find_package(Boost REQUIRED COMPONENTS program_options)

# Use pkg-config to find Jsoncpp
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)

# Include Boost and Jsoncpp headers
include_directories(${Boost_INCLUDE_DIRS} ${JSONCPP_INCLUDE_DIRS})

# Add your executable target
add_executable(course_03 main.cpp menu.cpp menu.h
        config.cpp
        config.h)

# Link your executable target with Boost.Program_options and Jsoncpp
target_link_libraries(course_03 ${Boost_LIBRARIES} ${JSONCPP_LIBRARIES})