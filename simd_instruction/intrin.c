#include <immintrin.h>
#include <stdio.h>

#define SIZE 32
typedef __m256d SimdTypeVd;
typedef __m256 SimdTypeVf;
typedef float FLOAT;
#define NumBytesAlign (256 / 8)
int main()
{
    // FLOAT Va[SIZE] __attribute__((aligned(NumBytesAlign)));
    // FLOAT Vb[SIZE] __attribute__((aligned(NumBytesAlign)));
    FLOAT *Va = (FLOAT *) _mm_malloc(SIZE * sizeof(FLOAT), NumBytesAlign);
    // FLOAT *Vb = (FLOAT *) _mm_malloc(SIZE * sizeof(FLOAT), NumBytesAlign);
    for (size_t i = 0; i < SIZE; i++) {
        Va[i] = (FLOAT) i;
        // Vb[i] = -(FLOAT) i;
    }
    // SimdTypeVf evens = _mm256_load_ps(Va);
    // SimdTypeVf odds = _mm256_load_ps(Vb);

    FLOAT *f = NULL;
    __m256 result;

    printf("SimdTypeVd bytes = %ld\n", sizeof(SimdTypeVd));

    /* Initialize the two argument vectors */
    __m256 evens = _mm256_setr_ps(2.0, 4.0, 6.0, 8.0, 10.0, 12.0, 14.0, 16.0);
    __m256 odds = _mm256_setr_ps(1.0, 3.0, 5.0, 7.0, 9.0, 11.0, 13.0, 15.0);

    f = (FLOAT *) &evens;
    printf("vec evens: %f %f %f %f %f %f %f %f\n", f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);
    f = (FLOAT *) &odds;
    printf("vev  odds: %f %f %f %f %f %f %f %f\n", f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);

    /* Compute the difference between the two vectors */
    printf("\n_mm256_sub_ps(evens, odds)\n");
    result = _mm256_sub_ps(evens, odds);
    f = (FLOAT *) &result;
    printf("      sub: %f %f %f %f %f %f %f %f\n", f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);

    /* Test shuffle_ps */
    printf("\n_mm256_shuffle_ps(evens, odds, 0b00001101)\n");
    result = _mm256_shuffle_ps(evens, odds, 0b00001101);
    f = (FLOAT *) &result;
    printf("  shuffle: %f %f %f %f %f %f %f %f\n", f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);
    // 0b00001101                         01    11      00  00      01  11      00  00
    // m256 result                  evens 14    10 odds 15  15  even 6   2  odd  7   7
    result = _mm256_shuffle_ps(evens, evens, 0b10110001);
    f = (FLOAT *) &result;
    printf("  shuffle: %f %f %f %f %f %f %f %f\n", f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);

    printf("\n_mm256_permute_ps(evens, 0b10110001)\n");
    result = _mm256_permute_ps(evens, 0b10110001);
    f = (FLOAT *) &result;
    printf("  permute: %f %f %f %f %f %f %f %f\n", f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);

    printf("\n_mm256_hsub_ps(mul, div)\n");
    __m256 _mul = _mm256_mul_ps(evens, odds);
    f = (FLOAT *) &_mul;
    printf("      mul: %f %f %f %f %f %f %f %f\n", f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);
    __m256 _div = _mm256_div_ps(evens, odds);
    f = (FLOAT *) &_div;
    printf("      div: %f %f %f %f %f %f %f %f\n", f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);
    result = _mm256_hsub_ps(_mul, _div);
    f = (FLOAT *) &result;
    printf("     hsub: %f %f %f %f %f %f %f %f\n", f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);

    __m256d evend = _mm256_setr_pd(2.0, 4.0, 6.0, 8.0);
    __m256d oddd = _mm256_setr_pd(1.0, 3.0, 5.0, 7.0);
    __m256d resd;
    double *d;

    printf("\n_mm256_permute_pd(evend, 0b0101)\n");
    resd = _mm256_permute_pd(evend, 0b0101);
    d = (double *) &resd;
    printf("  permute: %lf %lf %lf %lf\n", d[0], d[1], d[2], d[3]);

    printf("%d  %d\n", 2 << 6, 0b10000000);

    /* Test insertf128*/
    // __m128 ist = _mm_set_ps(1.0, 3.0, 5.0, 7.0);
    // result = _mm256_insertf128_ps(evens, ist, 0);
    // f = (FLOAT *) &result;
    // printf("\n%f %f %f %f %f %f %f %f\n", f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);

    // result = _mm256_unpacklo_ps(evens, odds);

    // __m256i mask = _mm256_set_epi32(0, 0, 0, 0, 0, 128, 64, 0);
    // result = _mm256_maskload_ps(Va, mask);
    return 0;
}