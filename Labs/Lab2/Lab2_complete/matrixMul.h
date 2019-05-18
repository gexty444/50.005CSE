//
//  matrixMul.h
//  CSE_CCodes
//
//  Created by Natalie Agus on 11/1/19.
//  Copyright © 2019 Natalie Agus. All rights reserved.
//

#ifndef matrixMul_h
#define matrixMul_h

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <assert.h>

#define M 10
#define N 2
#define K 8

typedef enum{
    Matrix1,
    Matrix2,
    Matrix3
} matrixName;

void initMatrices(int seed);
void emptyM3(void);
void printMatrix(matrixName m);
void multiplyMatricesSequentially(void);
void multiplyMatricesWithTThreads(int T);
void fillM3Task(int startM, int startK, int endM, int endK);
void *fillM3TaskThreadMethodEntry(void* args);
#endif /* matrixMul_h */
