#ifndef _COMPLEX_MUL_SIMD_H
#define _COMPLEX_MUL_SIMD_H

// #include <immintrin.h>
#include <x86intrin.h>
#include <stdio.h>

#define SIMD256

#define BitsAligen 32

#ifdef SIMD256
#define SimdVecFloat __m256
#define SimdVecDouble __m256d
#define SimdSizeFloat 8
#define SimdSizeDouble 4
#endif

#ifdef SIMD128
#define SimdVecFloat __mm128
#define SimdVecDouble __mm128d
#define SimdSizeFloat 4
#define SimdSizeDouble 2
#endif

static inline void SIMD_mul_pd(double *zc, const double *za, const double *zb, const int len)
{
    for (size_t i = 0; i < len; i += SimdSizeDouble) {
        __m256d CdNegSign = _mm256_set_pd(-1.0, 1.0, -1.0, 1.0);
        __m256d vec1;
        __m256d vec2;
        __m256d vec3;
        __m256d vec4;
        vec1 = _mm256_load_pd(za + i);
        vec2 = _mm256_load_pd(zb + i);
        vec3 = _mm256_mul_pd(vec1, vec2);
        double *f = (double *) &vec2;
        vec2 = _mm256_permute_pd(vec2, 0b0101);
        vec2 = _mm256_mul_pd(vec2, CdNegSign);
        vec4 = _mm256_mul_pd(vec1, vec2);
        vec1 = _mm256_hsub_pd(vec3, vec4);
        _mm256_store_pd(zc + i, vec1);
    }
}

#define EO_SWITCH_256f 0b10110001

void SIMD_mul_ps(float *cc, const float *ca, const float *cb, const int len)
{
    for (size_t i = 0; i < len; i += SimdSizeFloat) {
        __m256 vec1 = _mm256_load_ps(ca + i);
        __m256 vec2 = _mm256_load_ps(cb + i);
        __m256 _arr = _mm256_moveldup_ps(vec1);
        __m256 _aii = _mm256_movehdup_ps(vec2);
        _aii = _mm256_mul_ps(_aii, _mm256_permute_ps(vec2, EO_SWITCH_256f));
        _arr = _mm256_fmaddsub_ps(_arr, vec1, _aii);
        _mm256_store_ps(cc + i, _arr);
    }
}

#endif