#include <sys/time.h>
#include <cstdio>
#include <iostream>
#include <complex>
#include <string.h>
#include "complex_mul_simd.h"

// #define SINGLE_PREC

#ifndef SIZE
#define SIZE 8
#endif

// using std::default_random_engine;
// default_random_engine rng;
// rng.seed(time(0));

FLOAT resd(FLOAT *A, FLOAT *B);
void checkValue(Complex *A, Complex *B);

int main(int argc, char **argv)
{
    struct timeval start;
    struct timeval end;

    void *ma = _mm_malloc(2 * SIZE * sizeof(FLOAT), ALIGEN_BTS);
    void *mb = _mm_malloc(2 * SIZE * sizeof(FLOAT), ALIGEN_BTS);
    void *mc = _mm_malloc(2 * SIZE * sizeof(FLOAT), ALIGEN_BTS);
    void *md = _mm_malloc(2 * SIZE * sizeof(FLOAT), ALIGEN_BTS);
    Complex *Va = static_cast<Complex *>(ma);
    Complex *Vb = static_cast<Complex *>(mb);
    Complex *Vd = static_cast<Complex *>(md);
    Complex *Vc = static_cast<Complex *>(mc);
    FLOAT *Fa = static_cast<FLOAT *>(ma);
    FLOAT *Fb = static_cast<FLOAT *>(mb);
    FLOAT *Fc = static_cast<FLOAT *>(mc);
    FLOAT *Fd = static_cast<FLOAT *>(md);

    // for (size_t i = 0; i < SIZE; i++) {
    //     Fa[2 * i + 0] = static_cast<FLOAT>(2 * i + 0);
    //     Fa[2 * i + 1] = static_cast<FLOAT>(2 * i + 1);
    //     Fb[2 * i + 0] = static_cast<FLOAT>(2 * i + 0);
    //     Fb[2 * i + 1] = -static_cast<FLOAT>(2 * i + 1);
    // }
    FLOAT invRandMax = 1.0 / RAND_MAX;
    for (size_t i = 0; i < 2 * SIZE; i++) {
        Fa[i] = invRandMax * (FLOAT) rand();
        Fb[i] = invRandMax * (FLOAT) rand();
    }

    for (size_t wm = 0; wm < 10; wm++) {
        std_vec_mul(Vc, Va, Vb, SIZE);
// #ifndef SINGLE_PREC
//         simd_mul_pd_rrii(Fd, Fa, Fb, 2 * SIZE);
// #else
//         simd_mul_ps_rrii(Fd, Fa, Fb, 2 * SIZE);
// #endif
    }

    printf("Time Use(ms): std_complex_vmul  simd_mul_ps/d   cml/simd  resd\n");

    /* 标准 */
    gettimeofday(&start, NULL);
    std_vec_mul(Vc, Va, Vb, SIZE);
    gettimeofday(&end, NULL);
    double timeref = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);

    /* Simd method 00 */
    gettimeofday(&start, NULL);
#ifndef SINGLE_PREC
    simd_mul_z(Fd, Fa, Fb, 2 * SIZE);
#else
    simd_mul_c(Fd, Fa, Fb, 2 * SIZE);
#endif
    gettimeofday(&end, NULL);
    double timeuse0 = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
    printf("method 00 hsub  : %14.3lf %14.3lf %9.2lf %14.3e\n", timeref, timeuse0,
           timeref / timeuse0, resd(Fc, Fd));
    checkValue(Vc, Vd);

    /* Simd method 01 */
    gettimeofday(&start, NULL);
#ifndef SINGLE_PREC
    simd_mul_pd(Fd, Fa, Fb, 2 * SIZE);
#else
    simd_mul_ps(Fd, Fa, Fb, 2 * SIZE);
#endif
    gettimeofday(&end, NULL);
    double timeuse1 = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
    printf("method 01 addsub: %14.3lf %14.3lf %9.2lf %14.3e\n", timeref, timeuse1,
           timeref / timeuse1, resd(Fc, Fd));
    checkValue(Vc, Vd);

#ifdef AVX512
    /* Simd method 02 */
    gettimeofday(&start, NULL);
#ifndef SINGLE_PREC
    simd_mul_pd_rrii(Fd, Fa, Fb, 2 * SIZE);
#else
    simd_mul_ps_rrii(Fd, Fa, Fb, 2 * SIZE);
#endif
    gettimeofday(&end, NULL);
    double timeuse2 = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
    printf("method 02 rrii  : %14.3lf %14.3lf %9.2lf %14.3e\n", timeref, timeuse2,
           timeref / timeuse2, resd(Fc, Fd));
    checkValue(Vc, Vd);
#endif

    /* Simd method 03 */
    gettimeofday(&start, NULL);
#ifndef SINGLE_PREC
    simd_mul_pd_rrii2(Fd, Fa, Fb, 2 * SIZE);
#else
    // simd_mul_ps_rrii2(Fd, Fa, Fb, 2 * SIZE);
#endif
    gettimeofday(&end, NULL);
    double timeuse3 = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
    /* */
    printf("method 03 rrii2 : %14.3lf %14.3lf %9.2lf %14.3e\n", timeref, timeuse3,
           timeref / timeuse3, resd(Fc, Fd));
    checkValue(Vc, Vd);

    free(ma);
    free(mb);
    free(mc);
    free(md);
    return 0;
}
// printf("check point %s, %d\n", __FILE__, __LINE__);

FLOAT resd(FLOAT *A, FLOAT *B)
{
    FLOAT res = 0.0;
    for (size_t i = 0; i < 2 * SIZE; i++) {
        res += A[i] - B[i];
    }
    return res;
}

void checkValue(Complex *A, Complex *B)
{
    /*  */
    if (SIZE <= 16) {
        for (size_t i = 0; i < SIZE; i++) {
            printf("%11.3e  %11.3e    %11.3e  %11.3e    %14.5e  %14.5e\n", A[i].real(), A[i].imag(),
                   B[i].real(), B[i].imag(), A[i].real() - B[i].real(), A[i].imag() - B[i].imag());
        }
    }
}
