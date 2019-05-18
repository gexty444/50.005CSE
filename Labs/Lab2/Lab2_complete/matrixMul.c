//
//  matrixMul.c
//  CSE_CCodes
//
//  Created by Natalie Agus on 11/1/19.
//  Copyright © 2019 Natalie Agus. All rights reserved.
//

#include "matrixMul.h"

int M1[M][N];
int M2[N][K];
int M3[M][K];

void initMatrices(int seed){
    //init random number generator
    srand(seed);
    
    //fill up M1
    for (int i = 0; i<M; i++){
        for (int j = 0; j<N; j++){
            M1[i][j] = rand() % 10;
        }
    }
    
    //fill up M2
    for (int i = 0; i<N; i++){
        for (int j = 0; j<K; j++){
            M2[i][j] = rand() % 10;
        }
    }
    
    emptyM3();
    

}

void emptyM3(void){
    //fill up M3 with zeroes
    for (int i = 0; i<M; i++){
        for (int j = 0; j<K; j++){
            M3[i][j] = 0;
        }
    }
}

void printMatrix(matrixName m){
    switch (m) {
        case Matrix1:
            //print M1
            printf("Printing M1:\n");
            for (int i = 0; i<M; i++){
                for (int j = 0; j<N; j++){
                    printf("%d, ",M1[i][j]);
                }
                printf("\n");
            }
            break;
        case Matrix2:
            //print M2
            printf("Printing M2: \n");
            for (int i = 0; i<N; i++){
                for (int j = 0; j<K; j++){
                    printf("%d, ",M2[i][j]);
                }
                printf("\n");
            }
            break;
        case Matrix3:
            //print M3
            printf("Printing M3:\n");
            for (int i = 0; i<M; i++){
                for (int j = 0; j<K; j++){
                    printf("%d, ",M3[i][j]);
                }
                printf("\n");
            }
            break;
        default:
            break;
    }
}

void multiplyMatricesSequentially(){
    for(int i = 0; i<M; i++){
        for(int j = 0; j<K; j++){
            int sum = 0;
            for(int k = 0; k<N; k++){
                sum += M1[i][k] * M2[k][j];
            }
            M3[i][j] = sum;
        }
    }
}

typedef struct threadArgs {
	int start_K;
	int start_M;
	int end_M;
	int end_K;
}TA;

/**
 @param T is the number of threads
 Each cell in M3 should be computed by just one Thread at most.
 A thread can compute multiple cells in M3.
 This is to avoid using mutex
 Hence, if T > #of cells in M3, clamp T to be equal to #cells in M3
 **/
void multiplyMatricesWithTThreads(int T){
	
	TA* args = malloc(T * sizeof(TA));		// allocate spaces
	pthread_t threads[T];						// thread identifier

	if (T > M*K) {		// clamp T to be equal to #cells in M3 if T > #cells in M3
		T = M * K;
	}


	for (int j = 0; j < T; j++)		// i represents the thread number
	{
		// prepare parameters for args
		// split threads to do different columns. so each thread will do for all rows but different columns
		args[j].start_M = 0;
        args[j].end_M = M;

        args[j].start_K = (double) K/T * j;
        args[j].end_K = (double) K/T*j + (double) K/T;

		pthread_create(&threads[j], NULL, fillM3TaskThreadMethodEntry, (void*) &args[j]);
	}

    assert(T > 0);
	for (int i = 0; i < T; i++) {
		//wait until the thread completes
		pthread_join(threads[i], NULL);
	}

	free(args);
}


/**
 This code is called by pthread_create in multiplyMatricesWithTThreads(int T)
 FROM THIS POINT ONWARDS, YOU ARE FREE TO MODIFY THE METHOD AND ITS ARGUMENTS.
 THE CODES PROVIDED BELOW SERVE AS A GUIDE ONLY.
 HOWEVER, DO NOT CHANGE ANY OF THE METHODS ABOVE THIS POINT. THEY ARE GOING TO BE USED TO RUN TEST CASES.
 **/

void *fillM3TaskThreadMethodEntry(void* args){
	TA* args_int = (TA*) args;  // type cast back to logical stuffss

	int startM = args_int->start_M;    //pointer so use -> to access attributes of struct
	int startK = args_int->start_K;
	int endM = args_int->end_M;
	int endK = args_int->end_K;

	fillM3Task(startM, startK, endM, endK);


	return 0;
}


/**
 @param startM: the start row in M1
 @param startK: the start column in M2
 @param endM: the end row in M1 (inclusive)
 @param endK: the end column in M2 (inclusive)
 Example: startM = 0, endM = 0, startK = 2, endK = 4 means
            1. multiply row 0 of M1 with column 2 of M2, store it at corresponding cell in M3
            2. multiply row 0 of M2 with column 3 of M2, store it at corresponding cell in M3
            3. multiply row 0 of M2 with column 4 of M2, store it at corresponding cell in M3
 **/
void fillM3Task(int startM, int startK, int endM, int endK){
    for (int i = startM; i < endM; i++) {

        for (int j = startK; j < endK; j++){

            for (int n = 0; n < N; n++) {
                M3[i][j] += M1[i][n] * M2[n][j];
            }
        }
    }
}

/**END OF LAB 2**/
