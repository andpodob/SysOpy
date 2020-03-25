//
// Created by andrz on 3/25/2020.
//

//
// Created by andrz on 3/21/2020.
//

#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h>
#include "filesmanagment.h"
#include "matrixutils.h"
#include "processmanagment.h"
#include <sys/resource.h>

int N = 0;
int PROCESS_INDEX = 0;

char* LIST;
time_t start;
int TIME_PER_PROCESS = 0;

int processList(int outputType);
void createOutputFiles(char** outputFileNames, int* n);
char** createArgList(char* fileNameBase);
void runCmd(char** cmd, char* outputFile);
int minResult(int* tab, int n);

void setLimits(int seconds, int vmemory){
    struct rlimit timeLimit;
    struct rlimit memoryLimit;
    timeLimit.rlim_cur = seconds;
    timeLimit.rlim_max = seconds;
    memoryLimit.rlim_cur = vmemory;
    memoryLimit.rlim_max = vmemory;
    setrlimit(RLIMIT_CPU, &timeLimit);
    //setrlimit(RLIMIT_AS, &memoryLimit);
}

void printUsage(struct rusage* usage){
    printf("%s %ld %s\n","used", usage->ru_utime.tv_usec, "user time");
    printf("%s %ld %s\n","used", usage->ru_stime.tv_usec, "system time");
}

int runProcessesOnList(char* list, int n, int type, int timePerProcess, int cpuLimit, int memLimit) {
    TIME_PER_PROCESS = timePerProcess;
    LIST = list;
    N = n;
    FILE* exists;
    int outputFilesCount = 0;
    char** cmd;
    char **outputFileNames = malloc(sizeof(char *) * 100);
    createOutputFiles(outputFileNames, &outputFilesCount);
    int *childPids = malloc(n * sizeof(int));
    int *results = malloc(n * sizeof(int));
    int pid;
    start = time(NULL);
    for (int i = 0; i < n; i++) {
        pid = fork();
        if (pid < 0) {
            return -1;
        } else if (pid > 0) {
            childPids[i] = pid;
            PROCESS_INDEX++;
        } else {
            setLimits(cpuLimit, memLimit);
            processList(type);
        }
    }
    //processList(SINGLE);
    int result;
    struct rusage usage;
    for (int i = 0; i < N; i++) {
        pid = wait3(&result, 0, &usage);
        printf("Process with PID: %d, solved: %d matrices\n",pid, WEXITSTATUS(result));
        printUsage(&usage);
    }

    if (type == SINGLE){
        for (int i = 0; i < outputFilesCount; i++) {
            removeWhiteSpaces(outputFileNames[i]);
        }
    }
    if(type == MULTIPLE){
        int m = minResult(results, N);
        for(int i = 0; i < m; i++){
            cmd = createArgList(outputFileNames[i]);
            runCmd(cmd, outputFileNames[i]);
            for(int i = 0; i < N; i++){
                exists = fopen(cmd[i+2], "r");
                if(exists != NULL) {
                    fclose(exists);
                    remove(cmd[i+2]);
                }
            }
            free(outputFileNames[i]);
        }
    }
}

int processList(int type){
    time_t stop;
    time_t timeElapsed;
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
        stop = time(NULL);
        timeElapsed = (stop-start);
        if(timeElapsed > TIME_PER_PROCESS){
            exit(i);
        }
        matrixA = strtok(lines[i], " \r\n");
        matrixB = strtok(NULL, " \r\n");
        matrixC = strtok(NULL, " \r\n");
        matrixCwithSuffix = malloc((strlen(matrixC)+10)*sizeof(char));
        strcpy(matrixCwithSuffix, matrixC);
        strcat(matrixCwithSuffix, "_");
        strcat(matrixCwithSuffix, indexAsString);

        intMatrixA = loadMatrix(matrixA, &aRows, &aCol);
        intMatrixB = loadMatrix(matrixB, &bRows, &bCol);

        temp = (bCol/N > 0)? bCol/N:1;
        startingIndex = PROCESS_INDEX*temp;

        if(PROCESS_INDEX == N-1 && startingIndex < bCol)temp = bCol-startingIndex;
        if(bCol >= startingIndex+temp) {
            intMatrixC = multiplyMatrices(intMatrixA, intMatrixB, &cRows, &cCol, aRows, aCol, bRows, temp, startingIndex);
            if(type == MULTIPLE){
                writeMatrixToFile(matrixCwithSuffix, intMatrixC, cRows, cCol);
            } else{
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
    exit(linesCount);
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

char** createArgList(char* fileNameBase){
    char buffer[100];
    char indexAsString[10];
    FILE* exists;
    char** cmd = malloc(200*sizeof(char*));
    for(int i = 0; i < 200; i++){
        cmd[i] = malloc(200*sizeof(char));
    }
    strcpy(cmd[0],"paste");
    strcpy(cmd[1], "-d,");
    int j = 2;
    for(int i = 0; i < N; i++){
        sprintf(indexAsString, "%d", i);
        strcpy(buffer, fileNameBase);
        strcat(buffer, "_");
        strcat(buffer, indexAsString);
        exists = fopen(buffer, "r");
        if(exists != NULL) {
            strcpy(cmd[j++], buffer);
            fclose(exists);
        }
    }
    cmd[j] = (char*)NULL;

    return cmd;
}

void runCmd(char** cmd, char* fileName){
    int child_status;
    int pid = fork();
    if(pid == 0){
        int fd = open(fileName, O_RDWR | O_CREAT, 0777);
        dup2(fd, 1);
        close(fd);
        execv("/usr/bin/paste", cmd);
    } else {
        wait(&child_status);
    }
}

int minResult(int* tab, int n){
    int min = WEXITSTATUS(tab[0]);
    for(int i = 0; i < n; i++){
        if(WEXITSTATUS(tab[i]) < min) min = WEXITSTATUS(tab[i]);
    }
    return min;
}