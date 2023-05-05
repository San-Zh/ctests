#include <iostream>
#include <stdlib.h>
#include <sys/time.h>
#include <complex>
#include <string.h>

#define SIZE 1024 * 1024

using DataTp = double;
using namespace std;

struct timeval start;
struct timeval timer_end;

#define timer(time_ms, code)                                                                       \
    gettimeofday(&start, NULL);                                                                    \
    code;                                                                                          \
    gettimeofday(&timer_end, NULL);                                                                \
    time_ms = (timer_end.tv_sec - start.tv_sec) * 1000000 + (timer_end.tv_usec - start.tv_usec);

int main(int argc, char **argv)
{
    DataTp *A = (DataTp *) malloc(SIZE * sizeof(DataTp));
    DataTp *B = (DataTp *) malloc(SIZE * sizeof(DataTp));
    DataTp *C = (DataTp *) malloc(SIZE * sizeof(DataTp));
    size_t *idx = (size_t *) malloc(SIZE * sizeof(size_t));

    memset(A, 0, SIZE * sizeof(DataTp));
    memset(B, 0, SIZE * sizeof(DataTp));
    memset(B, 0, SIZE * sizeof(DataTp));

    for (size_t i = 0; i < SIZE; i++) {
        idx[i] = (i % 2) * SIZE / 2 + i / 2;
    }

    DataTp invRandMax = 1.0 / RAND_MAX;
    for (size_t loop = 0; loop < 100; loop++) {
        for (size_t i = 0; i < SIZE; i++) {
            A[i] = invRandMax * (DataTp) rand();
        }
        for (size_t i = 0; i < SIZE; i++) {
            B[i] = A[idx[i]];
        }
        for (size_t i = 0; i < SIZE; i++) {
            A[i] = invRandMax * (DataTp) rand();
        }
        for (size_t i = 0; i < SIZE; i++) {
            int eoidx = (i % 2) * SIZE / 2 + i / 2;
            B[i] = A[eoidx];
        }
    }

    double time0;
    double time1;

    {
        for (size_t i = 0; i < SIZE; i++) {
            A[i] = invRandMax * (DataTp) rand();
        }
        timer(time0, {
            for (size_t i = 0; i < SIZE; i++) {
                int eoidx = (i % 2) * SIZE / 2 + i / 2;
                B[i] = A[eoidx];
            }
        });
    }
    {
        for (size_t i = 0; i < SIZE; i++) {
            A[i] = invRandMax * (DataTp) rand();
        }
        timer(time1, {
            for (size_t i = 0; i < SIZE; i++) {
                B[i] = A[idx[i]];
            }
        });
    }

    cout << "time0 = " << time0 << ",  time1 = " << time1 << endl;
    free(A);
    free(B);
    free(idx);
}