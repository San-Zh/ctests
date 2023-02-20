#include <sys/time.h>
#include <cstdio>
#include <iostream>
#include <complex>
#include "simd_complex.h"

#define SIZE 4096 * 1028
#define FLOAT double

using ComplexF = std::complex<float>;
using ComplexD = std::complex<double>;

int main(int argc, char **argv)
{
    double timeuse0 = 0, timeuse1 = 0;
    struct timeval start;
    struct timeval end;

    void *ma = _mm_malloc(2 * SIZE * sizeof(FLOAT), 32);
    void *mb = _mm_malloc(2 * SIZE * sizeof(FLOAT), 32);
    void *mc = _mm_malloc(2 * SIZE * sizeof(FLOAT), 32);
    void *md = _mm_malloc(2 * SIZE * sizeof(FLOAT), 32);
    ComplexD *Va = static_cast<ComplexD *>(ma);
    ComplexD *Vb = static_cast<ComplexD *>(mb);
    ComplexD *Vc = static_cast<ComplexD *>(mc);
    ComplexD *Vd = static_cast<ComplexD *>(md);
    FLOAT *Fa = static_cast<FLOAT *>(ma);
    FLOAT *Fb = static_cast<FLOAT *>(mb);
    FLOAT *Fc = static_cast<FLOAT *>(mc);

    for (size_t i = 0; i < SIZE; i++) {
        Fa[2 * i + 0] = static_cast<FLOAT>(2 * i + 0);
        Fa[2 * i + 1] = static_cast<FLOAT>(2 * i + 1);
        Fb[2 * i + 0] = static_cast<FLOAT>(2 * i + 10);
        Fb[2 * i + 1] = -static_cast<FLOAT>(2 * i + 11);
    }

    /* Simd*/
    gettimeofday(&start, NULL);
    for (size_t i = 0; i < 2 * SIZE; i += SimdSizeDouble) {
        SimdVecDouble va = _mm256_load_pd(Fa + i);
        SimdVecDouble vb = _mm256_load_pd(Fb + i);
        // SimdVecDouble vc = Simd_mul_complex(va,vb);
        _mm256_store_pd(Fc + i, Simd_mul_complex(va, vb));
    }
    gettimeofday(&end, NULL);
    timeuse1 = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);

    /* 标准 */
    gettimeofday(&start, NULL);
    for (size_t i = 0; i < SIZE; i++) {
        Vd[i] = Va[i] * Vb[i];
    }
    gettimeofday(&end, NULL);
    timeuse0 = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);

    /* */
    printf("Time Use(ms): %14.3lf %14.3lf   %14.2lf\n", timeuse0, timeuse1,
           (timeuse0 - timeuse1) / timeuse0);

    // for (size_t i = 0; i < SIZE; i++) {
    //     printf("%14.lf%14.lf  %14.lf%14.lf\n", Vc[i].real(), Vc[i].imag(), Vd[i].real(),
    //            Vd[i].imag());
    // }

    return 0;
}