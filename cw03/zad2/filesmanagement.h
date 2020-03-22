//
// Created by andrz on 3/21/2020.
//

#ifndef CW03_FILESMANAGEMENT_H
#define CW03_FILESMANAGEMENT_H

#include <bits/types/FILE.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int** loadMatrix(char* filename, int* rows, int* columns);
void convertBinToTextFile(char* binFile, char* textFile);
void writeMatrixToFile(char* fileName, int** matrix, int rows, int columns);
void writeMatrixToBinaryFile(char* fileName, int** matrix, int rows, int columns, int columnsToWrite, int startingIndex);


#endif //CW03_FILESMANAGEMENT_H
