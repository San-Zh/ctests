#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <complex>
#include <sys/time.h>

template <typename Tp>
void axb(Tp *x, Tp *y, int Size)
{
    Tp nrm = 0.0;
    for (int i = 0; i < Size; i++) {
        x[i] = x[i] * y[i];
    }
}

template <typename Tp>
Tp norm2(Tp *x, int Size)
{
    Tp nrm = 0.0;
    for (int i = 0; i < Size; i++) {
        nrm += x[i] * x[i];
    }
    return nrm;
}

// template <typename Tp>
// Tp Cosh(Tp *x, int Size)
// {
//     Tp nrm = 0.0;
//     for (int i = 0; i < Size; i++) {
//         nrm += x[i] * x[i];
//     }
//     return nrm;
// }

typedef float Stype;
typedef double Dtype;
typedef std::complex<float> ComplexF;
typedef std::complex<double> ComplexD;

int main(int argc, char *argv[])
{
    int SIZE, LOOP;
    if (argc > 1) {
        SIZE = atoi(argv[1]);
        // LOOP = atoi(argv[2]);
    } else {
        SIZE = 1000;
        // LOOP = 100;
    }

    // Dtype precision
    double timeuseD = 0.0;
    struct timeval startD;
    struct timeval endD;
    // single precision
    double timeuseF = 0.0;
    struct timeval startF;
    struct timeval endF;

    Dtype *A = (Dtype *) malloc(SIZE * 2 * sizeof(Dtype));
    Dtype *destA = (Dtype *) malloc(SIZE * 2 * sizeof(Dtype));
    Stype *AF = (Stype *) malloc(SIZE * 2 * sizeof(Stype));
    Stype *destF = (Stype *) malloc(SIZE * 2 * sizeof(Stype));

    srand(0);
    for (int i = 0; i < SIZE * 2; i++) {
        *(A + i) = (Dtype) rand() / (Dtype)(RAND_MAX);
        *(destA + i) = (Dtype) rand() / (Dtype)(RAND_MAX);
    }
    srand(0);
    for (int i = 0; i < SIZE * 2; i++) {
        *(AF + i) = (Stype)(rand()) / (RAND_MAX + 1.0);
        *(destF + i) = (Stype)(rand()) / (RAND_MAX + 1.0);
    }

    // norm2D(A, SIZE * 2);
    // norm2F(AF, SIZE * 2);
    gettimeofday(&startD, NULL);
    // axb((ComplexD *) A, (ComplexD *) destA, SIZE);
    axb(A, destA, SIZE * 2);
    // auto n2d = norm2(A, SIZE * 2);
    gettimeofday(&endD, NULL);
    timeuseD = (endD.tv_sec - startD.tv_sec) * 1000000 + (endD.tv_usec - startD.tv_usec);

    gettimeofday(&startF, NULL);
    // axb((ComplexF *) AF, (ComplexF *) destF, SIZE);
    axb(AF, destF, SIZE * 2);
    // auto n2f = norm2(AF, SIZE * 2);
    gettimeofday(&endF, NULL);
    timeuseF = (endF.tv_sec - startF.tv_sec) * 1000000 + (endF.tv_usec - startF.tv_usec);

    printf("time axbD time: %10.5f us\n", timeuseD);
    printf("time axbF time: %10.5f us, acc=%6.3lf\n\n", timeuseF, timeuseD / timeuseF);

    return 0;
}