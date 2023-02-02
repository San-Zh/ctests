/**
 * @file complex_vector.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-01-22
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <complex.h>
#include "prec_float.h"

int main(int argc, char **argv)
{
    int SIZE, LOOP;
    if (argc > 1) {
        SIZE = atoi(argv[1]);
        LOOP = atoi(argv[2]);
    } else {
        SIZE = 1000;
        LOOP = 100;
    }

    double *A = (double *) malloc(SIZE * 2 * sizeof(double));
    float *AF = (float *) malloc(SIZE * 2 * sizeof(float));
    // ComplexD *C = (ComplexD *) A;
    // ComplexF *CF = (ComplexF *) AF;
    srand(0);
    for (int i = 0; i < SIZE * 2; i++) {
        *(A + i) = (double) rand() / (double) (RAND_MAX);
    }
    srand(0);
    for (int i = 0; i < SIZE * 2; i++) {
        *(AF + i) = (float) (rand()) / (RAND_MAX + 1.0);
    }
    // double *AFF = (double *) malloc(SIZE * sizeof(double));
    // memcpy(AFF, AF, SIZE * sizeof(double));

    double renomalize = (double) (1.0 / (SIZE * LOOP));
    // double precision
    double timeuseD = 0.0;
    struct timeval startD;
    struct timeval endD;
    // single precision
    double timeuseF = 0.0;
    struct timeval startF;
    struct timeval endF;

    for (int loop = 0; loop < LOOP; loop++) {
        {
            double nrm2D = 0.0;
            gettimeofday(&startD, NULL);
            nrm2D += norm2D(A, SIZE * 2) * renomalize;
            gettimeofday(&endD, NULL);
            timeuseD += (endD.tv_sec - startD.tv_sec) * 1000000 + (endD.tv_usec - startD.tv_usec);
            printf("Norm2 <double> time: %10.5lf, nrm2D=%11.8lf\n", timeuseD, nrm2D);
        }
        {
            float nrm2F = 0.0;
            gettimeofday(&startF, NULL);
            nrm2F += norm2F(AF, SIZE * 2) * renomalize;
            gettimeofday(&endF, NULL);
            timeuseF += (endF.tv_sec - startF.tv_sec) * 1000000 + (endF.tv_usec - startF.tv_usec);
            printf("Norm2 <single> time: %10.5f us, nrm2F=%11.8f, acc=%6.3lf\n\n", timeuseF, nrm2F,
                   timeuseD / timeuseF);
        }
    }

    free(A);
    free(AF);
    return 0;
}
