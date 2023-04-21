#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prec_float.h"

int main(int argc, char *argv[])
{
    int SIZE, LOOP;
    if (argc > 1) {
        SIZE = atoi(argv[1]);
        // LOOP = atoi(argv[2]);
    } else {
        SIZE = 1000;
        // LOOP = 100;
    }

    // double precision
    double timeuseD = 0.0;
    struct timeval startD;
    struct timeval endD;
    // single precision
    double timeuseF = 0.0;
    struct timeval startF;
    struct timeval endF;

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
    gettimeofday(&startD, NULL);
    axbD(A, destA, SIZE * 2);
    gettimeofday(&endD, NULL);
    timeuseD = (endD.tv_sec - startD.tv_sec) * 1000000 + (endD.tv_usec - startD.tv_usec);

    gettimeofday(&startF, NULL);
    axbF(AF, destF, SIZE * 2);
    gettimeofday(&endF, NULL);
    timeuseF = (endF.tv_sec - startF.tv_sec) * 1000000 + (endF.tv_usec - startF.tv_usec);

    printf("time axbD time: %10.5f us\n", timeuseD);
    printf("time axbF time: %10.5f us, acc=%6.3lf\n\n", timeuseF, timeuseD / timeuseF);

    return 0;
}