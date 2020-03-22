//
// Created by andrz on 3/21/2020.
//

#include "matrixutils.h"

int** multiplyMatrices(int** matrixA, int** matrixB, int* resultRows, int* resultCol, int rowsA, int columnsA, int rowsB, int columnsB, int idxStartingColumn){
    int** result = malloc(rowsA*sizeof(int*));

    for(int i = 0; i < rowsA; i++){
        result[i] = malloc(columnsB* sizeof(int));
    }
    int sum = 0;
    for(int i = 0; i < rowsA; i++){
        for(int j = idxStartingColumn; j < columnsB+idxStartingColumn; j++){
            for(int k = 0; k < rowsB; k++){
                sum+=(matrixA[i][k]*matrixB[k][j]);
            }
            result[i][j-idxStartingColumn] = sum;
            sum = 0;
        }
    }
    *resultCol = columnsB;
    *resultRows = rowsA;

    return result;
}

void freeMatrix(int** matrix, int rows){
    if(matrix!= NULL) {
        for (int i = 0; i < rows; i++) {
            free(matrix[i]);
        }
        free(matrix);
    }
}