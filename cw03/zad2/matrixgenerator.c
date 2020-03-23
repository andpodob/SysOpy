//
// Created by andrz on 3/23/2020.
//
#include "filesmanagement.h"
#include <time.h>

int main(int argc, char** argv){
    srand(time(0));
    int matrixSize;
    int maxVal = 10, minVal = -10;
    if(argc <= 2){
        return -1;
    }
    if(argc == 4){
        minVal = atoi(argv[3]);
    }
    if(argc == 5){
        minVal = atoi(argv[3]);
        maxVal = atoi(argv[4]);
    }
    if(argc >= 3){
        matrixSize = rand()%300;
        writeRandomMatrixToFile(argv[1],rand()%500, matrixSize, minVal, maxVal);
        writeRandomMatrixToFile(argv[2], matrixSize, rand()%500, minVal, maxVal);
    }


    return 0;
}
