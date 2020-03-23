//
// Created by andrz on 3/23/2020.
//

#include <stdlib.h>
#include "processmanagement.h"
#include <string.h>

int main(int argc, char** argv){
    if(argc == 4){
        if(strcmp(argv[3], "SINGLE") == 0)
            runProcessesOnList(argv[1], atoi(argv[2]), SINGLE);
    }

}