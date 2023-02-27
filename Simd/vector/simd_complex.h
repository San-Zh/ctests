#ifndef _SIMD_COMPLEX_H
#define _SIMD_COMPLEX_H

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
#endif

#ifdef SIMD128
#define SimdVecFloat __mm128
#define SimdVecDouble __mm128d
#define SimdSizeFloat 4
#define SimdSizeDouble 2
#endif

void simd_mul_z(double *Fc, const double *Fa, const double *Fb, const int len)
{
    SimdVecDouble CdNegSign = _mm256_set_pd(-1.0, 1.0, -1.0, 1.0);
    for (size_t i = 0; i < len; i += SimdSizeDouble) {
        __m256d va = _mm256_load_pd(Fa + i);
        __m256d vb = _mm256_load_pd(Fb + i);
        __m256d vavbi = _mm256_mul_pd(va, _mm256_permute_pd(vb, 0b0101));
        __m256d vavbr = _mm256_mul_pd(_mm256_mul_pd(va, vb), CdNegSign);
        _mm256_store_pd(Fc + i, _mm256_hadd_pd(vavbr, vavbi));
    }
}

void simd_mul_c(float *Fc, const float *Fa, const float *Fb, const int len)
{
    SimdVecFloat CsNegSign = _mm256_set_ps(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    for (size_t i = 0; i < len; i += SimdSizeFloat) {
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

template <typename T>
void std_vec_mul(T *Vd, const T *Va, const T *Vb, const int len)
{
    for (size_t i = 0; i < len; i++) {
        Vd[i] = Va[i] * Vb[i];
    }
}

#endif