#pragma once
// #include <immintrin.h>
#include <x86intrin.h>
#include <complex>
#include <stdio.h>

#define SIMD256
#define ALIGEN_BTS 32

#ifdef SINGLE_PREC
#define FLOAT float
#else
#define FLOAT double
#endif
using Complex = std::complex<FLOAT>;

#ifdef SIMD256

#if defined SINGLE_PREC
#define SimdVecF __m256
#define FSIMD_SIZE 8
#define EO_SWITCH_256 0b10110001
#define Fsimd_load(maddr) _mm256_load_ps((float *) maddr)
#define Fsimd_store(maddr, vecf) _mm256_store_ps((float *) maddr, vecf)
#define Fsimd_unpacklo(v0, v1) _mm256_unpacklo_ps(v0, v1)
#define Fsimd_unpackhi(v0, v1) _mm256_unpackhi_ps(v0, v1)
#define SimdGatherIndex __m256i
#define Fsimd_i32gather(maddr, m256i, scale) _mm256_i32gather_ps(maddr, m256i, scale)
#define Fsimd_i32scatter(maddr, m256i, vec, scale) _mm256_i32scatter_ps(maddr, m256i, vec, scale)
#define Fsimd_fmadd(v0, v1, v2) _mm256_fmadd_ps(v0, v1, v2)
#define Fsimd_mul(v0, v1) _mm256_mul_ps(v0, v1)
#define Fsimd_setzero() _mm256_setzero_ps();
#else
#define EO_SWITCH_256 0b0101
#define SimdVecF __m256d
#define FSIMD_SIZE 4
#define Fsimd_load(maddr) _mm256_load_pd((double *) maddr)
#define Fsimd_store(maddr, vecf) _mm256_store_pd((double *) maddr, vecf)
#define Fsimd_unpacklo(v0, v1) _mm256_unpacklo_pd(v0, v1)
#define Fsimd_unpackhi(v0, v1) _mm256_unpackhi_pd(v0, v1)
#define SimdGatherIndex __m128i
#define Fsimd_i32gather(maddr, m128i, scale) _mm256_i32gather_pd(maddr, m128i, scale)
#define Fsimd_i32scatter(maddr, m128i, vec, scale) _mm256_i32scatter_pd(maddr, m128i, vec, scale)
#define Fsimd_fmadd(v0, v1, v2) _mm256_fmadd_pd(v0, v1, v2)
#define Fsimd_mul(v0, v1) _mm256_mul_pd(v0, v1)
#define Fsimd_setzero() _mm256_setzero_pd();
#endif

#endif

/**
 * @brief nomal y = A x;
 * 
 * @tparam T 
 * @param y des vector
 * @param A Matrix, Row major;
 * @param x src vector, x[i * Row + row]
 * @param Col Max num of column index;
 * @param Row Max num of row index;
 * @param GridSize Grid Size
 */
template <typename T>
void gemv(T *y, T *A, T *x, const int &Col, const int &Row, const int &GridSize)
{
    for (size_t i = 0; i < GridSize; i++) {
        for (size_t c = 0; c < Col; c++) {
            for (size_t r = 0; r < Row; r++) {
                y[i * Col + c] += A[i * Col * Row + c * Row + r] * x[i * Row + r];
            }
        }
    }
}

void simdGemv(FLOAT *y, FLOAT *A, FLOAT *x, const int &Col, const int &Row, const int &GridSize)
{
    SimdGatherIndex midx;
#ifdef SINGLE_PREC
    midx = _mm256_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7);
    const int Scale = sizeof(FLOAT);
#else
    midx = _mm_setr_epi32(0, 1, 2, 3);
    const int Scale = sizeof(FLOAT);
#endif

    SimdVecF acr, vr, vc;
    for (size_t i = 0; i < GridSize; i += FSIMD_SIZE) {
        for (size_t c = 0; c < Col; c++) {
            vc = Fsimd_setzero();
            for (size_t r = 0; r < Row; r++) {
                acr = Fsimd_i32gather((A + i * Col * Row + c * Row + r), midx * Col * Row, Scale);
                vr = Fsimd_i32gather((x + i * Row + r), midx * Row, Scale);
                vc = Fsimd_fmadd(acr, vr, vc);
            }
            Fsimd_i32scatter((y + i * Col + c), midx * Col, vc, Scale); // -mavx512vl
        }
    }

    // SimdVecF acr, vr[Row], vc; // vc[Col];
    // for (size_t i = 0; i < GridSize; i += FSIMD_SIZE) {
    //     for (size_t r = 0; r < Row; r++) {
    //         vr[r] = Fsimd_i32gather((x + i * Row + r), midx * Row, Scale);
    //     }
    //     for (size_t c = 0; c < Col; c++) {
    //         // acr = Fsimd_i32gather((A + i * Col * Row + c * Row + 0), midx * Col * Row, Scale);
    //         // vr[0] = Fsimd_i32gather((x + i * Row + 0), midx * Row, Scale);
    //         // // vc[c] = _mm256_mul_pd(acr, vr[0]);
    //         // vc = _mm256_mul_pd(acr, vr[0]);
    //         vc = Fsimd_setzero();
    //         for (size_t r = 0; r < Row; r++) {
    //             // a[c][r] = Fsimd_i32gather(Mi + c * Row + r, midx * Col * Row, FSIMD_SIZE);
    //             acr = Fsimd_i32gather((A + i * Col * Row + c * Row + r), midx * Col * Row,
    //                                   Scale);
    //             vr[r] = Fsimd_i32gather((x + i * Row + r), midx * Row, Scale);
    //             // vc[c] = Fsimd_fmadd(acr, vr[r], vc[c]);
    //             vc = Fsimd_fmadd(acr, vr[r], vc);
    //         }
    //         Fsimd_i32scatter((y + i * Col + c), midx * Col, vc, Scale); // -mavx512vl
    //     }
    //     // for (size_t c = 0; c < Col; c++) {
    //     //     Fsimd_i32scatter((y + i * Col + c), midx * Col, vc[c], Scale); // -mavx512vl
    //     // }
    // }
}

/**
 * @brief Tensor Grid gemv
 * 
 * @tparam T basic data type: complex, double, float, int 
 * @param y y[col * GridSize + i], i=[0:GridSize); initialized result vector
 * @param A A[(col * Row + raw) * GridSize + i]
 * @param x x[row * GridSize + i]
 * @param Col Tesnor col size
 * @param Row Tensor row size
 * @param GridSize 
 */
template <typename T>
void TG_gemv(T *y, T *A, T *x, const int &Col, const int &Row, const int &GridSize)
{
    T *Mcr[Col * Row], *xr[Row], *yc[Col];
    for (size_t c = 0; c < Col; c++) {
        for (size_t r = 0; r < Row; r++) {
            Mcr[c * Row + r] = A + GridSize * (c * Row + r);
        }
    }

    for (size_t r = 0; r < Row; r++) {
        xr[r] = x + GridSize * r;
    }

    for (size_t c = 0; c < Col; c++) {
        yc[c] = y + GridSize * c;
    }
    for (size_t c = 0; c < Col; c++) {
        for (size_t r = 0; r < Row; r++) {
            for (size_t i = 0; i < GridSize; i++) {
                yc[c][i] += Mcr[c * Row + r][i] * xr[r][i];
            }
        }
    }
}

template <typename T>
void TG_simdGemv(T *y, T *A, T *x, const int &Col, const int &Row, const int &GridSize)
{
    T *Acr[Col * Row], *xr[Row], *yc[Col];
    for (size_t c = 0; c < Col; c++) {
        for (size_t r = 0; r < Row; r++) {
            Acr[c * Row + r] = A + GridSize * (c * Row + r);
        }
    }

    for (size_t r = 0; r < Row; r++) {
        xr[r] = x + GridSize * r;
    }

    for (size_t c = 0; c < Col; c++) {
        yc[c] = y + GridSize * c;
    }
    SimdVecF vx[Row], acr;
    for (size_t i = 0; i < GridSize; i += FSIMD_SIZE) {
        for (size_t r = 0; r < Row; r++) {
            vx[r] = Fsimd_load(xr[r] + i);
        }
        for (size_t c = 0; c < Col; c++) {
            SimdVecF vy = Fsimd_setzero();
            for (size_t r = 0; r < Row; r++) {
                acr = Fsimd_load(Acr[c * Row + r] + i);
                vy = Fsimd_fmadd(acr, vx[r], vy);
            }
            Fsimd_store(yc[c] + i, vy);
        }
    }
}

/**
 * @brief Tensor Grid gemv method2: 
 * 
 * @tparam T basic data type: complex, double, float, int 
 * @param y y[col * GridSize + i], i=[0:GridSize); initialized result vector
 * @param A A[(col * Row + raw) * GridSize + i]
 * @param x x[row * GridSize + i]
 * @param Col Tesnor col size
 * @param Row Tensor row size
 * @param GridSize 
 */
template <typename T>
void TG_gemv01(T *y, T *A, T *x, const int &Col, const int &Row, const int &GridSize)
{
    T *Mcr[Col * Row], *xr[Row], *yc[Col];
    for (size_t c = 0; c < Col; c++) {
        for (size_t r = 0; r < Row; r++) {
            Mcr[c * Row + r] = A + GridSize * (c * Row + r);
        }
    }

    for (size_t r = 0; r < Row; r++) {
        xr[r] = x + GridSize * r;
    }

    for (size_t c = 0; c < Col; c++) {
        yc[c] = y + GridSize * c;
    }

    for (size_t i = 0; i < GridSize; i++) {
        for (size_t c = 0; c < Col; c++) {
            for (size_t r = 0; r < Row; r++) {
                yc[c][i] += Mcr[c * Row + r][i] * xr[r][i];
            }
        }
    }
}

template <typename T>
void CTG_gemv1(T *y, T *A, T *x, const int &Col, const int &Row, const int &GridSize)
{
    T *Mcr[Col * Row], *xr[Row], *yc[Col];
    for (size_t c = 0; c < Col; c++) {
        for (size_t r = 0; r < Row; r++) {
            Mcr[c * Row + r] = A + GridSize * (c * Row + r);
        }
    }

    for (size_t r = 0; r < Row; r++) {
        xr[r] = x + GridSize * r;
    }

    for (size_t c = 0; c < Col; c++) {
        yc[c] = y + GridSize * c;
    }

    for (size_t i = 0; i < GridSize; i++) {
        for (size_t c = 0; c < Col; c++) {
            for (size_t r = 0; r < Row; r++) {
                yc[c][i] += Mcr[c * Row + r][i] * xr[r][i];
            }
        }
    }
}

// {
//     for (size_t i = 0; i < GridSize; i++) {
//         for (size_t c = 0; c < Col; c++) {
//             for (size_t r = 0; r < Row; r++) {
//                 printf("%lf  ", A[i * Col * Row + c * Row + r]);
//             }
//             printf("\n");
//         }
//         printf("\n");
//     }
//     for (size_t i = 0; i < GridSize; i++) {
//         for (size_t r = 0; r < Row; r++) {
//             printf("%lf ", x[i * Row + r]);
//         }
//         printf(" | ");
//     }
//     printf("\n\n");
// }