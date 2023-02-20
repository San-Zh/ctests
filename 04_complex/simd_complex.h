#ifndef _SIMD_COMPLEX_H
#define _SIMD_COMPLEX_H

#include <immintrin.h>

#define SIMD256

#define BitsAligen 32

#ifdef SIMD256
#define SimdVecFloat __m256
#define SimdVecDouble __m256d
#define SimdSizeFloat 8
#define SimdSizeDouble 4
#define Simd_mul_float(a, b) _mm256_mul_ps(a, b)
#define Simd_mul_double(a, b) _mm256_mul_pd(a, b)
#define Simd_fmadd_float(a, b, c) _mm256_fmadd_ps(a, b, c)
#define Simd_fmadd_double(a, b, c) _mm256_fmadd_pd(a, b, c)
#endif

#ifdef SIMD128
#define SimdVecFloat __mm128
#define SimdVecDouble __mm128d
#define SimdSizeFloat 4
#define SimdSizeDouble 2
#endif

SimdVecFloat CplNegSign = _mm256_set_ps(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
SimdVecDouble CdNegSign = _mm256_set_pd(-1.0, 1.0, -1.0, 1.0);

static inline SimdVecFloat Simd_mul_complex(SimdVecFloat a, SimdVecFloat b)
{
    SimdVecFloat _vec1 = Simd_mul_float(a, b);
    _vec1 = Simd_mul_float(_vec1, CplNegSign);
    SimdVecFloat _vec2 = _mm256_permute_ps(b, 0b10110001);
    _vec2 = Simd_mul_float(a, _vec2);
    return _vec2 = _mm256_hadd_ps(_vec1, _vec2);
}

static inline SimdVecDouble Simd_mul_complex(SimdVecDouble a, SimdVecDouble b)
{
    SimdVecDouble _vec1 = Simd_mul_double(a, b);
    _vec1 = Simd_mul_double(_vec1, CdNegSign);
    SimdVecDouble _vec2 = _mm256_permute_pd(b, 0b0101);
    _vec2 = Simd_mul_double(a, _vec2);
    return _mm256_hadd_pd(_vec1, _vec2);
}

static inline SimdVecFloat Simd_fmadd_complex(SimdVecFloat a, SimdVecFloat b, SimdVecFloat c)
{
    SimdVecFloat _vec1 = Simd_mul_float(a, b);
    _vec1 = Simd_mul_float(_vec1, CplNegSign);
    SimdVecFloat _vec2 = _mm256_permute_ps(b, 0b10110001);
    _vec2 = _mm256_hadd_ps(_vec1, _vec2);
    return _mm256_add_ps(_vec2, c);
}

#endif