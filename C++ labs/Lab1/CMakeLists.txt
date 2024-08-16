cmake_minimum_required(VERSION 3.26)
project(Lab1_test)

set(CMAKE_CXX_STANDARD 17)

add_executable(Lab1_test main.cpp
        matrix.cpp
        matrix.h
        func.cpp
        func.h
        LinkedList.cpp
        LinkedList.h
)
