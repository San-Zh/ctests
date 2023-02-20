#include <sys/time.h>
#include <cstdio>
#include <iostream>
#include <complex>
#include <string.h>
#include "complex_mul_simd.h"

#define SIZE 8 * 1024 * 1024
#define FLOAT double
#define AligenBit 32

using ComplexF = std::complex<float>;
using Complex = std::complex<double>;

int main(int argc, char **argv)
{
    double timeuse0 = 0, timeuse1 = 0;
    struct timeval start;
    struct timeval end;

    unsigned long i = 0;

    void *ma = _mm_malloc(2 * SIZE * sizeof(FLOAT), AligenBit);
    void *mb = _mm_malloc(2 * SIZE * sizeof(FLOAT), AligenBit);
    Complex *Va = static_cast<Complex *>(ma);
    Complex *Vb = static_cast<Complex *>(mb);
    FLOAT *Fa = static_cast<FLOAT *>(ma);
    FLOAT *Fb = static_cast<FLOAT *>(mb);

    void *mc = _mm_malloc(2 * SIZE * sizeof(FLOAT), AligenBit);
    void *md = _mm_malloc(2 * SIZE * sizeof(FLOAT), AligenBit);
    Complex *Vd = static_cast<Complex *>(md);
    Complex *Vc = static_cast<Complex *>(mc);
    FLOAT *Fc = static_cast<FLOAT *>(mc);

    for (i = 0; i < SIZE; i++) {
        Fa[2 * i + 0] = static_cast<FLOAT>(2 * i + 0);
        Fa[2 * i + 1] = static_cast<FLOAT>(2 * i + 1);
        Fb[2 * i + 0] = static_cast<FLOAT>(2 * i + 10);
        Fb[2 * i + 1] = -static_cast<FLOAT>(2 * i + 11);
    }

    // /* warm */
    // // gettimeofday(&start, NULL);
    // for (i = 0; i < SIZE; i++) {
    //     Vd[i] = Va[i] + Vb[i];
    // }
    // // gettimeofday(&end, NULL);
    // // timeuse0 = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);

    /* 标准 */
    gettimeofday(&start, NULL);
    for (i = 0; i < SIZE; i++) {
        Vd[i] = Va[i] * Vb[i];
    }
    gettimeofday(&end, NULL);
    timeuse0 = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);

    /* Simd*/
    // printf("check point %s, %d\n", __FILE__, __LINE__);
    gettimeofday(&start, NULL);
    SIMD_mul_pd(Fc, Fa, Fb, 2 * SIZE);
    gettimeofday(&end, NULL);
    timeuse1 = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);

    /* */
    printf("Time Use(ms): %14.3lf %14.3lf   %14.2lf\n", timeuse0, timeuse1,
           (timeuse0 - timeuse1) / timeuse0);

    FLOAT resd = 0.0;
    for (i = 0; i < SIZE; i++) {
        resd += Vc[i].real() - Vd[i].real() + Vd[i].imag() - Vc[i].imag();
    }
    printf("resd = %lf\n", resd);
    
    if (SIZE < 16) {
        for (i = 0; i < SIZE; i++) {
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