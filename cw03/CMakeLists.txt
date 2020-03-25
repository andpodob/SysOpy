cmake_minimum_required(VERSION 3.10)
project(cw03 C)

set(CMAKE_C_STANDARD 99)

add_executable(cw03 zad3/macierz.c zad3/filesmanagment.c zad3/filesmanagment.h zad3/matrixutils.c zad3/matrixutils.h zad3/processmanagment.h zad3/processmanagment.c)
