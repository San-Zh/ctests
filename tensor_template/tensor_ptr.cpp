/**
 * @file tensor_ptr.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-04-27
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <iostream>
#include <stdio.h>
#include <typeinfo>
#include "tensor_ptr.h"

using namespace std;

typedef tensor_ptr<tensor_ptr<tensor_ptr<double, 3>, 4>, 6> T643;
typedef tensor_ptr<double, 3> Vptr3;

int main(int argc, char **argv)
{
    int Vsize = 2;
    T643 A;
    double B[6][4][3][Vsize];
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            for (int k = 0; k < 6; k++) {
                for (int m = 0; m < Vsize; m++) {
                    B[i][j][k][m] = i * 1.0e-1 + j * 1.0e-2 + k * 1.0e-3 + m * 1.0e-4;
                }
            }
        }
    }
    double *C[6][4][3];

    cout << typeid(T643).name() << endl;

    // A[0][0][0] = C[0][0][0];

    double M[3][Vsize];
    Vptr3 V3;

    return 0;
}