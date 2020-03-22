cmake_minimum_required(VERSION 3.10)
project(cw03 C)

set(CMAKE_C_STANDARD 99)

add_executable(cw03 zad2/macierz.c zad2/processmanagement.h zad2/filesmanagement.h zad2/proccessmanagment.c zad2/matrixutils.h zad2/filesmanagement.c zad2/matrixutils.c)