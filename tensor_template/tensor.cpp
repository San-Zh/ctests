#include <iostream>
#include <stdio.h>
#include <typeinfo>
#include <array>
#include "tensor.h"

using namespace std;

int main(int argc, char **argv)
{
    int Vsize = 2;

    double A[3][4];
    cout << "\ndouble A[3][4]" << endl;
    cout << "  typeid(A          ).name()  " << typeid(A).name() << endl;
    cout << "  typeid(A[0]       ).name()  " << typeid(A[0]).name() << endl;
    cout << "  typeid(A[0][0]    ).name()  " << typeid(A[0][0]).name() << endl;
    cout << "  typeid(A[0][0]    ).name()  " << typeid(&A[0][0]).name() << endl;

    array<array<double, 4>, 3> T34;
    cout << "\narray<<d,4>,3>" << endl;
    cout << "  typeid(array<<d,4>,3>.ary    ).name()  " << typeid(T34).name() << endl;
    cout << "  typeid(array<<d,4>,3>[0]     ).name()  " << typeid(T34[0]).name() << endl;
    cout << "  typeid(array<<d,4>,3>[0][0]  ).name()  " << typeid(T34[0][0]).name() << endl;
    cout << "  typeid(array<<d,4>,3>.data() ).name()  " << typeid(T34.data()).name() << endl;

    typedef tesnor<tesnor<tesnor<double, 4>, 3>, 2> T234;
    cout << "\n  typeid(T234::Type        ).name()  " << typeid(T234::Type).name() << endl;
    cout << "  typeid(T234::Pointer     ).name()  " << typeid(T234::pointer).name() << endl;

    tesnor<tesnor<tesnor<double, 4>, 3>, 2> Ary;
    // tesnor<tesnor<double, 4>, 3> Ary;
    cout << "\ntesnor<tesnor<double, 4>, 3> Ary" << endl;
    cout << "  typeid(Ary               ).name()  " << typeid(Ary).name() << endl;
    cout << "  typeid(Ary[0]            ).name()  " << typeid(Ary[0]).name() << endl;
    cout << "  typeid(Ary[0][0]         ).name()  " << typeid(Ary[0][0]).name() << endl;
    cout << "  typeid(Ary[0][0][0]      ).name()  " << typeid(Ary[0][0][0]).name() << endl;
    cout << "  typeid(&Ary              ).name()  " << typeid(&Ary).name() << endl;
    // cout << "  typeid(&Ary[0]           ).name()  " << typeid(&Ary[0]).name() << endl;
    cout << "  typeid(&Ary[0][0]        ).name()  " << typeid(&Ary[0][0]).name() << endl;
    cout << "  typeid(&Ary[0][0][0]     ).name()  " << typeid(&Ary[0][0][0]).name() << endl;
    // cout << "  typeid(Ary.ary           ).name()  " << typeid(static_cast<double *> (Ary.ary)).name()
    //  << endl;
    cout << "  typeid(Ary.ary           ).name()  " << typeid(Ary.ary).name() << endl;
    cout << "  typeid(Ary.ary[0]        ).name()  " << typeid(Ary.ary[0]).name() << endl;
    cout << "  typeid(Ary.ary[0][0]     ).name()  " << typeid(Ary.ary[0][0]).name() << endl;
    cout << "  typeid(Ary.ary[0][0][0]  ).name()  " << typeid(Ary.ary[0][0][0]).name() << endl;
    // for (int i = 0; i < 2; i++)
    //     for (int j = 0; j < 3; j++)
    //         for (int k = 0; k < 4; k++)
    //             Ary[i][j][k] = rand() * 1.0 / INT64_MAX;

    // auto p = &Ary[0][0][0];
    // for (int i = 0; i < 2; i++)
    //     for (int j = 0; j < 3; j++)
    //         for (int k = 0; k < 4; k++)
    //             cout << p[i * 3 * 4 + j * 4 + k] << "\t" << Ary[i][j][k] << "\t"
    //                  << p[i * 3 * 4 + j * 4 + k] - Ary[i][j][k] << endl;

    return 0;
}