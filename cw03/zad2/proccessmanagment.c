//
// Created by andrz on 3/21/2020.
//

#include <unistd.h>
#include <sys/wait.h>
#include "filesmanagement.h"
#include "matrixutils.h"
#include "processmanagement.h"

int N = 0;
int PROCESS_INDEX = 0;
char* LIST;

int processList(int outputType);
void createOutputFiles(char** outputFileNames, int* n);


int runProcessesOnList(char* list, int n, int type) {
    LIST = list;
    N = n;
    int outputFilesCount = 0;
    char **outputFileNames = malloc(sizeof(char *) * 100);
    createOutputFiles(outputFileNames, &outputFilesCount);
    int *childPids = malloc(n * sizeof(int));
    int *results = malloc(n * sizeof(int));
    int pid;
    for (int i = 0; i < n; i++) {
        pid = fork();
        if (pid < 0) {
            return -1;
        } else if (pid > 0) {
            childPids[i] = pid;
            PROCESS_INDEX++;
        } else {
            processList(type);
            exit(0);
        }
    }
    //processList(SINGLE);
    for (int i = 0; i < N; i++) {
        waitpid(childPids[i], &results[i], 0);
    }
    if (type == SINGLE){
        for (int i = 0; i < outputFilesCount; i++) {
            removeWhiteSpaces(outputFileNames[i]);
        }
        }
}

int processList(int type){
    char lines[100][100];
    char indexAsString[10];
    sprintf(indexAsString, "%d", PROCESS_INDEX);
    FILE* listFd = fopen(LIST, "r");

    int linesCount = 0;

    while (fgets(lines[linesCount++],1000, listFd) != NULL);

    fclose(listFd);

    linesCount--;

    char* matrixA, *matrixB, *matrixC, *matrixCwithSuffix;
    int** intMatrixA = NULL, **intMatrixB = NULL, **intMatrixC = NULL;
    int aRows, aCol, bRows, bCol, cRows, cCol, temp, startingIndex;
    for(int i = 0; i < linesCount; i++){
        matrixA = strtok(lines[i], " \r\n");
        matrixB = strtok(NULL, " \r\n");
        matrixC = strtok(NULL, " \r\n");
        matrixCwithSuffix = malloc((strlen(matrixC)+10)*sizeof(char));
        intMatrixA = loadMatrix(matrixA, &aRows, &aCol);
        intMatrixB = loadMatrix(matrixB, &bRows, &bCol);

        temp = (bCol/N > 0)? bCol/N:1;
        startingIndex = PROCESS_INDEX*temp;

        if(PROCESS_INDEX == N-1 && startingIndex < bCol)temp = bCol-startingIndex;
        if(bCol >= startingIndex+temp) {
            intMatrixC = multiplyMatrices(intMatrixA, intMatrixB, &cRows, &cCol, aRows, aCol, bRows, temp, startingIndex);
            if(type == MULTIPLE){
                strcpy(matrixCwithSuffix, matrixC);
                strcat(matrixCwithSuffix, "_");
                strcat(matrixCwithSuffix, indexAsString);
                writeMatrixToFile(matrixCwithSuffix, intMatrixC, cRows, cCol);
            } else{
                //strcat(matrixCwithSuffix, "bin");
                writeMatrixToTextFile(matrixC, intMatrixC, aRows, bCol, cCol, startingIndex);
            }
        }
        freeMatrix(intMatrixA, aRows);
        intMatrixA=NULL;
        freeMatrix(intMatrixB, bRows);
        intMatrixB=NULL;
        freeMatrix(intMatrixC, cRows);
        intMatrixC=NULL;
    }
}

void createOutputFiles(char** outputFileNames, int* n){
    FILE* listFd = fopen(LIST, "r");
    FILE* createdFile;
    char line[1000];
    char* token;
    *n=0;
    while(fgets(line, 1000, listFd) != NULL){
        strtok(line, " \r\n");
        strtok(NULL, " \r\n");
        token = strtok(NULL, " \r\n");
        createdFile = fopen(token, "w");
        fclose(createdFile);
        outputFileNames[*n] = malloc(100* sizeof(char));
        strcpy(outputFileNames[*n], token);
        (*n)++;
    }
}