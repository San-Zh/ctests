#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 100

void aXpY(double a, double *X, double *Y, size_t size)
{
    for (size_t i = 0; i < size; i++) {
        Y[i] = a * X[i] + Y[i];
    }
}

void (*ptr_Op_aXY)(double, double *, double *, size_t);

void Op_aXY(double a, double *X, double *Y, size_t size,
            void (*ptr_Op_aXY)(double, double *, double *, size_t))
{
    ptr_Op_aXY = aXpY;
    ptr_Op_aXY(a, X, Y, size);
}

int main()
{
    double c = 0.05;
    double A[SIZE], B[SIZE], C[SIZE];
    for (size_t i = 0; i < SIZE; i++) {
        A[i] = (double) i;
        B[i] = (double) (SIZE - i);
        C[i] = (double) i;
    }

    printf("before func_ptr\n");
    ptr_Op_aXY = &aXpY;
    (*ptr_Op_aXY)(c, A, B, SIZE);

    Op_aXY(c, B, C, SIZE, aXpY);

    for (size_t i = 0; i < SIZE; i++) {
        printf("%lf\t%lf\t%lf\t%lf\n", c, A[i], B[i],C[i]);
    }

    return 0;
}