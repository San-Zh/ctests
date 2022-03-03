/**
 * @file complex_vector.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-01-22
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <iostream>
#include <stdlib.h>
#include <sys/time.h>
#include <complex>
#include <climits>
#include <cstring>
#include <iomanip>

using namespace std;
typedef complex<double> Complex;

double norm2(double *x, int Size);
double norm2(Complex *x, int Size);

double vmul_complex(Complex *const &A, Complex *const &x, int SIZE);
double vmul_Vec2D(double *const &B, double *const &x, int SIZE);
double vmul_2VecD(double *const &B, double *const &x, int SIZE);
double vmul_2VecDTmp(double *const &B, double *const &x, int SIZE);

int main(int argc, char **argv)
{
    int SIZE = 0;
    if (argc > 1)
    {
        SIZE = atoi(argv[1]);
    }
    else
    {
        SIZE = 100;
    }
    // cout << "Size:\t" << SIZE << endl;
    // cout << " SIZE = " << SIZE << "  SIZE MAX = " << INT_MAX << endl;
    // cout << "SIZE < INT_MAX ? " << (SIZE < INT_MAX ? "YES" : "NO") << endl;
    double *A = new double[SIZE * 2];
    double *B = A;
    Complex *C = (Complex*) A;
    double *Im = B + SIZE;
    for (int i = 0; i < SIZE; i++)
    {
        A[2 * i] = i;
        A[2 * i + 1] = -i;
    }

    double *xA = new double[2 * SIZE];
    double *xB = xA; // new double[2 * SIZE];
    Complex *xC = (Complex *) xA; //new Complex[SIZE];
    for (int i = 0; i < SIZE; i++)
    {
        xC[i] = Complex((SIZE - i), (i - SIZE));
        xA[2 * i] = i;
        // xA[2 * i + 1] = -i;
        // xB[i] = (double)(SIZE - i);
        // xB[SIZE + i] = (double)(i - SIZE);
    }

    double timeuse = 0;
    struct timeval start;
    struct timeval end;

    double tdiff[4] = {0};
    double ratio[4] = {0};
    int LOOP = 10;
    for (int loop = 0; loop < 5; loop++)
    {
        tdiff[0] += vmul_complex(C, xC, SIZE);
        tdiff[1] += vmul_Vec2D(A, xA, SIZE);
        tdiff[2] += vmul_2VecD(B, xB, SIZE);
        tdiff[3] += vmul_2VecDTmp(B, xB, SIZE);
    }
    double divLoop = 1.0 / LOOP;
    for (int i = 0; i < 4; i++)
    {
        tdiff[i] *= divLoop;
        ratio[i] = tdiff[i] / tdiff[0];
    }
    cout << SIZE << "\t";
    cout << tdiff[0] << "\t" << tdiff[1] << "\t" << tdiff[2] << "\t" << tdiff[3] << "\t";
    cout << ratio[0] << "\t" << ratio[1] << "\t" << ratio[2] << "\t" << ratio[3] << endl;

    delete[] xA;
    // delete[] xB;
    // delete[] xC;
    delete[] A;
    // delete[] B;
    // delete[] C;

    return 0;
}

double vmul_complex(Complex *const &C, Complex *const &x, int SIZE)
{
    double timeuse = 0;
    struct timeval start;
    struct timeval end;
    gettimeofday(&start, NULL);
    for (int i = 0; i < SIZE; i++)
    {
        C[i] *= x[i];
    }
    gettimeofday(&end, NULL);
    timeuse = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
    return timeuse;
}

double vmul_Vec2D(double *const &A, double *const &x, int SIZE)
{
    double timeuse = 0;
    struct timeval start;
    struct timeval end;
    gettimeofday(&start, NULL);
    double re = 0;
    double im = 0;
    for (int i = 0; i < 2 * SIZE; i += 2)
    {
        re = x[i] * A[i] - x[i + 1] * A[i + 1];
        im = x[i] * A[i + 1] + x[i + 1] * A[i];
        A[i] = re;
        A[i + 1] = im;
    }
    gettimeofday(&end, NULL);
    timeuse = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
    return timeuse;
}

double vmul_2VecD(double *const &B, double *const &x, int SIZE)
{
    double timeuse = 0;
    struct timeval start;
    struct timeval end;
    gettimeofday(&start, NULL);
    double re = 0;
    double im = 0;
    double *Bim = B + SIZE;
    double *xim = x + SIZE;
    double xtmp_re, xtmp_im, ytmp_re, ytmp_im;
    for (int i = 0; i < SIZE; i++)
    {
    	xtmp_re = x[i];
    	xtmp_im = xim[i];
    	ytmp_re = B[i];
    	ytmp_im = Bim[i];
        re = xtmp_re * ytmp_re - xtmp_im * ytmp_im;
        im = xtmp_re * xtmp_im + ytmp_re * ytmp_im;
        B[i] = re;
        Bim[i] = im;
    }
    gettimeofday(&end, NULL);
    timeuse = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
    return timeuse;
}

double vmul_2VecDTmp(double *const &B, double *const &x, int SIZE)
{
    double timeuse = 0;
    struct timeval start;
    struct timeval end;
    gettimeofday(&start, NULL);
    double *tmp = new double[SIZE * 2];
    double *tmpim = tmp + SIZE;
    double *Bim = B + SIZE;
    double *xim = x + SIZE;
    for (int i = 0; i < SIZE; i++)
    {
        tmp[i] = x[i] * B[i];
    }
    for (int i = 0; i < SIZE; i++)
    {
        tmpim[i] = xim[i] * Bim[i];
    }
    for (int i = 0; i < SIZE; i++)
    {
        tmp[i] -= tmpim[i];
    }

    for (int i = 0; i < SIZE; i++)
    {
        tmpim[i] = x[i] * Bim[i];
    }
    for (int i = 0; i < SIZE; i++)
    {
        Bim[i] = xim[i] * B[i];
    }
    for (int i = 0; i < SIZE; i++)
    {
        Bim[i] += tmpim[i];
    }

    for (int i = 0; i < SIZE; i++)
    {
        B[i] = tmp[i];
    }
    delete[] tmp;
    gettimeofday(&end, NULL);
    timeuse = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
    return timeuse;
}

double norm2(double *x, int Size)
{
    double nrm = 0;
    for (int i = 0; i < 2 * Size; i++)
    {
        nrm += x[i] * x[i];
    }
    return nrm;
}

double norm2(Complex *x, int Size)
{
    double nrm = 0;
    for (int i = 0; i < Size; i++)
    {
        nrm += x[i].real() * x[i].real() + x[i].imag() * x[i].imag();
    }
    return nrm;
}
