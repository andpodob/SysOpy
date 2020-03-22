#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//

#ifndef SYSOPY_DIFF_LIB_H
#define SYSOPY_DIFF_LIB_H
void execDiff(const char* fileA, const char* fileB, const char* outputFile);
int parseOutputFile(char* outputFile, char** blocks);
#endif //SYSOPY_DIFF_LIB_H
