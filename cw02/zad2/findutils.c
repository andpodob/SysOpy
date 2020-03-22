//
// Created by andrz on 3/16/2020.
//

#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>
#include <time.h>
#include <stdbool.h>
#include "findutils.h"
#define __USE_XOPEN_EXTENDED 1
#include <ftw.h>

int MAX_LEVEL;
LL ATIME;
LL CTIME;


int findRequ(int maxDepth);

void findNftw();

int find(char* staringPoint, char* atime, char* ctime, char* maxDepth, int flag){
    int status = chdir(staringPoint);
    if(status == -1){
        fprintf(stderr,"%s\n", strerror(errno));
        return -1;
    }
    int atimeInt = (strlen(atime) > 0) ? abs(atoi(atime)):0;
    int ctimeInt = (strlen(ctime) > 0) ? abs(atoi(ctime)):0;
    atimeInt = (atimeInt > 0) ? atimeInt: 0;
    ctimeInt = (ctimeInt > 0) ? ctimeInt : 0;
    int maxDepthExtracted = (strlen(maxDepth) > 0)?abs(atoi(maxDepth)):INT_MAX;


    time_t now = time(NULL);

    now = now + (60*60);
    now = now/(24*60*60);
    now = now*(24*60*60);

    long long int atimeWithSign = now - 24*60*60*atimeInt;
    long long int ctimeWithSign = now - 24*60*60*ctimeInt;

    char sign = (atimeInt == 0) ? '*' : atime[0];
    switch (sign){
        case '+':
            atimeWithSign |= PLUS_MASK<<SHIFT;
            break;
        case '-':
            atimeWithSign |= MINUS_MASK<<SHIFT;
            break;
        case '*':
            atimeWithSign |= ZERO_MASK<<SHIFT;
            break;
        default:
            atimeWithSign |= EQ_MASK << SHIFT;
            break;
    }
    sign = (ctimeInt == 0) ? '*' : ctime[0];
    switch (sign){
        case '+':
            ctimeWithSign |= PLUS_MASK<<SHIFT;
            break;
        case '-':
            ctimeWithSign |= MINUS_MASK<<SHIFT;
            break;
        case '*':
            ctimeWithSign |= ZERO_MASK<<SHIFT;
            break;
        default:
            ctimeWithSign |= EQ_MASK << SHIFT;
            break;
    }
    MAX_LEVEL = maxDepthExtracted;
    ATIME = atimeWithSign;
    CTIME = ctimeWithSign;
    if(flag == REQ) {
        findRequ(maxDepthExtracted);
    }
    else {
        findNftw();
    }
}

void printStat(struct stat* statBuff){
    char* type;
    if(S_ISREG(statBuff->st_mode)){
        type = "REG";
    } else if(S_ISDIR(statBuff->st_mode)){
        type = "DIR";
    } else if(S_ISCHR(statBuff->st_mode)){
        type = "CHAR_DEV";
    } else if(S_ISBLK(statBuff->st_mode)){
        type = "BLOCK_DEV";
    } else if(S_ISFIFO(statBuff->st_mode)){
        type = "FIFO";
    } else if(S_ISLNK(statBuff->st_mode)){
        type = "LINK";
    } else if(S_ISSOCK(statBuff->st_mode)){
        type = "SOCKET";
    } else{
        type = "UN_DEF";
    }
    unsigned long nlink = statBuff->st_nlink;
    unsigned long size = statBuff->st_size;

    struct tm atimeTm;
    struct tm ctimeTm;

    localtime_r(&statBuff->st_atim.tv_sec, &atimeTm);
    localtime_r(&statBuff->st_ctim.tv_sec, &ctimeTm);

    char atimeStr[80];
    char ctimeStr[80];
    strftime(atimeStr,80,"%x-%I:%M%p", &atimeTm);
    strftime(ctimeStr,80,"%x-%I:%M%p", &ctimeTm);

    //printf("%ld %s %ld %s %s \n", nlink, type, size, atimeStr, ctimeStr);
    printf("%ld %s %ld %s\n", nlink, type, size, atimeStr);


}

bool validateTime(long long int fileTime, long long int requiredTime){
    int mask = requiredTime>>SHIFT;
    requiredTime &= MASK;
    bool ret;
    fileTime /= (24*60*60);
    fileTime *= (24*60*60);
    if(mask == PLUS_MASK){
        ret = (fileTime < requiredTime)?true:false;
    } else if(mask == EQ_MASK){
        ret = (fileTime == requiredTime)?true:false;
    } else if(mask == MINUS_MASK){
        ret = (fileTime > requiredTime)?true:false;
    } else{
        ret = true;
    }
    return ret;
}

int findRequ(int maxDepth){
    char pathBuff[PATH_MAX];
    struct stat statBuff;
    if(getcwd(pathBuff, PATH_MAX) != NULL){
        stat(pathBuff, &statBuff);
        if(validateTime(statBuff.st_atim.tv_sec, ATIME)==true && validateTime(statBuff.st_ctim.tv_sec, CTIME)==true) {
            printf("%s ", pathBuff);
            printStat(&statBuff);
        }
        if(maxDepth == 0){
            chdir("..");
            return 0;
        }
        char childPath[PATH_MAX];
        int status = 0;
        DIR* dir;
        struct dirent* dirent = NULL;
        dir = opendir(pathBuff);
        while ((dirent = readdir(dir)) != NULL){
            strcpy(childPath, pathBuff);
            strcat(childPath, "/");
            strcat(childPath, dirent->d_name);
            stat(childPath, &statBuff);
            if(strcmp(dirent->d_name, "..") != 0 && strcmp(dirent->d_name, ".") != 0
                && validateTime(statBuff.st_atim.tv_sec, ATIME) && validateTime(statBuff.st_ctim.tv_sec, CTIME)) {
                if(dirent->d_type == DT_DIR){
                    chdir(childPath);
                    status = findRequ(maxDepth-1);
                    if(status == -1){
                        closedir(dir);
                        return -1;
                    }
                } else{
                    printf("%s ", childPath);
                    printStat(&statBuff);
                }
            }
        }
        closedir(dir);
    }else{
        if(errno != 0) {
            fprintf(stderr, "%s\n", strerror(errno));
            return -1;
        }else{
            chdir("..");
            return 0;
        }
    }
}

int displayInfo(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf){
    if (ftwbuf->level > MAX_LEVEL || validateTime(sb->st_atim.tv_sec, ATIME) == false || validateTime(sb->st_ctim.tv_sec, CTIME) == false) {
        return 0;
    }
    //tu jeszcze parę linijek na sprawdzenie tym ctime atime
    printf("%s ",fpath);
    printStat(sb);
    return 0;
}

void findNftw(){
    char pathBuff[PATH_MAX];
    getcwd(pathBuff, PATH_MAX);
    nftw(pathBuff, displayInfo, 20, 0);
}