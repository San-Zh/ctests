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

    double *TensorAry[6][4][3];
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 3; k++) {
                TensorAry[i][j][k] = new double[Vsize];
                for (int m = 0; m < Vsize; m++) {
                    TensorAry[i][j][k][m] = i * 1.0e-1 + j * 1.0e-2 + k * 1.0e-3 + m * 1.0e-4;
                }
            }
        }
    }

    cout << "\ntypeid(TensorAry),name()  " << endl;
    cout << "  typeid(TensorAry).name()  " << typeid(TensorAry).name() << endl;
    cout << "  typeid(TensorAry[0]).name()  " << typeid(TensorAry[0]).name() << endl;

    double VecAry[3][Vsize];
    for (int k = 0; k < 3; k++) {
        for (int m = 0; m < Vsize; m++) {
            VecAry[k][m] = k * 1.0e-3 + m * 1.0e-4;
        }
    }


    T643 A;
    cout << "\ntypeid(T643),name()  " << endl;
    cout << "  typeid(T643          ),name()  " << typeid(T643).name() << endl;
    cout << "  typeid(A[0]          ),name()  " << typeid(A[0]).name() << endl;
    cout << "  typeid(A[0][0]       ),name()  " << typeid(A[0][0]).name() << endl;
    cout << "  typeid(A[0][0][0]    ),name()  " << typeid(A[0][0][0]).name() << endl;
    cout << "  typeid(A[0][0][0][0] ),name()  " << typeid(A[0][0][0][0]).name() << endl;
    // cout << typeid(A).name() << endl;
    // cout << typeid(A[0]).name() << endl;
    // cout << typeid(A[0][0]).name() << endl;
    // cout << typeid(A[0][0][0]).name() << endl;

    Vptr3 V3((Vptr3::PtrType *) VecAry);
    cout << "\ntypeid(Vptr3),name()" << endl;
    cout << "  typeid(double        ).name()  " << typeid(double).name() << endl;
    cout << "  typeid(Vptr3         ).name()  " << typeid(Vptr3).name() << endl;
    cout << "  typeid(Vptr3::PtrType).name()  " << typeid(Vptr3::PtrType).name() << endl;
    cout << "  typeid(V3            ).name()  " << typeid(V3).name() << endl;
    cout << "  typeid(V3[0]         ).name()  " << typeid(V3[0]).name() << endl;

    cout << "V3 values" << endl;
    for (int k = 0; k < 3; k++) {
        for (int m = 0; m < Vsize; m++) {
            printf("  %10.4f", V3[k][m]);
        }
        printf("\n");
    }

    return 0;
}