//
// Created by andrz on 3/25/2020.
//

#include <stdlib.h>
#include "processmanagment.h"
#include <string.h>

int main(int argc, char** argv){
    if(argc == 7){
        if(strcmp(argv[3], "SINGLE") == 0)
            runProcessesOnList(argv[1], atoi(argv[2]), SINGLE, atoi(argv[4]), atoi(argv[5]),atoi(argv[6]));
        if(strcmp(argv[3], "MULTIPLE") == 0)
            runProcessesOnList(argv[1], atoi(argv[2]), MULTIPLE, atoi(argv[4]),atoi(argv[5]),atoi(argv[6]));

    }
    return 0;
}