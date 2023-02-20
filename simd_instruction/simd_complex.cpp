#include <sys/time.h>
#include <cstdio>
#include <iostream>
#include <complex>
#include "simd_complex.h"

#define SIZE 8 * 1024 * 1024
#define FLOAT float
#define AligenBt 32

using Complex = std::complex<FLOAT>;

int main(int argc, char **argv)
{
    double timeuse0 = 0.0, timeuse1 = 0.0;
    struct timeval start;
    struct timeval end;

    void *ma = _mm_malloc(2 * SIZE * sizeof(FLOAT), AligenBt);
    void *mb = _mm_malloc(2 * SIZE * sizeof(FLOAT), AligenBt);
    void *mc = _mm_malloc(2 * SIZE * sizeof(FLOAT), AligenBt);
    void *md = _mm_malloc(2 * SIZE * sizeof(FLOAT), AligenBt);
    Complex *Va = static_cast<Complex *>(ma);
    Complex *Vb = static_cast<Complex *>(mb);
    Complex *Vc = static_cast<Complex *>(mc);
    Complex *Vd = static_cast<Complex *>(md);
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
    // simd_mul_z(Fc, Fa, Fb, 2 * SIZE);
    simd_mul_c(Fc, Fa, Fb, 2 * SIZE);
    gettimeofday(&end, NULL);
    timeuse1 = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);

    /* 标准 */
    gettimeofday(&start, NULL);
    std_vec_mul(Vd, Va, Vb, SIZE);
    gettimeofday(&end, NULL);
    timeuse0 = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);

    /* */
    printf("Time Use(ms): %14.3lf %14.3lf   %14.2lf\n", timeuse0, timeuse1,
           (timeuse0 - timeuse1) / timeuse0);

    /* */
    FLOAT resd = 0.0;
    for (size_t i = 0; i < SIZE; i++) {
        resd += Vc[i].real() - Vd[i].real() + Vd[i].imag() - Vc[i].imag();
    }
    printf("resd = %lf\n", resd);

    if (SIZE < 16) {
        for (size_t i = 0; i < SIZE; i++) {
            printf("%10.lf%10.lf  %14.lf%10.lf\n", Vc[i].real(), Vc[i].imag(), Vd[i].real(),
                   Vd[i].imag());
        }
    }

    free(ma);
    free(mb);
    free(mc);
    free(md);
    return 0;
}