//
// Created by andrz on 3/14/2020.
//

#include "fileutils.h"
#include <string.h>

#define N 100

int main(int argc, char** args){
    switch (argc){
        case 1 || 2 || 3 || 4:
            return 0;
        case 5:
            if(strcmp(args[1], "generate") == 0){
                generate(args[2], atoi(args[3]), atoi(args[4]));
                return 0;
            }
        case 6:
            if(strcmp(args[1], "sort") == 0){
                if(strcmp(args[5], "sys") == 0){
                    sortSys(args[2], atoi(args[3]), atoi(args[4]));
                    return 0;
                }
                if(strcmp(args[5], "lib") == 0){
                    sort(args[2], atoi(args[3]), atoi(args[4]));
                    return 0;
                }
                return -1;
            }
        case 7:
            if(strcmp(args[1], "copy") == 0){
                if(strcmp(args[6], "sys") == 0){
                    copySys(args[2], args[3], atoi(args[4]), atoi(args[5]));
                    return 0;
                }
                if(strcmp(args[6], "lib") == 0){
                    copy(args[2], args[3], atoi(args[4]), atoi(args[5]));
                    return 0;
                }
                return -1;
            }
    }
}