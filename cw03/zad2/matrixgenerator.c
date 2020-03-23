//
// Created by andrz on 3/23/2020.
//
#include "filesmanagement.h"

int main(int argc, char** argv){
    int matrixSize;
    int maxVal = 100, minVal = -100;
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
        matrixSize = rand()%10;
        writeRandomMatrixToFile(argv[1],rand()%10, matrixSize, minVal, maxVal);
        writeRandomMatrixToFile(argv[2], matrixSize, rand()%10, minVal, maxVal);
    }


    return 0;
}
