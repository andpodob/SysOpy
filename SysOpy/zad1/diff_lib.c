#include "diff_lib.h"


void execDiff(const char* fileA, const char* fileB, const char* outputFile);

int parseOutputFile(char* outputFile, char** blocks){
    FILE* fp;
    fp = fopen(outputFile, "r");
    char line[1000];
    char** lines = (char**)malloc(1000*sizeof(char*));
    for(int i = 0; i < 1000; i++){
        lines[i] = (char*)malloc(1000*sizeof(char));
    }
    int lineCount = 0;
    int lineLen = 0;
    int blockLen = 0;
    int blockIdx = 0;
    while(fgets(line, 1000, fp)){
        if(lineCount > 0 && line[0] >=48 && line[0] <= 57){
            blocks[blockIdx] = realloc(blocks[blockIdx], blockLen*sizeof(char));
            blockLen = 0;
            blockIdx = blockIdx+1;
        }
        lineLen = strlen(line);
        blockLen = blockLen+lineLen;
        strcpy(lines[lineCount], line);
        lineCount = lineCount+1;
    }
    blocks[blockIdx] = realloc(blocks[blockIdx], blockLen*sizeof(char));
    blockLen = 0;
    blockIdx = blockIdx+1;

    blockIdx = -1;

    for(int i = 0; i < lineCount; i++){
        if(lines[i][0] >= 48 && lines[i][0] <= 57){
            blockIdx = blockIdx + 1;
        }
        strcat(blocks[blockIdx], lines[i]);
    }

    for(int i = 0; i < 1000; i++) free(lines[i]);
    free(lines);
    fclose(fp);
    // char* command = calloc(50, sizeof(char));
    // strcat(command, "rm ");
    // strcat(command, outputFile);
    // system(command);
    // free(command);

    return blockIdx+1;
}

void execDiff(const char* fileA, const char* fileB, const char* outputFile){
    int commandLen = strlen(fileA)+strlen(fileB)+strlen(outputFile)+strlen("diff   > ");
    char* command = calloc(commandLen, sizeof(char));
    strcat(command, "diff ");
    strcat(command, fileA);
    strcat(command, " ");
    strcat(command, fileB);
    strcat(command," > ");
    strcat(command, outputFile);
    system(command);
}

// int main(){
//     // char** blocks = calloc(255, sizeof(char*));
//     // int blocksCount = parseOutputFile("/home/andrzej/SysOpy/Zestaw1/output.txt", blocks);
//     // blocks = realloc(blocks, (blocksCount)*sizeof(char*));
//     // for(int i = 0; i < blocksCount; i++){
//     //     printf("%s", blocks[i]);
//     // }

//     execDiff("~/Temp/a.txt", "~/Temp/b.txt", "output.txt");
//     char** blocks = calloc(255, sizeof(char*));
//     int blocksCount = parseOutputFile("output.txt", blocks);
//     blocks = realloc(blocks, (blocksCount)*sizeof(char*));
//     for(int i = 0; i < blocksCount; i++){
//         printf("%s", blocks[i]);
//     }

//     return 0;
// }
//
// Created by andrz on 3/12/2020.
//

