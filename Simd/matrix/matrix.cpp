#include <sys/time.h>
#include <cstdio>
#include <iostream>
#include <complex>
#include <string.h>
#include <string>
#include "matrix.h"

// #define SINGLE_PREC

#ifndef SIZE
#define SIZE 4
#endif

#ifndef ROW
#define ROW 3
#endif

#ifndef COLUMN
#define COLUMN 3
#endif

FLOAT sumall(FLOAT *A);
FLOAT resd(FLOAT *A, FLOAT *B);
void checkvalue(FLOAT *A, FLOAT *B);

#define printftime(method, timeuse, A, B)                                                          \
    {                                                                                              \
        printf("%15s:%8.2lf%8.2lf% 8.2lf% 14.5e\n", #method, timeref, timeuse, timeref / timeuse,  \
               resd(A, B));                                                                        \
    }
#define printfnorm(A)                                                                              \
    {                                                                                              \
        printf(" sumall(" #A ") = \n", sumall(A));                                                 \
    }
#define checkValue(A, B)                                                                           \
    {                                                                                              \
        if (SIZE < 9)                                                                              \
            printf("    " #A "      \t" #B "          \t" #A " - " #B "\n");                       \
        checkvalue(A, B);                                                                          \
    }

#define Checkpoint                                                                                 \
    {                                                                                              \
        printf(" #### %s: line %d... \n", __FILE__, __LINE__);                                     \
    }

int main(int argc, char **argv)
{
    printf("Grid Size: %d\nTensor <C,R> = <%d, %d>           sizeof(FLOAT) = %ld\n", SIZE, COLUMN,
           ROW, sizeof(FLOAT));
    if ((SIZE & 0b0111)) {
        printf("Tensor Grid Simd(256bits) method requires \b SIZE shuold be divisible by %d\n",
               FSIMD_SIZE);
        printf("SIZE%%8 = %d\nExit\n\n", SIZE & 0b0111);
        exit(EXIT_FAILURE);
    }
    struct timeval start;
    struct timeval end;

    FLOAT *A = (FLOAT *) _mm_malloc(SIZE * COLUMN * ROW * sizeof(FLOAT), ALIGEN_BTS);
    FLOAT *X = (FLOAT *) _mm_malloc(SIZE * ROW * sizeof(FLOAT), ALIGEN_BTS);
    FLOAT *Y = (FLOAT *) _mm_malloc(SIZE * COLUMN * sizeof(FLOAT), ALIGEN_BTS);
    FLOAT *Z = (FLOAT *) _mm_malloc(SIZE * COLUMN * sizeof(FLOAT), ALIGEN_BTS);

    size_t GridKB = SIZE * sizeof(FLOAT) / 1024;
    size_t AKB = GridKB * COLUMN * ROW;
    size_t XKB = GridKB * ROW;
    printf("Memory: A %ld KB,  X %ld KB,  A+X %ld KB\n", AKB, XKB, AKB + XKB);
    printf("---------------------------------------------------------\n");
    printf("%16s%8s%8s%8s%14s\n", "--method--", "timeref", "timeuse", "ACC", "diff");
    /// initial
    FLOAT invRandMax = 1.0 / RAND_MAX;
    for (size_t i = 0; i < ROW * SIZE; i++) {
        X[i] = invRandMax * (FLOAT) rand();
    }
    for (size_t i = 0; i < COLUMN * ROW * SIZE; i++) {
        A[i] = invRandMax * (FLOAT) rand();
    }

    /// warm
    for (size_t wm = 0; wm < 100; wm++) {
        memset(Z, 0, COLUMN * SIZE * sizeof(FLOAT));
        gemv(Z, A, X, COLUMN, ROW, SIZE);

        // memset(Y, 0, COLUMN * SIZE * sizeof(FLOAT));
        // simdGemv(Y, A, X, COLUMN, ROW, SIZE);

        memset(Z, 0, COLUMN * SIZE * sizeof(FLOAT));
        TG_gemv(Z, A, X, COLUMN, ROW, SIZE);

        memset(Y, 0, COLUMN * SIZE * sizeof(FLOAT));
        TG_simdGemv(Y, A, X, COLUMN, ROW, SIZE);
    }

    /////////////// Tensor Grid ////////////////

    /// gemv:method reference
    memset(Z, 0, COLUMN * SIZE * sizeof(FLOAT));
    gettimeofday(&start, NULL);
    gemv(Z, A, X, COLUMN, ROW, SIZE);
    gettimeofday(&end, NULL);
    double timeref = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
    printftime(gemv, timeref, Z, Z);

    // /// simdGemv
    // memset(Y, 0, COLUMN * SIZE * sizeof(FLOAT));
    // gettimeofday(&start, NULL);
    // simdGemv(Y, A, X, COLUMN, ROW, SIZE);
    // gettimeofday(&end, NULL);
    // double simdtime00 = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
    // printftime(simdGemv, simdtime00, Z, Y);
    // checkValue(Z, Y);

    // /// simdGemv01
    // memset(Y, 0, COLUMN * SIZE * sizeof(FLOAT));
    // gettimeofday(&start, NULL);
    // simdGemv01(Y, A, X, COLUMN, ROW, SIZE);
    // gettimeofday(&end, NULL);
    // double simdtime01 = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
    // printftime(simdGemv01, simdtime01, Z, Y);
    // checkValue(Z, Y);

    /////////////// Tensor Grid ////////////////
    /// TG_gemv: Tensor grid method 00
    memset(Z, 0, COLUMN * SIZE * sizeof(FLOAT));
    gettimeofday(&start, NULL);
    TG_gemv(Z, A, X, COLUMN, ROW, SIZE);
    gettimeofday(&end, NULL);
    double timeTG = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
    printftime(TG_gemv, timeTG, Z, Y);

    /// TG_simdGemv: Tensor grid simd method 00
    memset(Y, 0, COLUMN * SIZE * sizeof(FLOAT));
    gettimeofday(&start, NULL);
    TG_simdGemv(Y, A, X, COLUMN, ROW, SIZE);
    gettimeofday(&end, NULL);
    double timeTGsimd = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
    printftime(TG_simdGemv, timeTGsimd, Z, Y);
    checkValue(Z, Y);

    ////////// Tensor Grid Gemv 01 ///////////

    /// TG_gemv01: Tensor grid method 01
    memset(Z, 0, COLUMN * SIZE * sizeof(FLOAT));
    gettimeofday(&start, NULL);
    TG_gemv01(Z, A, X, COLUMN, ROW, SIZE);
    gettimeofday(&end, NULL);
    double timeTG01 = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
    printftime(TG_gemv01, timeTG01, Z, Y);
    checkValue(Z, Y);

    /// TG_simdGemv01: Tensor grid method 01
    memset(Y, 0, COLUMN * SIZE * sizeof(FLOAT));
    gettimeofday(&start, NULL);
    TG_simdGemv01(Y, A, X, COLUMN, ROW, SIZE);
    gettimeofday(&end, NULL);
    double timeTGsimd01 = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
    printftime(TG_simdGemv01, timeTGsimd01, Z, Y);
    checkValue(Z, Y);

    printf("\n");

    _mm_free(Z);
    _mm_free(Y);
    _mm_free(A);
    _mm_free(X);

    return 0;
}

/* sumall */
FLOAT sumall(FLOAT *A)
{
    FLOAT res = 0.0;
    for (size_t i = 0; i < COLUMN * SIZE; i++) {
        res += A[i];
    }
    return res;
}

/* resd */
FLOAT resd(FLOAT *A, FLOAT *B)
{
    FLOAT res = 0.0;
    for (size_t i = 0; i < COLUMN * SIZE; i++) {
        res += A[i] - B[i];
    }
    return res;
}

/* checkVale */
// #define checkValue(FLOAT *A, FLOAT *B)

void checkvalue(FLOAT *A, FLOAT *B)
{
    if (SIZE <= 8) {
        // size_t i, c;
        for (size_t i = 0; i < SIZE; i++) {
            for (size_t c = 0; c < COLUMN; c++) {
                std::cout << A[i * COLUMN + c] << "  \t" << B[i * COLUMN + c] << "  \t"
                          << A[i * COLUMN + c] - B[i * COLUMN + c] << std::endl;
            }
            std::cout << "\n" << std::endl;
        }
    }
}