
#include <iostream>
#include "template.h"

using namespace std;

int main(int argc, char **argv)
{
    stuOp<double> opd;
    stuOp<float> opf;
    cout << opd() << endl;
    cout << opf() << endl;
    // cout << stuOp<double>() << endl;

    boundary<x> b_x;
    boundary<y> b_y;
    boundary<z> b_z;
    boundary<t> b_t;
    // boundary<4> b_4;
    cout << "boundary<x> x_b; x_b.dim() " << b_x.dim() << endl;
    cout << "boundary<y> y_b; y_b.dim() " << b_y.dim() << endl;
    cout << "boundary<z> z_b; z_b.dim() " << b_z.dim() << endl;
    cout << "boundary<t> t_b; t_b.dim() " << b_t.dim() << endl;

    return 0;
}