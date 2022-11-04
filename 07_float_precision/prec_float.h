#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <complex.h>

#define ComplexD complex<double>
#define ComplexF complex<float>

double norm2D(double *x, int Size)
{
    double nrm = 0.0;
    for (int i = 0; i < Size; i++) {
        nrm += x[i] * x[i];
    }
    return nrm;
}

float norm2F(float *x, int Size)
{
    float nrm = 0.0;
    for (int i = 0; i < Size; i++) {
        float xp = x[i] * x[i];
        nrm += xp;
    }
    return nrm;
}

// double norm2CD(ComplexD *x, int Size)
// {
//     double nrm = 0;
//     for (int i = 0; i < Size; i++) {
//         nrm += x[i].real() * x[i].real() + x[i].imag() * x[i].imag();
//     }
//     return nrm;
// }

// float norm2CF(ComplexF *x, int Size)
// {
//     double nrm = 0;
//     for (int i = 0; i < Size; i++) {
//         nrm += x[i].real() * x[i].real() + x[i].imag() * x[i].imag();
//     }
//     return nrm;
// }