#include "complex_mul_simd.h"
#include <complex>

void stdComplexToReal2(FLOAT *des, const Complex *src, size_t clen)
{
    FLOAT *real = des;
    FLOAT *imag = des + clen;
    for (size_t i = 0; i < clen; i++) {
        real[i] = src[i].real();
        imag[i] = src[i].imag();
    }
}

void simdComplexToReal2(FLOAT *des, const Complex *csrc, size_t clen)
{
    FLOAT *real = des;
    FLOAT *imag = des + clen;
    FLOAT *src = (FLOAT *) src;
    SimdVecF v0, v1, vr, vi;
#ifdef SINGLE_PREC
    __m256i idxr = _mm256_set_epi32(0, 2, 4, 6, 8, 10, 12, 14);
    __m256i idxi = _mm256_set_epi32(1, 3, 5, 7, 9, 11, 13, 15);
    for (size_t i = 0; i < clen; i += 2 * FSIMD_SIZE) {
        vr = Fsimd_i32gather(src + i, idxr, 4);
        vi = _mm256_i32gather_ps(src + i, idxi, 4);
    }
#else
    for (size_t i = 0; i < clen; i += 2 * FSIMD_SIZE) {
        v0 = _mm256_load_pd(src);
        v1 = _mm256_load_pd(src + FSIMD_SIZE);
        SimdVecF v00 = _mm256_shuffle_f64x2(v0, v1, 0b00);
        SimdVecF v11 = _mm256_shuffle_f64x2(v0, v1, 0b11);
        vr = _mm256_shuffle_pd(v0, v1, 0b0000);
        vi = _mm256_shuffle_pd(v0, v1, 0b1111);
    }
    for (size_t i = 0; i < clen; i += 2 * FSIMD_SIZE) {
        __m128i idx = _mm_set_epi32(0, 2, 4, 6);
        vr = Fsimd_i32gather(src + i, idx, 8);
        vi = Fsimd_i32gather(src + 1, idx, 8);
    }
    _mm256_store_pd(real, vr);
    _mm256_store_pd(imag, vi);
#endif
}