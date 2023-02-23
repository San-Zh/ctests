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
    __m256 vf0 = _mm256_setr_ps(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0);
    __m256 vf1 = _mm256_setr_ps(9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0);

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

    printf("\n\n===== normal E-O m256f ===== \n");
    f = (FLOAT *) &vf0;
    printf("     vf0: %f %f %f %f %f %f %f %f\n", f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);
    f = (FLOAT *) &vf1;
    printf("     vf1: %f %f %f %f %f %f %f %f\n", f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);
    f = (FLOAT *) &result;
    result = _mm256_movehdup_ps(vf0);
    printf("movehdup: %f %f %f %f %f %f %f %f\n", f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);
    result = _mm256_mask_movehdup_ps(vf0, 0b01010101, vf1);
    printf("mskmvhud: %f %f %f %f %f %f %f %f\n", f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);
    result = _mm256_unpacklo_ps(vf0, vf1);
    printf("unpacklo: %f %f %f %f %f %f %f %f\n", f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);
    result = _mm256_shuffle_ps(vf0, vf1, 0b10001000);
    printf(" shuffle: %f %f %f %f %f %f %f %f\n", f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);
    result = _mm256_shuffle_f32x4(vf0, vf1, 0b00);
    printf("shufff32: %f %f %f %f %f %f %f %f\n", f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);
    result = _mm256_shuffle_f32x4(vf0, vf1, 0b11);
    printf("shufff32: %f %f %f %f %f %f %f %f\n", f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);
    evens = _mm256_shuffle_f32x4(vf0, vf1, 0b00);
    odds = _mm256_shuffle_f32x4(vf0, vf1, 0b11);
    result = _mm256_shuffle_ps(evens, odds, 0b10001000);
    printf("shufff32: %f %f %f %f %f %f %f %f\n", f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);
    result = _mm256_unpacklo_ps(_mm256_shuffle_ps(evens, odds, 0b10001000),
                                _mm256_shuffle_ps(evens, odds, 0b11011101));
    printf("shufff32: %f %f %f %f %f %f %f %f\n", f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);

    /* *** */
    __m256d evend = _mm256_setr_pd(2.0, 4.0, 6.0, 8.0);
    __m256d oddd = _mm256_set_pd(1.0, 3.0, 5.0, 7.0);
    __m256d resd;
    double *d;
    printf("\n\nm256d <4-double>\n");
    d = (double *) &evend;
    printf("   evend: %lf %lf %lf %lf\n", d[0], d[1], d[2], d[3]);
    d = (double *) &oddd;
    printf("    oddd: %lf %lf %lf %lf\n", d[0], d[1], d[2], d[3]);

    printf("\n_mm256_permute_pd(evend, 0b0101)\n");
    resd = _mm256_permute_pd(evend, 0b0101);
    d = (double *) &resd;
    printf("  permute: %lf %lf %lf %lf\n", d[0], d[1], d[2], d[3]);

    printf("\n_mm256_addsub_pd(evend, oddd);\n");
    resd = _mm256_addsub_pd(evend, oddd);
    d = (double *) &resd;
    printf("   addsub: %lf %lf %lf %lf\n", d[0], d[1], d[2], d[3]);

    printf("\n_mm256_fmadd_pd(evend, oddd, oddd);\n");
    resd = _mm256_fmadd_pd(evend, oddd, oddd);
    printf("     fadd: %lf %lf %lf %lf\n", d[0], d[1], d[2], d[3]);
    resd = 10.0 * evend;
    printf("10.0*evend: %lf %lf %lf %lf\n", d[0], d[1], d[2], d[3]);

    printf("\n\n===== normal E-O m256d ===== \n");
    __m256d vd0 = _mm256_setr_pd(1.0, 2.0, 3.0, 4.0);
    __m256d vd1 = _mm256_setr_pd(5.0, 6.0, 7.0, 8.0);
    d = (double *) &vd0;
    printf("       vd0: %lf %lf %lf %lf\n", d[0], d[1], d[2], d[3]);
    d = (double *) &vd1;
    printf("       vd1: %lf %lf %lf %lf\n", d[0], d[1], d[2], d[3]);
    resd = _mm256_unpacklo_pd(vd0, vd1);
    d = (double *) &resd;
    printf("  unpacklo: %lf %lf %lf %lf\n", d[0], d[1], d[2], d[3]);
    resd = _mm256_shuffle_pd(vd0, vd1, 0b0000);
    printf("   shuffle: %lf %lf %lf %lf\n", d[0], d[1], d[2], d[3]);
    resd = _mm256_shuffle_pd(resd, resd, 0b0100);
    printf("   shuffle: %lf %lf %lf %lf\n", d[0], d[1], d[2], d[3]);
    __m256d vd00 = _mm256_shuffle_f64x2(vd0, vd1, 0b00);
    d = (double *) &vd00;
    printf("shufflef64: %lf %lf %lf %lf\n", d[0], d[1], d[2], d[3]);
    __m256d vd11 = _mm256_shuffle_f64x2(vd0, vd1, 0b11);
    d = (double *) &vd11;
    printf("shufflef64: %lf %lf %lf %lf\n", d[0], d[1], d[2], d[3]);

    d = (double *) &resd;
    resd = _mm256_shuffle_pd(vd00, vd11, 0b0000);
    printf("shufflef64: %lf %lf %lf %lf\n", d[0], d[1], d[2], d[3]);
    resd = _mm256_shuffle_pd(vd00, vd11, 0b1111);
    printf("shufflef64: %lf %lf %lf %lf\n", d[0], d[1], d[2], d[3]);

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