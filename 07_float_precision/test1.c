#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prec_float.h"

int main()
{
    int SIZE = 1000;
    double *A = (double *) malloc(SIZE * 2 * sizeof(double));
    double *destA = (double *) malloc(SIZE * 2 * sizeof(double));
    float *AF = (float *) malloc(SIZE * 2 * sizeof(float));
    float *destF = (float *) malloc(SIZE * 2 * sizeof(float));

    srand(0);
    for (int i = 0; i < SIZE * 2; i++) {
        *(A + i) = (double) rand() / (double) (RAND_MAX);
        *(destA + i) = (double) rand() / (double) (RAND_MAX);
    }
    srand(0);
    for (int i = 0; i < SIZE * 2; i++) {
        *(AF + i) = (float) (rand()) / (RAND_MAX + 1.0);
        *(destF + i) = (float) (rand()) / (RAND_MAX + 1.0);
    }

    // norm2D(A, SIZE * 2);
    // norm2F(AF, SIZE * 2);
    axbD(A, destA, SIZE * 2);
    // axbF(AF, destF, SIZE * 2);

    return 0;
}