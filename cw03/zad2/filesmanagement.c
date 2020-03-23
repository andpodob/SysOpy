//
// Created by andrz on 3/21/2020.
//

#include <fcntl.h>
#include <unistd.h>
#include "filesmanagement.h"

int** loadMatrix(char* filename, int* rows, int* columns){
    char* buffer;
    char lines[500][1000];
    char* token;
    int** intMatrix;
    int intLineBuffer[500];

    FILE* matrix = fopen(filename, "r");

    int i = 0;
    int j = 0;
    while (fgets(lines[i++],1000, matrix) != NULL){
        if(strcmp(lines[i-1], "\n") == 0 || strcmp(lines[i-1],"\r\n") == 0) {
            break;
        }
    }

    fclose(matrix);

    *rows = i-1;
    *columns = 0;

    intMatrix = malloc((*rows)* sizeof(int*));
    for(i = 0; i < *rows; i++){
        j = 0;
        token = strtok(lines[i], ",");
        while(token != NULL && j < 500){
            intLineBuffer[j] = atoi(token);
            token = strtok(NULL, ",");
            j++;
        }
        intMatrix[i] = malloc(j*sizeof(int));
        for(int k = 0; k < j; k++){
            intMatrix[i][k] = intLineBuffer[k];
        }
        *columns = (*columns > 0)? *columns:j;
    }

    free(buffer);

    return intMatrix;
}

void writeMatrixToFile(char* fileName, int** matrix, int rows, int columns){
    FILE* fd = fopen(fileName, "w");
    char valueBuffer[20];
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns-1; j++){
            sprintf(valueBuffer, "%d,", matrix[i][j]);
            fwrite(valueBuffer, sizeof(char), strlen(valueBuffer), fd);
        }
        sprintf(valueBuffer, "%d\n", matrix[i][columns-1]);
        fwrite(valueBuffer, sizeof(char), strlen(valueBuffer), fd);
    }
    fclose(fd);
}

void writeMatrixToBinaryFile(char* fileName, int** matrix, int rows, int columns, int columnsToWrite, int startingIndex){
    int fd = open(fileName, O_CREAT | O_WRONLY, 0777);
    if(fd == -1){
        return;
    }
    char valueBuffer[31];
    for(int i = 0; i < rows; i++){
        lseek(fd,21*sizeof(char)*(i*columns+startingIndex),SEEK_SET);
        for(int j = startingIndex; j < startingIndex+columnsToWrite-1; j++){
            sprintf(valueBuffer, "%20d,", matrix[i][j-startingIndex]);
            write(fd,valueBuffer, 21*sizeof(char));
        }
        if(startingIndex+columnsToWrite == columns){
            sprintf(valueBuffer, "%20d\n", matrix[i][columnsToWrite-1]);
            write(fd,valueBuffer, 21*sizeof(char));
        }else{
            sprintf(valueBuffer, "%20d,", matrix[i][columnsToWrite-1]);
            write(fd,valueBuffer, 21*sizeof(char));
        }
    }
}

void convertBinToTextFile(char* binFile, char* textFile){
    int src = open(binFile, O_RDONLY);

}