//
// Created by andrz on 3/16/2020.
//

#ifndef CW02_FINDUTILS_H
#define CW02_FINDUTILS_H

#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define REQ 1
#define NFTW 2

#define LL long long int
#define MASK 0x0FFFFFFFFFFFFFFF
#define PLUS_MASK (LL)4
#define MINUS_MASK (LL)2
#define EQ_MASK (LL)3
#define ZERO_MASK (LL)1
#define SHIFT 60

int find(char* staringPoint, char* atime, char* ctime, char* maxDepth, int flag);

void printStat(struct stat* statBuff);

#endif //CW02_FINDUTILS_H
