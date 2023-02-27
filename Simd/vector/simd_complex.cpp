#include <sys/time.h>
#include <cstdio>
#include <iostream>
#include <complex>
#include <random>
#include "simd_complex.h"

// #define SINGLE_PREC

#ifndef SINGLE_PREC
#define FLOAT double
#else
#define FLOAT float
#endif

#ifndef SIZE
#define SIZE 8
#endif

using Complex = std::complex<FLOAT>;
// using std::default_random_engine;
// default_random_engine rng;
// rng.seed(time(0));

int main(int argc, char **argv)
{
    double timeuse0 = 0.0, timeuse1 = 0.0;
    struct timeval start;
    struct timeval end;

    void *ma = _mm_malloc(2 * SIZE * sizeof(FLOAT), ALIGEN_BTS);
    void *mb = _mm_malloc(2 * SIZE * sizeof(FLOAT), ALIGEN_BTS);
    void *mc = _mm_malloc(2 * SIZE * sizeof(FLOAT), ALIGEN_BTS);
    void *md = _mm_malloc(2 * SIZE * sizeof(FLOAT), ALIGEN_BTS);
    Complex *Va = static_cast<Complex *>(ma);
    Complex *Vb = static_cast<Complex *>(mb);
    Complex *Vc = static_cast<Complex *>(mc);
    Complex *Vd = static_cast<Complex *>(md);
    FLOAT *Fa = static_cast<FLOAT *>(ma);
    FLOAT *Fb = static_cast<FLOAT *>(mb);
    FLOAT *Fc = static_cast<FLOAT *>(mc);

    FLOAT invRandMax = 1.0 / RAND_MAX;
    for (size_t i = 0; i < 2 * SIZE; i++) {
        Fa[i] = invRandMax * (FLOAT) rand();
        Fb[i] = invRandMax * (FLOAT) rand();
    }

    // for (size_t i = 0; i < SIZE; i++) {
    //     Fa[2 * i + 0] = static_cast<FLOAT>(2 * i + 0);
    //     Fa[2 * i + 1] = static_cast<FLOAT>(2 * i + 1);
    //     Fb[2 * i + 0] = static_cast<FLOAT>(2 * i + 10);
    //     Fb[2 * i + 1] = -static_cast<FLOAT>(2 * i + 11);
    // }

    /* 标准 */
    gettimeofday(&start, NULL);
    std_vec_mul(Vd, Va, Vb, SIZE);
    gettimeofday(&end, NULL);
    timeuse0 = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);

    /* Simd*/
    gettimeofday(&start, NULL);
#ifndef SINGLE_PREC
    simd_mul_z(Fc, Fa, Fb, 2 * SIZE);
#else
    simd_mul_c(Fc, Fa, Fb, 2 * SIZE);
#endif
    gettimeofday(&end, NULL);
    timeuse1 = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);

    /* */
    printf("Time Use(ms): %14.3lf %14.3lf   %14.2lf\n", timeuse0, timeuse1, timeuse0 / timeuse1);

    /* */
    FLOAT resd = 0.0;
    for (size_t i = 0; i < SIZE; i++) {
        resd += abs(Vc[i] - Vd[i]);
    }
    printf("resd = %e\n", resd);

    if (SIZE <= 16) {
        for (size_t i = 0; i < SIZE; i++) {
            printf("%11.3e  %11.3e    %11.3e  %11.3e    %14.5e  %14.5e\n", Vc[i].real(),
                   Vc[i].imag(), Vd[i].real(), Vd[i].imag(), Vc[i].real() - Vd[i].real(),
                   Vc[i].imag() - Vd[i].imag());
        }
    }

    free(ma);
    free(mb);
    free(mc);
    free(md);
    return 0;
}
// printf("check point %s, %d\n", __FILE__, __LINE__);