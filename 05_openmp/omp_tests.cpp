#include <iostream>
#include <complex>
#include <omp.h>

#define DO_PRAGMA(x) _DO_PRAGMA_(x)
#define _DO_PRAGMA_(x) _Pragma(#x)
// #define DO_PRAGMA(x) _Pragma(#x)
#define OMP_FOR_(Nthreads) DO_PRAGMA(omp parallel for num_thread(Nthreads))
#define OMP_FOR DO_PRAGMA(omp parallel for)

using namespace std;
typedef complex<double> Complex_d;

int N = 100;

void omp_for();
void omp_private(Complex_d *a, Complex_d *b, Complex_d *c, int threads);
void check(Complex_d *a, Complex_d *b, Complex_d *c);

int main(int argc, char *argv[])
{
    Complex_d a[N], b[N], c[N];
    int thread_count = strtol(argv[1], NULL, 10);
    // #pragma omp parallel for num_threads(thread_count)
    OMP_FOR_(thread_count)
    // OMP_FOR
    for (int i = 0; i < N; i++) {
        a[i] = Complex_d(i, -i);
        b[i] = Complex_d(N - i, i - N);
    }

    omp_private(a, b, c, thread_count);

    check(a, b, c);

    return 0;
}

void omp_private(Complex_d *a, Complex_d *b, Complex_d *c, int threads)
{
    Complex_d tmp;
#pragma omp parallel for num_threads(threads) private(tmp)
    // #pragma omp private(tmp) /// Wrong
    for (int i = 0; i < N; i++) {
        tmp = a[i];
        c[i].real(tmp.imag());
        c[i].imag(tmp.real());
    }
}

void check(Complex_d *a, Complex_d *b, Complex_d *c)
{
    Complex_d tmp;
    Complex_d d[N];
    for (int i = 0; i < N; i++) {
        tmp = a[i];
        d[i].real(tmp.imag());
        d[i].imag(tmp.real());

        if (d[i] != c[i])
            cout << " Wrong, i=" << i << " " << d[i] << " " << c[i] << endl;
    }
}
