#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void hello();

int main(int argc, char *argv[])
{
    int thread_count = strtol(argv[1], NULL, 10);
#pragma omp parallel num_threads(thread_count)
    hello();
}

void hello()
{
    int thread_rank = omp_get_thread_num();
    int threads_count = omp_get_num_threads();
    printf("Hello from thread %d of %d \n", thread_rank, threads_count);
    // cout << "Hello from thred " << thread_rank << " of " << threads_count << endl;
}