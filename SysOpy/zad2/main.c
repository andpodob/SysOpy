//
// Created by andrz on 3/12/2020.
//

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

#include "../zad1/my_lib.h"
#include "../zad1/diff_lib.h"
#include <time.h>
#include <unistd.h>
#include <sys/times.h>
#include <stdio.h>

clock_t st_time, en_time;
struct tms st_cpu, en_cpu;
FILE *report_file;

void start_timer()
{
    st_time = times(&st_cpu);
}

void end_timer()
{
    en_time = times(&en_cpu);
}

void write_file_header(FILE *f)
{
    fprintf(f, "%15s:\t\t%15s\t%15s\t%15s\n",
            "Name",
            "Real [s]",
            "User [s]",
            "System [s]");
}

void save_timer(char *name, FILE *f)
{
    end_timer();
    int clk_tics = sysconf(_SC_CLK_TCK);
    double real_time = (double)(en_time - st_time) / clk_tics;
    double user_time = (double)(en_cpu.tms_utime - st_cpu.tms_utime) / clk_tics;
    double system_time = (double)(en_cpu.tms_stime - st_cpu.tms_stime) / clk_tics;
    fprintf(f, "%15s:\t\t%15f\t%15f\t%15f\n",
            name,
            real_time,
            user_time,
            system_time);
}

int main(int argc, char *argv[]){
//    char test[] ="ct 12\nstart\ncp a.txt:b.txt f.txt:c.txt f.txt:b.txt a.txt:b.txt a.txt:b.txt a.txt:f.txt a.txt:b.txt a.txt:b.txt a.txt:b.txt a.txt:b.txt a.txt:b.txt a.txt:b.txt\nstop";
//    char * curLine = test;
    FILE* test;
    char* name;
    if(argc != 1){
        if(argc <= 2) return -1;
        test = fopen("test.txt", "r");
        name = argv[2];
        report_file = fopen("raport.txt", "a");
        if(argc == 4) {
            write_file_header(report_file);
        }
    }
    ArrayOfBlocks* mainArray = NULL;
    OperationBlock* operationBlock =  NULL;
    char command[255];
    char* token;
    int arg1, arg2;
    char* filenameA, *filenameB;
    File* fileA, *fileB;
    FileSequence* fileSequence;
    FilesPair* filePair;
    while (fgets(command, 100, test) != NULL)
    {
        token = strtok(command," ");
        if(token != NULL){
            if(strcmp(token, "ct") == 0){
                token = strtok(NULL, " ");
                arg1 = atoi(token);
                if(mainArray != NULL){
                    deleteArrayOfBlocks(mainArray);
                }
                mainArray = createArrayOfBlocks(arg1);
            }
            else if(strcmp(token, "cp") == 0){
                filenameA = strtok(NULL, " :");
                filenameB = strtok(NULL, " :");
                fileSequence = createFileSequence(mainArray->length);
                while (filenameB != NULL)
                {
                    if(filenameB[strlen(filenameB) -1] == '\n'){
                        filenameB[strlen(filenameB) -1] = 0;
                    }
                    fileA = createFile(filenameA);
                    fileB = createFile(filenameB);
                    filePair = createFilesPair(fileA, fileB);
                    addToFileSequence(fileSequence, filePair);
                    filenameA = strtok(NULL, " :");
                    filenameB = strtok(NULL, " :");
                }
                comparePairs(mainArray, fileSequence);
            }
            else if(strcmp(token, "rb") == 0){
                token = strtok(NULL, " ");
                arg1 = atoi(token);
                removeOperationBlockFromArrayOfBlocks(mainArray, arg1);
            }
            else if(strcmp(token, "ro") == 0){
                token = strtok(NULL, " ");
                arg1 = atoi(token);
                token = strtok(NULL, " ");
                arg2 = atoi(token);
                if(arg1 >= 0 && arg1 < mainArray->length){
                    operationBlock = mainArray->array[arg1];
                }
                if(operationBlock != NULL){
                    removeOperationFromOperationBlock(operationBlock, arg2);
                }
            }
            else if(strcmp(token, "start\n") == 0){
                start_timer();
            }
            else if(strcmp(token, "stop\n") == 0){
                save_timer(name, report_file);
            }
            else if(strcmp(token, "ps\n") == 0){
                printStructure(mainArray);
            }
            else if(strcmp(token, "q\n") == 0){
                return 0;
            }
        }
    }

    fclose(test);
    return 0;
}