cmake_minimum_required(VERSION 3.26)
project(Lab3)

set(CMAKE_CXX_STANDARD 23)

add_executable(Lab3 main.cpp
        product.cpp
        product.h
        task_time.cpp
        task_time.h
        sting.cpp
        sting.h
        manager.cpp
        manager.h
        worker.cpp
        worker.h
        director.cpp
        director.h
        tester.cpp
        tester.h
        developer.cpp
        developer.h
        administrator.cpp
        administrator.h
        name.cpp
        name.h
)
