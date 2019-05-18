#include "matrixMul.h"

int main(){
	initMatrices(10);
    printMatrix(Matrix1);
    printMatrix(Matrix2);
    printMatrix(Matrix3);

    multiplyMatricesSequentially();
    printMatrix(Matrix3);

    emptyM3();

    multiplyMatricesWithTThreads(2);
    printMatrix(Matrix3);
}