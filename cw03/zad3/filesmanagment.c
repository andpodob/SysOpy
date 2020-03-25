//
// Created by andrz on 3/25/2020.
//

//
// Created by andrz on 3/21/2020.
//

#include <unistd.h>
#include "filesmanagment.h"


void removeWhiteSpaces(char* filename){
    FILE *f = fopen(filename, "r");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

    char *string = malloc(fsize + 1);
    fread(string, 1, fsize, f);
    fclose(f);
    string[fsize] = 0;

    f = fopen(filename, "w");
    char* token = strtok(string, " ");
    while (token != NULL){
        fwrite(token, sizeof(char),strlen(token), f);
        token = strtok(NULL, " ");
    }
}

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

void writeMatrixToTextFile(char* fileName, int** matrix, int rows, int columns, int columnsToWrite, int startingIndex){
//    int fd = open(fileName, O_CREAT | O_WRONLY, 0777);
    FILE* fd = fopen(fileName, "r+");
    if(fd == NULL){
        return;
    }
    char valueBuffer[13];
    for(int i = 0; i < rows; i++){
        fseek(fd,12*sizeof(char)*(i*columns+startingIndex),SEEK_SET);
        for(int j = startingIndex; j < startingIndex+columnsToWrite-1; j++){
            sprintf(valueBuffer, "%11d,", matrix[i][j-startingIndex]);
            fwrite(valueBuffer, sizeof(char), 12, fd);
        }
        if(startingIndex+columnsToWrite == columns){
            sprintf(valueBuffer, "%11d\n", matrix[i][columnsToWrite-1]);
            fwrite(valueBuffer, sizeof(char), 12, fd);
        }else{
            sprintf(valueBuffer, "%11d,", matrix[i][columnsToWrite-1]);
            fwrite(valueBuffer, sizeof(char), 12, fd);
        }
    }
    fclose(fd);
}

void writeRandomMatrixToFile(char* filename, int rows, int columns, int valueMin, int valueMax){
    FILE* fd = fopen(filename, "w");
    int width = abs(valueMax-valueMin);
    int val;
    char valueBuffer[12];
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns-1; j++){
            val = rand()%width+valueMin;
            sprintf(valueBuffer, "%d,", val);
            fwrite(valueBuffer, sizeof(char), strlen(valueBuffer), fd);
        }
        val = rand()%width+valueMin;
        sprintf(valueBuffer, "%d\n", val);
        fwrite(valueBuffer, sizeof(char), strlen(valueBuffer), fd);
    }
}