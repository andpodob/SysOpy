//
// Created by andrz on 3/16/2020.
//

#include <time.h>
#include "findutils.h"
#include <stdio.h>

int main(int argc, char** argv){
    char* startingPoint = calloc(255, sizeof(char));
    char* atime = calloc(20, sizeof(char));
    char* ctime = calloc(20, sizeof(char));
    char* maxdepth = calloc(20, sizeof(char));

    int i = 1;
    if(argc > 1 && strcmp(argv[1], "-atime") != 0 && strcmp(argv[1], "-ctime") != 0 && strcmp(argv[1], "-maxdepth") != 0){
        strcpy(startingPoint, argv[1]);
        i++;
    }else{
        strcpy(startingPoint, ".");
    }
    for(; i < argc; i++){
        if(strcmp(argv[i], "-ctime") == 0){
            i++;
            strcpy(ctime, argv[i]);
        }
        else if(strcmp(argv[i], "-atime") == 0){
            i++;
            strcpy(atime, argv[i]);
        }
        else if(strcmp(argv[i], "-maxdepth") == 0){
            i++;
            strcpy(maxdepth, argv[i]);
        }
    }
    int flag = (strcmp(argv[argc-1], "-r") == 0)?REQ:NFTW;
        find(startingPoint, atime, ctime, maxdepth, flag);

    free(atime);
    free(ctime);
    free(maxdepth);
}