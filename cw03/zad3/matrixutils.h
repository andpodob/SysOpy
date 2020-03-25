//
// Created by andrz on 3/21/2020.
//

#ifndef CW03_MATRIXUTILS_H
#define CW03_MATRIXUTILS_H

#include <stdlib.h>

int** multiplyMatrices(int** matrixA, int** matrixB, int* resultRows, int* resultCol, int rowsA, int columnsA, int rowsB, int columnsB, int idxStartingColumn);
void freeMatrix(int** matrix, int rows);

#endif //CW03_MATRIXUTILS_H
