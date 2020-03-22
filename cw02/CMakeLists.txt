# cmake_minimum_required(VERSION <specify CMake version here>)
project(cw02 C)

set(CMAKE_C_STANDARD 99)

add_executable(cw02 zad2/find.c zad2/findutils.h zad2/findutils.c)

#add_executable(cw02 zad1/fileutils.h zad1/fileutils.c zad1/main.c zad2/find.c)