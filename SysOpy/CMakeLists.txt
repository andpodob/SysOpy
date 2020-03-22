# cmake_minimum_required(VERSION <specify CMake version here>)
project(SysOpy C)

set(CMAKE_C_STANDARD 99)

add_executable(SysOpy zad1/diff_lib.c zad1/my_lib.c zad1/diff_lib.h zad1/my_lib.h zad2/main.c)