/**
 * @file cmatrix.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-02-28
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

/**
 * @brief 
 * 
 * @tparam T 
 * @param y 
 * @param A 
 * @param x 
 * @param Col 
 * @param Row 
 * @param GridSize 
 */
template <typename T>
void CTG_gemv(T *y, T *A, T *x, const int &Col, const int &Row, const int &GridSize)
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
