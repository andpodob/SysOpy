//
// Created by andrz on 3/14/2020.
//

#ifndef CW02_FILEUTILS_H
#define CW02_FILEUTILS_H

#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

void generate(char* filename, int rows, int rowLen);
void sortSys(char* filename, int rows, int buffor_size);
void sort(char* filename, int rows, int buffor_size);
void copySys(char* src, char* dest, int rows, int buffor_size);
void copy(char* src, char* dest, int rows, int buffor_size);

#endif //CW02_FILEUTILS_H
