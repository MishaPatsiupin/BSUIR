cmake_minimum_required(VERSION 3.28)
project(back)

set(CMAKE_CXX_STANDARD 17)

# Создаем исполняемый файл
add_executable(back main.cpp procces.h procces.cpp)

# Подключаем библиотеки
target_link_libraries(back setupapi powrprof)
