//
// Created by andrz on 3/12/2020.
//
#include "my_lib.h"

ArrayOfBlocks* createArrayOfBlocks(int length){
    //Alokowanie structa Blok
    ArrayOfBlocks* arrayOfBlocks = (ArrayOfBlocks* ) calloc(1, sizeof(ArrayOfBlocks));
    //Alokowanie tablicy wewnatrz tego structa na length elementow (tablica wskaznikow na bloki operacji)
    arrayOfBlocks->array = (OperationBlock** ) calloc(length, sizeof(OperationBlock*));
    arrayOfBlocks->length = length;
    arrayOfBlocks->index = -1;
    arrayOfBlocks->size = 0;

    return arrayOfBlocks;
}

OperationBlock* createOperationBlock(int length){
    //Struct operationBlock (tu się cos odpierdala)
    printf("1");
    OperationBlock* operationBlock = (OperationBlock*) calloc(1, sizeof(OperationBlock));
    printf("2");
    //Tablica wewnatrz tego structa (tablica wskaznikow na operacje)
    operationBlock->operations = (Operation**) calloc(length, sizeof(Operation*));
    operationBlock->length = length;
    operationBlock->index = -1;
    operationBlock->size = 0;

    return operationBlock;
}

Operation* createOperation(char* o){
    Operation* operation = (Operation*) calloc(1, sizeof(Operation));
    operation->operation = (char *) calloc(strlen(o), sizeof(char));
    //operation->operation = o;
    strcpy(operation->operation, o);


    return operation;
}

void deleteArrayOfBlocks(ArrayOfBlocks* array){
    for(int i = 0; i < array->length; i++){
        if(array->array[i] !=   NULL){
            deleteOperationBlock(array->array[i]);
        }
    }
    free(array);
}

void deleteOperationBlock(OperationBlock* block){
    for(int i = 0; i < block->length; i++){
        deleteOperation(block->operations[i]);
    }
    free(block);
}

void deleteOperation(Operation* operation){
    if(operation != NULL){
        free(operation->operation);
        free(operation);
    }
}

int addToOperationBlock(OperationBlock* operationBlock, Operation* operation){
    if(operationBlock->length == operationBlock->size) return -1;
    int index = operationBlock->index;
    while(operationBlock->operations[index%operationBlock->length] != NULL){
        index = index + 1;
    }

    operationBlock->operations[index%operationBlock->length] = operation;
    operationBlock->index = index%operationBlock->length;

    return operationBlock->index;
}

int addToArrayOfBlocks(ArrayOfBlocks* array, OperationBlock* operationBlock){
    if(array->length == array->size) return -1;
    int index = array->index;
    while (array->array[index%array->length] != NULL)
    {
        index = index +1;
    }
    array->array[index%array->length] = operationBlock;
    array->index = index%array->length;

    return array->index;
}

OperationBlock* removeOperationBlockFromArrayOfBlocks(ArrayOfBlocks* array, int index){
    if(index < 0 || index >= array->length) return NULL;
    OperationBlock* removed = array->array[index];
    array->array[index] = NULL;
    return removed;
}

Operation* removeOperationFromOperationBlock(OperationBlock* operations, int index){
    if(index < 0 || index >= operations->length) return NULL;
    Operation* removed = operations->operations[index];
    operations->operations[index] = NULL;
    return removed;
}

FileSequence* createFileSequence(int length){
    FileSequence* fileSequence = (FileSequence*) calloc(1, sizeof(FileSequence));
    fileSequence->filesPairSeq = (FilesPair**) calloc(length, sizeof(FilesPair*));
    fileSequence->length = length;
    fileSequence->index = -1;
    fileSequence->size = 0;

    return fileSequence;
}

FilesPair* createFilesPair(File* a, File* b){
    FilesPair* filesPair = (FilesPair*) calloc(1, sizeof(FilesPair));
    filesPair->a = a;
    filesPair->b = b;

    return filesPair;
}

File* createFile(char* filename){
    File* file = (File*) calloc(1, sizeof(File));
    file->filename = filename;

    return file;
}

void deleteFile(File* file){
    free(file->filename);
    free(file);
}

void deleteFilePair(FilesPair* pair){
    deleteFile(pair->a);
    deleteFile(pair->b);
    free(pair);
}

void deleteFileSequence(FileSequence* seq){
    for(int i = 0; i < seq->size; i++){
        deleteFilePair(seq->filesPairSeq[i]);
    }
    free(seq);
}

int addToFileSequence(FileSequence* fileSequnece, FilesPair* pair){
    //jezeli w tablicy blokow jest jeszcze miejsce
    if(fileSequnece->length > fileSequnece->index + 1){
        fileSequnece->index = fileSequnece->index + 1;
        fileSequnece->filesPairSeq[fileSequnece->index] = pair;
        fileSequnece->size = fileSequnece->size + 1;
        return fileSequnece->index;
    }else{
        return -1;
    }
}

void comparePairs(ArrayOfBlocks* mainArray, FileSequence* fileSeq){
    //char** blocks = calloc(200, sizeof(char*));
    char** blocks;
    Operation* operation;
    OperationBlock* block;
    int operationCount = 0;
    for(int i = 0; i < fileSeq->size; i++){
        blocks = calloc(200, sizeof(char*));
        execDiff(fileSeq->filesPairSeq[i]->a->filename, fileSeq->filesPairSeq[i]->b->filename, "output.txt");
        printf("Exec done\n");
        operationCount = parseOutputFile("output.txt", blocks);
        printf("Parsed %d\n", operationCount);
        block  = createOperationBlock(operationCount);
        printf("1*\n");
        addToArrayOfBlocks(mainArray, block);
        for(int i = 0; i < operationCount; i++){
            printf("2*\n");
            operation = createOperation(blocks[i]);
            printf("3*\n");
            addToOperationBlock(block, operation);
            printf("4*\n");
        }
        printf("Cleaning\n");
        for(int i = 0; i < 200; i++){
            free(blocks[i]);
        }
        free(blocks);
        printf("Done\n");
    }
    // printf("Cleaning\n");
    // for(int i = 0; i < 200; i++){
    //     free(blocks[i]);
    // }
    // free(blocks);
    // printf("Done\n");
}

void printStructure(ArrayOfBlocks* mainArray){
    OperationBlock* block;
    char* operation = calloc(255, sizeof(char));
    char *p, *temp;

    if(mainArray != NULL){
        for(int i = 0; i < mainArray->length; i++){
            if(mainArray->array[i] != NULL){
                printf("Blok o indeksie: %d\n", i);
                block = mainArray->array[i];
                for(int j = 0; j < block->length; j++){
                    if(block->operations[j] != NULL){
                        strcpy(operation, block->operations[j]->operation);
                        printf("      Operacja o indeksie: %d\n", j);
                        p = strtok_r(operation, "\n", &temp);
                        do {
                            printf("          %s\n", p);
                        } while ((p = strtok_r(NULL, "\n", &temp)) != NULL);
                    }
                }
            }
        }
    }
}

