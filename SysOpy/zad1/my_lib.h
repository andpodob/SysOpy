//
// Created by andrz on 3/12/2020.
//

#ifndef SYSOPY_MY_LIB_H
#define SYSOPY_MY_LIB_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "./diff_lib.h"

typedef struct {
    //const char* dir;
    char* filename;
} File;

typedef struct {
    File* a;
    File* b;
} FilesPair;

typedef struct {
    char* operation;
} Operation;

typedef struct {
    int index;
    int length;
    int size;

    Operation** operations;
} OperationBlock;

typedef struct {
    int index;
    int length;
    int size;
    OperationBlock** array;
} ArrayOfBlocks;

typedef struct {
    int size;
    int length;
    int index;
    FilesPair** filesPairSeq;
} FileSequence;


ArrayOfBlocks* createArrayOfBlocks(int length);
OperationBlock* createOperationBlock(int length);
Operation* createOperation(char* operation);

void deleteArrayOfBlocks(ArrayOfBlocks* array);
void deleteOperationBlock(OperationBlock* block);
void deleteOperation(Operation* operation);

int addToArrayOfBlocks(ArrayOfBlocks* array, OperationBlock* operationBlock);
int addToOperationBlock(OperationBlock* operationBlock, Operation* operation);
OperationBlock* removeOperationBlockFromArrayOfBlocks(ArrayOfBlocks* array, int index);
Operation* removeOperationFromOperationBlock(OperationBlock* operations, int index);

FileSequence* createFileSequence(int length);
FilesPair* createFilesPair(File* a, File* b);
File* createFile(char* filename);


void deleteFile(File* file);
void deleteFilePair(FilesPair* pair);
void deleteFileSequence(FileSequence* seq);
int addToFileSequence(FileSequence* fileSequnece, FilesPair* pair);


void comparePairs(ArrayOfBlocks* mainArray, FileSequence* fileSeq);

void printStructure(ArrayOfBlocks* mainArray);
#endif //SYSOPY_MY_LIB_H
