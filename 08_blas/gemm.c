/**
 * @file gemm.c
 * @author your name (you@domain.com)
 * @brief This is a test program to compare the performance of base_gemm, block_gemm, and cblas_dgemm.
 * @version 0.1
 * @date 2024-07-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>
// #include "cblas.h"

#define MDIM 1024
#define NDIM 1024
#define KDIM 1024

// base_gemm function
void base_gemm(double *a, double *b, double *c, int m, int n, int k)
{
#pragma omp parallel for collapse(2) schedule(static)
    for (int i = 0; i < m; i++) {
        for (int l = 0; l < k; l++) {
            for (int j = 0; j < n; j++) {
                c[i * n + j] += a[i * k + l] * b[l * n + j]; // compute c[i][j] += a[i][l] * b[l][j]
            }
        }
    }
}

// block_gemm function
/*
Function: block_gemm
Arguments:
  - a: pointer to input matrix a
  - b: pointer to input matrix b
  - c: pointer to output matrix c
  - m: number of rows in matrix a and output matrix c
  - n: number of columns in matrix b and output matrix c
  - k: number of columns in matrix a and number of rows in matrix b

This function performs blocked matrix multiplication for matrices a, b, and c with dimensions m x k, k x n, and m x n respectively.
*/
#if defined V0
void block_gemm(double *a, double *b, double *c, int m, int n, int k, int block_size)
{
    int k_block = 4; // number of blocks in k direction
    // Iterate over blocks of the matrices
#pragma omp parallel for collapse(3) schedule(static)
    for (int i = 0; i < m; i += block_size) {
        for (int l = 0; l < k; l += block_size) {
            for (int j = 0; j < n; j += block_size) {
                // Perform multiplication within each block
                for (int ii = i; ii < i + block_size && ii < m; ii++) {
                    for (int ll = l; ll < l + block_size && ll < k; ll++) {
                        for (int jj = j; jj < j + block_size && jj < n; jj++) {
                            // Accumulate the result of multiplication in the output matrix c
                            c[ii * n + jj] += a[ii * k + ll] * b[ll * n + jj];
                        }
                    }
                }
            }
        }
    }
}
#endif

#ifdef V1
// optimized_block_gemm function
void block_gemm(double *a, double *b, double *c, int m, int n, int k, int block_size)
{
    // int block_size = 8;                   // 用于分块矩阵乘法的块大小
    int step = 64 / sizeof(double); // 每个缓存行可以容纳的 double 类型元素个数

    // 并行计算每个块
#pragma omp parallel for collapse(3) schedule(static)
    for (int i = 0; i < m; i += block_size) {
        for (int j = 0; j < n; j += block_size) {
            for (int l = 0; l < k; l += block_size) {
                // 对每个块进行计算
                for (int ii = i; ii < i + block_size; ii += step) {
                    for (int jj = j; jj < j + block_size; jj += step) {
                        for (int ll = l; ll < l + block_size; ll += step) {
                            // 在计算内部，首先将划分的子块加载到 L1/L2 缓存中
                            // 然后执行计算以利用局部性原理
                            // 最后将计算结果写回内存前进行合并处理
                            for (int i1 = ii; i1 < ii + step && i1 < i + block_size; i1++) {
                                for (int j1 = jj; j1 < jj + step && j1 < j + block_size; j1++) {
                                    for (int l1 = ll; l1 < ll + step && l1 < l + block_size; l1++) {
                                        c[i1 * n + j1] += a[i1 * k + l1] * b[l1 * n + j1];
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
#endif

#ifdef V2

#define CACHE_LINE_SIZE 64
#define L1_CACHE_SIZE   131072  // 128 KB
#define L2_CACHE_SIZE   1048576 // 1 MB
#define L3_CACHE_SIZE   6291456 // 6 MB
#define NUM_CORES       4

void block_gemm(double *a, double *b, double *c, int m, int n, int k, int block_size)
{
    // Determine the optimal block size based on cache sizes and CPU cores
    int max_block_size = (L1_CACHE_SIZE / sizeof(double)) / (3 * NUM_CORES);
    block_size         = block_size < max_block_size ? block_size : max_block_size;

#pragma omp parallel for collapse(3) schedule(static)
    for (int i = 0; i < m; i += block_size) {
        for (int j = 0; j < n; j += block_size) {
            for (int l = 0; l < k; l += block_size) {
                int block_end_i = i + block_size < m ? i + block_size : m;
                int block_end_j = j + block_size < n ? j + block_size : n;
                int block_end_l = l + block_size < k ? l + block_size : k;
                for (int ii = i; ii < block_end_i; ii++) {
                    for (int jj = j; jj < block_end_j; jj++) {
                        for (int ll = l; ll < block_end_l; ll++) {
                            c[ii * n + jj] += a[ii * k + ll] * b[ll * n + jj];
                        }
                    }
                }
            }
        }
    }
}
#endif


// self_defined_gemm function using avx2 instructions
void avx2_gemm(double *a, double *b, double *c, int m, int n, int k)
{
    // TODO: implement self_defined_gemm using avx2 instructions
}

// sve_gemm function using sve instructions
void sve_gemm(double *a, double *b, double *c, int m, int n, int k)
{
    // TODO: implement sve_gemm using sve instructions
}


int main(int argc, char **argv)
{
    int block_size = atoi(argv[1]);
    if (block_size == 0) { block_size = 8; }

    printf("MDIM: %d, NDIM: %d, KDIM: %d\n", MDIM, NDIM, KDIM);
    printf("block_size: %d\n", block_size);

    struct timeval start, end;

    double base_time = 0.0, block_time = 0.0, cblas_time = 0.0;

    double *a = (double *) malloc(MDIM * KDIM * sizeof(double));
    double *b = (double *) malloc(KDIM * NDIM * sizeof(double));
    double *c = (double *) malloc(MDIM * NDIM * sizeof(double));

    const int    m = MDIM, n = NDIM, k = KDIM, lda = KDIM, ldb = NDIM, ldc = NDIM;
    const double al = 1.0, be = 0.0;

    // initialize matrix a, b and c
    srand(0);
    for (int i = 0; i < m * k; i++) { a[i] = rand() / (double) RAND_MAX; }
    for (int i = 0; i < k * n; i++) { b[i] = rand() / (double) RAND_MAX; }
    for (int i = 0; i < k * n; i++) { b[i] = 2.0; }

    // call base_gemm and calculate the used time
    for (int i = 0; i < m * n; i++) { c[i] = 0.0; }
    gettimeofday(&start, NULL);
    base_gemm(a, b, c, m, n, k);
    gettimeofday(&end, NULL);
    // calculate the used time
    base_time = (end.tv_sec - start.tv_sec) * 1000000.0 + (end.tv_usec - start.tv_usec) * 1.0;
    printf("base_gemm : time: %g us\n", base_time);

    //call block_gemm and calculate the used time
    for (int i = 0; i < m * n; i++) { c[i] = 0.0; }
    gettimeofday(&start, NULL);
    block_gemm(a, b, c, m, n, k, block_size);
    gettimeofday(&end, NULL);
    // calculate the used time
    block_time = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
    printf("block_gemm: time: %g us\n", block_time);
    printf("block_gemm: speedup based on base_gemm: %f\n", base_time / block_time);

#if defined(HAVE_CBLAS)
    // reference: cblas_dgemm function to perform matrix multiplication
    gettimeofday(&start, NULL);
    cblas_dgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, CblasNoTrans, m, n, k, al, a, lda, b, ldb, be, c, ldc);
    gettimeofday(&end, NULL);
    // calculate the used time
    cblas_time = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
    printf("cblas_dgemm: time: %lf us\n", cblas_time);
    printf("cblas_dgemm: speedup based on base_gemm: %lf\n", base_time / cblas_time);
#endif

    free(a);
    free(b);
    free(c);

    return 0;
}