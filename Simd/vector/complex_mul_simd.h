#ifndef _COMPLEX_MUL_SIMD_H
#define _COMPLEX_MUL_SIMD_H

// #include <immintrin.h>
#include <x86intrin.h>
#include <complex>
#include <stdio.h>

#define ALIGEN_BTS 32

#ifndef SINGLE_PREC
#define FLOAT double
#define EO_SWITCH_256 0b0101
#else
#define FLOAT float
#define EO_SWITCH_256 0b10110001
#endif
using Complex = std::complex<FLOAT>;

#ifdef __AVX__||__AVX2__
#if defined SINGLE_PREC
#define FSIMD_SIZE 8
#else
#define FSIMD_SIZE 4
#endif

#endif

#ifdef __SSE__||__SSE2__||__SSE4_2__ 
#if defined SINGLE_PREC
#define FSIMD_SIZE 4
#else
#define FSIMD_SIZE 2
#endif
#endif

template <typename T>
void std_vec_mul(T *Vd, const T *Va, const T *Vb, const int len)
{
    for (size_t i = 0; i < len; i++) {
        Vd[i] = Va[i] * Vb[i];
    }
}

void simd_mul_z(double *Fc, const double *Fa, const double *Fb, const int len)
{
    __m256d CdNegSign = _mm256_set_pd(-1.0, 1.0, -1.0, 1.0);
    for (size_t i = 0; i < len; i += FSIMD_SIZE) {
        __m256d va = _mm256_load_pd(Fa + i);
        __m256d vb = _mm256_load_pd(Fb + i);
        __m256d vavbi = _mm256_mul_pd(va, _mm256_permute_pd(vb, 0b0101));
        __m256d vavbr = _mm256_mul_pd(_mm256_mul_pd(va, vb), CdNegSign);
        _mm256_store_pd(Fc + i, _mm256_hadd_pd(vavbr, vavbi));
    }
}

static inline void simd_mul_pd(double *zc, const double *za, const double *zb, const int len)
{
    __m256d va, vb, arar, aiai;
    for (size_t i = 0; i < len; i += FSIMD_SIZE) {
        va = _mm256_load_pd(za + i);
        vb = _mm256_load_pd(zb + i);
        arar = _mm256_shuffle_pd(va, va, 0b0000);
        aiai = _mm256_shuffle_pd(va, va, 0b1111);
        // arar = _mm256_permute_pd(va, 0b0000);
        // arar = _mm256_permute_pd(va, 0b1111);
        arar = _mm256_mul_pd(arar, vb);
        aiai = _mm256_mul_pd(aiai, vb);
        aiai = _mm256_shuffle_pd(aiai, aiai, EO_SWITCH_256);
        // aiai = _mm256_permute_pd(aiai, EO_SWITCH_256);
        _mm256_store_pd(zc + i, _mm256_addsub_pd(arar, aiai));
    }
}

#ifdef AVX512
/**
 * @brief method 02 rrii gather scatter
 * 
 * @param zc 
 * @param za 
 * @param zb 
 * @param len 
 */
static inline void simd_mul_pd_rrii(double *zc, double *za, double *zb, const int len)
{
    __m128i idxr = _mm_set_epi32(0, 2, 4, 6);
    // __m128i idxi = _mm_set_epi32(1, 3, 5, 7);
    __m256d ar, ai, br, bi, abr, abi;
    for (size_t i = 0; i < len; i += 2 * FSIMD_SIZE) {
        ar = _mm256_i32gather_pd(za + i, idxr, 8);
        ai = _mm256_i32gather_pd(za + i + 1, idxr, 8);
        br = _mm256_i32gather_pd(zb + i, idxr, 8);
        bi = _mm256_i32gather_pd(zb + i + 1, idxr, 8);
        abr = _mm256_sub_pd(_mm256_mul_pd(ar, br), _mm256_mul_pd(ai, bi));
        abi = _mm256_add_pd(_mm256_mul_pd(ar, bi), _mm256_mul_pd(ai, br));
        _mm256_i32scatter_pd(zc + i, idxr, abr, 8);
        _mm256_i32scatter_pd(zc + i + 1, idxr, abi, 8);
    }
}
#endif

/**
 * @brief method 03 rrii load v0, v1;shuffle...
 * 
 * @param zc 
 * @param za 
 * @param zb 
 * @param len 
 */
static inline void simd_mul_pd_rrii2(double *zc, double *za, double *zb, const int len)
{
    __m256d v0, v1, ar, ai, br, bi, abr, abi;
    for (size_t i = 0; i < len; i += 2 * FSIMD_SIZE) {
        double *pa = za + i;
        double *pb = zb + i;
        // ar = _mm256_setr_pd(pa[0], pa[4], pa[2], pa[6]);
        // ai = _mm256_setr_pd(pa[1], pa[5], pa[3], pa[7]);
        // br = _mm256_setr_pd(pb[0], pb[4], pb[2], pb[6]);
        // bi = _mm256_setr_pd(pb[1], pb[5], pb[3], pb[7]);

        v0 = _mm256_load_pd(za + i);              // 0 1 2 3
        v1 = _mm256_load_pd(za + i + FSIMD_SIZE); // 4 5 6 7
        ar = _mm256_shuffle_pd(v0, v1, 0b0000);   // 0 4 2 6
        ai = _mm256_shuffle_pd(v0, v1, 0b1111);   // 1 5 3 7
        v0 = _mm256_load_pd(zb + i);
        v1 = _mm256_load_pd(zb + i + FSIMD_SIZE);
        br = _mm256_shuffle_pd(v0, v1, 0b0000);
        bi = _mm256_shuffle_pd(v0, v1, 0b1111);

        abr = _mm256_sub_pd(_mm256_mul_pd(ar, br), _mm256_mul_pd(ai, bi)); // 0 4 2 6
        abi = _mm256_add_pd(_mm256_mul_pd(ar, bi), _mm256_mul_pd(ai, br)); // 1 5 3 7

        _mm256_store_pd(zc + i, _mm256_shuffle_pd(abr, abi, 0b0000));              // 0 1 2 3
        _mm256_store_pd(zc + i + FSIMD_SIZE, _mm256_shuffle_pd(abr, abi, 0b1111)); // 4 5 6 7
    }
}

/* SINGLE_PREC */

void simd_mul_c(float *Fc, const float *Fa, const float *Fb, const int len)
{
    __m256 CsNegSign = _mm256_set_ps(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    for (size_t i = 0; i < len; i += FSIMD_SIZE) {
        __m256 va = _mm256_load_ps(Fa + i);
        __m256 vb = _mm256_load_ps(Fb + i);
        __m256 vavbr = _mm256_mul_ps(va, vb);
        __m256 vavbi = _mm256_shuffle_ps(vb, vb, 0b10110001);
        vavbr = _mm256_mul_ps(vavbr, CsNegSign);
        vavbi = _mm256_mul_ps(va, vavbi);
        va = _mm256_hadd_ps(vavbr, vavbi);
        _mm256_store_ps(Fc + i, _mm256_shuffle_ps(va, va, 0b11011000));
    }
}

void simd_mul_ps(float *cc, const float *ca, const float *cb, const int len)
{
    for (size_t i = 0; i < len; i += FSIMD_SIZE) {
        __m256 va = _mm256_load_ps(ca + i);
        __m256 vb = _mm256_load_ps(cb + i);
        __m256 arar = _mm256_moveldup_ps(va);
        __m256 aiai = _mm256_movehdup_ps(va);
        arar = _mm256_mul_ps(arar, vb);
        aiai = _mm256_mul_ps(aiai, vb);
        aiai = _mm256_permute_ps(aiai, EO_SWITCH_256);
        // aiai = _mm256_shuffle_ps(aiai, aiai, EO_SWITCH_256);
        arar = _mm256_addsub_ps(arar, aiai);
        _mm256_store_ps(cc + i, arar);
    }
}

#ifdef AVX512
static inline void simd_mul_ps_rrii(float *zc, float *za, float *zb, const int len)
{
    __m256i idxr = _mm256_set_epi32(0, 2, 4, 6, 8, 10, 12, 14);
    __m256i idxi = _mm256_set_epi32(1, 3, 5, 7, 9, 11, 13, 15);
    __m256 ar, ai, br, bi, abr, abi;
    for (size_t i = 0; i < len; i += 2 * FSIMD_SIZE) {
        ar = _mm256_i32gather_ps(za + i, idxr, 4);
        ai = _mm256_i32gather_ps(za + i, idxi, 4);
        br = _mm256_i32gather_ps(zb + i, idxr, 4);
        bi = _mm256_i32gather_ps(zb + i, idxi, 4);
        abr = _mm256_sub_ps(_mm256_mul_ps(ar, br), _mm256_mul_ps(ai, bi));
        abi = _mm256_add_ps(_mm256_mul_ps(ar, bi), _mm256_mul_ps(ai, br));
        _mm256_i32scatter_ps(zc + i, idxr, abr, 4);
        _mm256_i32scatter_ps(zc + i, idxi, abi, 4);
    }
}
#endif

#ifdef SINGLE_PREC
#ifdef AVX512
static inline void simd_mul_ps_rrii2(float *zc, float *za, float *zb, const int len)
{
    __m256 v0, v1, ar, ai, br, bi, abr, abi;
    for (size_t i = 0; i < len; i += 2 * FSIMD_SIZE) {
        // float *pa = za + i;
        // float *pb = zb + i;
        // ar = _mm256_setr_pd(pa[0], pa[4], pa[2], pa[6]);
        // ai = _mm256_setr_pd(pa[1], pa[5], pa[3], pa[7]);
        // br = _mm256_setr_pd(pb[0], pb[4], pb[2], pb[6]);
        // bi = _mm256_setr_pd(pb[1], pb[5], pb[3], pb[7]);
        // abr = _mm256_sub_ps(_mm256_mul_ps(ar, br), _mm256_mul_ps(ai, bi));
        // abi = _mm256_add_ps(_mm256_mul_ps(ar, bi), _mm256_mul_ps(ai, br));

        v0 = _mm256_load_ps(za + i);                  // 0 1 2 3 4 5 6 7
        v1 = _mm256_load_ps(za + i + FSIMD_SIZE);     // 8 9 10 11 12 13 14 15
        abr = _mm256_shuffle_f32x4(v0, v1, 0b00);     // 0 1 2 3 8 9 10 11
        abi = _mm256_shuffle_f32x4(v0, v1, 0b11);     // 4 5 6 7 12 13 14 15
        ar = _mm256_shuffle_ps(abr, abi, 0b10001000); // 0 2 8 10 4 6 12 14
        ai = _mm256_shuffle_ps(abr, abi, 0b11011101); // 1 3 9 11 5 7 13 15

        // float *f = (float *) &v0;
        // printf(" va0: %f %f %f %f %f %f %f %f\n", f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);
        // f = (float *) &v1;        
        // printf(" va1: %f %f %f %f %f %f %f %f\n", f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);
        // f = (float *) &abr;
        // printf(" abr: %f %f %f %f %f %f %f %f\n", f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);
        // f = (float *) &abi;
        // printf(" abi: %f %f %f %f %f %f %f %f\n", f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);
        // f = (float *) &ar;
        // printf("  ar: %f %f %f %f %f %f %f %f\n", f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);
        // f = (float *) &ai;
        // printf("  ai: %f %f %f %f %f %f %f %f\n", f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);

        v0 = _mm256_load_ps(zb + i);
        v1 = _mm256_load_ps(zb + i + FSIMD_SIZE);
        abr = _mm256_shuffle_f32x4(v0, v1, 0b00);                          // 0 1 2 3 8 9 10 11
        abi = _mm256_shuffle_f32x4(v0, v1, 0b11);                          // 4 5 6 7 12 13 14 15
        br = _mm256_shuffle_ps(abr, abi, 0b10001000);                      // 0 2 8 10 4 6 12 14
        bi = _mm256_shuffle_ps(abr, abi, 0b11011101);                      // 1 3 9 11 5 7 13 15
        // f = (float *) &v0;
        // printf(" vb0: %f %f %f %f %f %f %f %f\n", f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);
        // f = (float *) &v1;
        // printf(" vb1: %f %f %f %f %f %f %f %f\n", f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);
        // f = (float *) &abr;
        // printf(" abr: %f %f %f %f %f %f %f %f\n", f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);
        // f = (float *) &abi;
        // printf(" abi: %f %f %f %f %f %f %f %f\n", f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);
        // f = (float *) &br;
        // printf("  br: %f %f %f %f %f %f %f %f\n", f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);
        // f = (float *) &bi;
        // printf("  bi: %f %f %f %f %f %f %f %f\n", f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);
        
        abr = _mm256_sub_ps(_mm256_mul_ps(ar, br), _mm256_mul_ps(ai, bi)); // 0 2 8 10 4 6 12 14
        abi = _mm256_add_ps(_mm256_mul_ps(ar, bi), _mm256_mul_ps(ai, br)); // 1 3 9 11 5 7 13 15
        // f = (float *) &abr;
        // printf(" abr: %f %f %f %f %f %f %f %f\n", f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);
        // f = (float *) &abi;
        // printf(" abi: %f %f %f %f %f %f %f %f\n", f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);

        v0 = _mm256_unpacklo_ps(abr, abi);
        v1 = _mm256_unpackhi_ps(abr, abi);
        // f = (float *) &v0;
        // printf(" va0: %f %f %f %f %f %f %f %f\n", f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);
        // f = (float *) &v1;
        // printf(" va1: %f %f %f %f %f %f %f %f\n", f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);
        _mm256_store_ps(zc + i, _mm256_shuffle_f32x4(v0, v1, 0b00)); // 0 1 2 3 4 5 6 7
        _mm256_store_ps(zc + i + FSIMD_SIZE,
                        _mm256_shuffle_f32x4(v0, v1, 0b11)); // 8 9 10 11 12 13 14 15

    }
}
#endif
#endif

#endif

// double *f = (double *) &va;
// printf(" va: %lf %lf %lf %lf\n", f[0], f[1], f[2], f[3]);

// float *f = (float *) &vb;
// printf(" vb: %f %f %f %f %f %f %f %f\n", f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);
// f = (float *) &vb;
// printf(" vb: %f %f %f %f %f %f %f %f\n", f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);
