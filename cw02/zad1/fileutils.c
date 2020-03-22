//
// Created by andrz on 3/14/2020.
//

#include "fileutils.h"

typedef struct {
    int initial_idx;
    int final_idx;
    int len;
    int fd;
    FILE* file;
} Record;

extern int errno;

char* getRecordValueSys(Record* record) {
    char* buffor = malloc(record->len* sizeof(char));
    int status = lseek(record->fd, record->len*record->initial_idx, SEEK_SET);
    if(status != record->len*record->initial_idx) return NULL;

    status  = read(record->fd, buffor, record->len);

    if(status == -1) return NULL;
    return buffor;
}

char* getRecordValue(Record* record) {
    char* buffor = malloc(record->len* sizeof(char));
    int status = fseek(record->file, record->len*record->initial_idx, SEEK_SET);
    if(status != 0) return NULL;

    status  = fread(buffor, sizeof(char),record->len, record->file);

    if(status != record->len) return NULL;

    return buffor;
}

int compareRecords(Record* rec1, Record* rec2) {
    char* r1 = getRecordValue(rec1);
    char* r2 = getRecordValue(rec2);
    for(int i = 0; i < rec1->len; i++) {
        if(r1[i] > r2[i]){
            free(r1);
            free(r2);
            return 1;
        }
        if(r1[i] < r2[i]){
            free(r1);
            free(r2);
            return -1;
        }
    }

    return 0;
}

int compareRecordsSys(Record* rec1, Record* rec2) {
    char* r1 = getRecordValueSys(rec1);
    char* r2 = getRecordValueSys(rec2);
    for(int i = 0; i < rec1->len; i++) {
        if(r1[i] > r2[i]){
            free(r1);
            free(r2);
            return 1;
        }
        if(r1[i] < r2[i]){
            free(r1);
            free(r2);
            return -1;
        }
    }

    return 0;
}

void swap(Record** a, Record** b) {
    Record* t = *a;
    *a = *b;
    *b = t;
}

int partitionSys (Record **arr, int low, int high)
{
    Record* pivot = arr[high];    // pivot
    int i = (low - 1);  // Index of smaller element

    for (int j = low; j <= high- 1; j++) {
        if (compareRecordsSys(pivot, arr[j]) == 1) {
            i++;    // increment index of smaller element
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

int partition(Record **arr, int low, int high)
{
    Record* pivot = arr[high];    // pivot
    int i = (low - 1);  // Index of smaller element

    for (int j = low; j <= high- 1; j++) {
        if (compareRecords(pivot, arr[j]) == 1) {
            i++;    // increment index of smaller element
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSortSys(Record **arr, int low, int high) {
    if (low < high) {
        int pi = partitionSys(arr, low, high);
        quickSortSys(arr, low, pi - 1);
        quickSortSys(arr, pi + 1, high);
    }
}

void quickSort(Record **arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void generate(char* filename, int rows, int rowLen) {
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0777); //dopisać O_EXCE
    char row[rowLen];
    char randomChar;
    if(errno == EEXIST) {
        fprintf(stderr,"%s\n", strerror(errno));
        return;
    }else{
        for(int i = 0; i < rows; i++) {
            for (int j = 0; j < rowLen-1; j++) {
                randomChar = 48+rand()%100;
                while (randomChar == '\n') randomChar = rand() % 256;
                row[j] = randomChar;
            }
            row[rowLen-1] = '\n';
            write(fd, row, rowLen);
        }
    }
    close(fd);
}

void sortSys(char* filename, int rows, int buffor_size){
    int fd = open(filename, O_RDWR);
    if(fd != -1){
        Record* records[rows];
        Record* initRecords[rows];
        Record* temp;
        for(int i = 0; i < rows; i++){
            temp = malloc(sizeof(Record));
            temp->initial_idx = i;
            temp->len = buffor_size;
            temp->fd = fd;
            initRecords[i] = temp;
            records[i] = temp;
        }
        quickSortSys(records, 0, rows - 1);
        for(int i = 0; i < rows; i++){
            records[i]->final_idx = i;
        }
        Record* temp2;
        char* ch1, *ch2;
        for(int i = 0; i < rows; i++){
            temp = initRecords[i];
            ch1 = getRecordValueSys(temp);
            while (temp != initRecords[temp->final_idx]){
                temp2 = initRecords[temp->final_idx];
                ch2 = getRecordValueSys(initRecords[temp->final_idx]);
                lseek(fd, temp->final_idx*temp->len, SEEK_SET);
                write(fd, ch1, temp->len);
                initRecords[temp->final_idx] = temp;
                free(ch1);
                temp = temp2;
                ch1 = ch2;
            }
        }
        free(ch1);
        free(ch2);

        close(fd);
    }else{
        fprintf(stderr,"%s\n", strerror(errno));
        return;
    }
}

void sort(char* filename, int rows, int buffor_size){
    FILE* fd = fopen(filename, "r+");
    if(fd != NULL){
        Record* records[rows];
        Record* initRecords[rows];
        Record* temp;
        for(int i = 0; i < rows; i++){
            temp = malloc(sizeof(Record));
            temp->initial_idx = i;
            temp->len = buffor_size;
            temp->file = fd;
            initRecords[i] = temp;
            records[i] = temp;
        }
        quickSort(records, 0, rows - 1);
        for(int i = 0; i < rows; i++){
            records[i]->final_idx = i;
        }
        Record* temp2;
        char* ch1, *ch2;
        for(int i = 0; i < rows; i++){
            temp = initRecords[i];
            ch1 = getRecordValue(temp);
            while (temp != initRecords[temp->final_idx]){
                temp2 = initRecords[temp->final_idx];
                ch2 = getRecordValue(initRecords[temp->final_idx]);
                fseek(fd, temp->final_idx*temp->len, SEEK_SET);
                fwrite(ch1, sizeof(char), temp->len, fd);
                initRecords[temp->final_idx] = temp;
                free(ch1);
                temp = temp2;
                ch1 = ch2;
            }
        }
        free(ch1);
        free(ch2);

        fclose(fd);
    }else{
        fprintf(stderr,"%s\n", strerror(errno));
        return;
    }
}

void copySys(char* src, char* dest, int rows, int buffor_size) {
    int srcFd = open(src, O_RDONLY);
    int destFd = open(dest, O_WRONLY | O_CREAT | O_TRUNC, 0777);
    int readStatus = 0;
    int writeStatus = 0;

    char buffor[buffor_size];
    for(int i = 0; i < rows; i++) {
        readStatus = read(srcFd, buffor, buffor_size);
        if (readStatus == 0) return;
        writeStatus = write(destFd, buffor, buffor_size);
        if (readStatus == -1 || writeStatus == -1) {
            fprintf(stderr, "%s\n", strerror(errno));
            return;
        }
    }
}

void copy(char* src, char* dest, int rows, int buffor_size){
    int readStatus = 0;
    int writeStatus = 0;
    FILE* srcFile = fopen(src,"r");
    FILE* destFile = fopen(dest,"w+");
    char buffor[buffor_size];
    for(int i = 0; i < rows; i++) {
        readStatus = fread(buffor, sizeof(char), buffor_size, srcFile);
        if (readStatus != buffor_size) return;
        writeStatus = fwrite(buffor, sizeof(char), buffor_size, destFile);
        if (readStatus != buffor_size || writeStatus != buffor_size) {
            fprintf(stderr, "%s\n", strerror(errno));
            return;
        }
    }
}
