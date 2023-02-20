#ifndef _COMPLEX_MUL_SIMD_H
#define _COMPLEX_MUL_SIMD_H

// #include <immintrin.h>
#include <x86intrin.h>
#include <stdio.h>

#define SIMD256

#ifdef SIMD256
#define ALIGEN_BTS 32
#define SimdVecFloat __m256
#define SimdVecDouble __m256d
#define SimdSizeFloat 8
#define SimdSizeDouble 4
#define EO_SWITCH_256D 0b0101
#define EO_SWITCH_256F 0b10110001
#endif

#ifdef SIMD128
#define SimdVecFloat __mm128
#define SimdVecDouble __mm128d
#define SimdSizeFloat 4
#define SimdSizeDouble 2
#endif

static inline void simd_mul_pd(double *zc, const double *za, const double *zb, const int len)
{
    __m256d CdNegSign = _mm256_set_pd(-1.0, 1.0, -1.0, 1.0);
    for (size_t i = 0; i < len; i += SimdSizeDouble) {
        __m256d va = _mm256_load_pd(za + i);
        __m256d vb = _mm256_load_pd(zb + i);
        __m256d vec3 = _mm256_mul_pd(va, vb);
        vb = _mm256_permute_pd(vb, EO_SWITCH_256D);
        vb = _mm256_mul_pd(vb, CdNegSign);
        __m256d vec4 = _mm256_mul_pd(va, vb);
        va = _mm256_hsub_pd(vec3, vec4);
        _mm256_store_pd(zc + i, va);
    }
}

void simd_mul_ps(float *cc, const float *ca, const float *cb, const int len)
{
    for (size_t i = 0; i < len; i += SimdSizeFloat) {
        __m256 va = _mm256_load_ps(ca + i);
        __m256 vb = _mm256_load_ps(cb + i);
        // float *f = (float *) &va;
        // printf(" va: %f %f %f %f %f %f %f %f\n", f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);
        // f = (float *) &vb;
        // printf(" vb: %f %f %f %f %f %f %f %f\n", f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);

        // va = _mm256_permute_ps(va, EO_SWITCH_256F);
        // f = (float *) &va;
        // printf(" va: %f %f %f %f %f %f %f %f\n", f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);

        __m256 arar = _mm256_moveldup_ps(va);
        // f = (float *) &arar;
        // printf("var: %f %f %f %f %f %f %f %f\n", f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);

        __m256 aiai = _mm256_movehdup_ps(va);
        // f = (float *) &aiai;
        // printf("vai: %f %f %f %f %f %f %f %f\n", f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);

        arar = _mm256_mul_ps(arar, vb);
        // f = (float *) &arar;
        // printf("arvb %f %f %f %f %f %f %f %f\n", f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);

        vb = _mm256_permute_ps(vb, EO_SWITCH_256F);
        // f = (float *) &vb;
        // printf(" vb: %f %f %f %f %f %f %f %f\n", f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);

        aiai = _mm256_mul_ps(aiai, vb);
        // f = (float *) &aiai;
        // printf(" vb: %f %f %f %f %f %f %f %f\n", f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);

        arar = _mm256_addsub_ps(arar, aiai);
        // f = (float *) &arar;
        // printf(" vb: %f %f %f %f %f %f %f %f\n", f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);

        // arar = _mm256_fmaddsub_ps(aiai, vb, arar);
        _mm256_store_ps(cc + i, arar);
    }
}
// float *f = (float *) &vb;
// printf(" vb: %f %f %f %f %f %f %f %f\n", f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);

template <typename T>
void std_vec_mul(T *Vd, const T *Va, const T *Vb, const int len)
{
    for (size_t i = 0; i < len; i++) {
        Vd[i] = Va[i] * Vb[i];
    }
}

#endif