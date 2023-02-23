#include <sys/time.h>
#include <cstdio>
#include <iostream>
#include <complex>
#include <string.h>
#include <string>
#include "matrix.h"

#ifndef SIZE
#define SIZE 4
#endif

#ifndef ROW
#define ROW 2
#endif

#ifndef COLUMN
#define COLUMN 3
#endif

FLOAT sumall(FLOAT *A);
FLOAT resd(FLOAT *A, FLOAT *B);
void checkvalue(FLOAT *A, FLOAT *B);

#define printftime(method, timeuse, A, B)                                                          \
    {                                                                                              \
        std::string s = "" #method "";                                                             \
        std::cout << s << ":\t" << timeref << "\t" << timeuse << "\t" << timeref / timeuse         \
                  << "\t\t" << resd(A, B) << std::endl;                                            \
    }
#define printfnorm(A)                                                                              \
    {                                                                                              \
        std::cout << " sumall( ) :" << sumall(A) << std::endl;                                     \
    }
#define checkValue(A, B)                                                                           \
    {                                                                                              \
        if (SIZE < 9)                                                                              \
            std::cout << "    " #A "      \t" #B "          \t" #A " - " #B "" << std::endl;       \
        checkvalue(A, B);                                                                          \
    }

int main(int argc, char **argv)
{
    struct timeval start;
    struct timeval end;

    FLOAT *A = (FLOAT *) _mm_malloc(SIZE * COLUMN * ROW * sizeof(FLOAT), ALIGEN_BTS);
    FLOAT *X = (FLOAT *) _mm_malloc(SIZE * ROW * sizeof(FLOAT), ALIGEN_BTS);
    FLOAT *Y = (FLOAT *) _mm_malloc(SIZE * COLUMN * sizeof(FLOAT), ALIGEN_BTS);
    FLOAT *Z = (FLOAT *) _mm_malloc(SIZE * COLUMN * sizeof(FLOAT), ALIGEN_BTS);

    size_t GridKB = SIZE * sizeof(FLOAT) / 1024;
    printf(" Grid Size: %d\n", SIZE);
    size_t AKB = GridKB * COLUMN * ROW;
    size_t XKB = GridKB * ROW;
    printf("Memory(KB): A = %ld,  X = %ld,  A+X = %ld\n", AKB, XKB, AKB + XKB);

    /// initial
    FLOAT invRandMax = 1.0 / RAND_MAX;
    for (size_t i = 0; i < ROW * SIZE; i++) {
        X[i] = invRandMax * (FLOAT) rand();
    }
    for (size_t i = 0; i < COLUMN * ROW * SIZE; i++) {
        A[i] = invRandMax * (FLOAT) rand();
    }

    /// test
    // memset(Z, 0, COLUMN * SIZE * sizeof(FLOAT));
    // memset(Y, 0, COLUMN * SIZE * sizeof(FLOAT));
    // gemv(Z, A, X, COLUMN, ROW, SIZE);
    // simdGemv(Y, A, X, COLUMN, ROW, SIZE);
    // std::cout << " sumall(Z) :" << sumall(Z) << std::endl;
    // std::cout << " sumall(Y) :" << sumall(Y) << "  simd" << std::endl;
    // checkValue(Z, Y);
    // TG_gemv00(Y, A, X, COLUMN, ROW, SIZE);
    // std::cout << " sumall(Y) :" << sumall(Y) << std::endl;
    // memset(Y, 0, COLUMN * SIZE * sizeof(FLOAT));
    // TG_gemv00(Y, A, X, COLUMN, ROW, SIZE);
    // std::cout << " sumall(Y) :" << sumall(Y) << std::endl;
    // checkValue(Z, Y);

    /// warm
    for (size_t wm = 0; wm < 10; wm++) {
        memset(Z, 0, COLUMN * SIZE * sizeof(FLOAT));
        gemv(Y, A, X, COLUMN, ROW, SIZE);
        memset(Z, 0, COLUMN * SIZE * sizeof(FLOAT));
        simdGemv(Y, A, X, COLUMN, ROW, SIZE);

        memset(Z, 0, COLUMN * SIZE * sizeof(FLOAT));
        TG_gemv(Z, A, X, COLUMN, ROW, SIZE);
        memset(Y, 0, COLUMN * SIZE * sizeof(FLOAT));
        TG_simdGemv(Y, A, X, COLUMN, ROW, SIZE);
    }

    /// method reference
    memset(Z, 0, COLUMN * SIZE * sizeof(FLOAT));
    gettimeofday(&start, NULL);
    gemv(Z, A, X, COLUMN, ROW, SIZE);
    gettimeofday(&end, NULL);
    double timeref = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
    std::cout << "\"       gemv\":\t" << timeref << "\t" << timeref << "\t" << 1.0 << "\t\t"
              << resd(Z, Z) << std::endl;

    /// simd
    memset(Y, 0, COLUMN * SIZE * sizeof(FLOAT));
    gettimeofday(&start, NULL);
    simdGemv(Y, A, X, COLUMN, ROW, SIZE);
    gettimeofday(&end, NULL);
    double simdtime00 = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
    printftime("   simdGemv", simdtime00, Z, Y);
    checkValue(Z, Y);

    /// Tensor Grid
    /// Tensor grid method 00
    memset(Z, 0, COLUMN * SIZE * sizeof(FLOAT));
    gettimeofday(&start, NULL);
    TG_gemv(Z, A, X, COLUMN, ROW, SIZE);
    gettimeofday(&end, NULL);
    double timeTG = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
    printftime("    TG_gemv", timeTG, Z, Y);
    // checkValue(Z, Y);

    /// Tensor grid simd method 00
    memset(Y, 0, COLUMN * SIZE * sizeof(FLOAT));
    gettimeofday(&start, NULL);
    TG_simdGemv(Y, A, X, COLUMN, ROW, SIZE);
    gettimeofday(&end, NULL);
    double timeTGsimd = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
    printftime("TG_simdGemv", timeTGsimd, Z, Y);
    checkValue(Z, Y);

    /*
    /// Tensor grid method 01 
    memset(Y, 0, COLUMN * SIZE * sizeof(FLOAT));
    gettimeofday(&start, NULL);
    TG_gemv01(Y, A, X, COLUMN, ROW, SIZE);
    gettimeofday(&end, NULL);
    double time01 = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
    printftime("     method 01", time01, Z, Y);
    checkValue(Z, Y);
*/
    printf("\n");

    free(Z);
    free(Y);
    free(A);
    free(X);

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
        size_t i, c;
        for (i = 0; i < SIZE; i++) {
            for (c = 0; c < COLUMN; c++) {
                std::cout << A[i * COLUMN + c] << "  \t" << B[i * COLUMN + c] << "  \t"
                          << A[i * COLUMN + c] - B[i * COLUMN + c] << std::endl;
            }
            std::cout << "\n" << std::endl;
        }
    }
}