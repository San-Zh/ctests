#include <iostream>
#include <complex>
#include <climits>
#include <omp.h>
#include <mpi.h>

#ifndef Num_Threads
#define Num_Threads 8
#endif
using namespace std;

#define DO_PRAGMA(x) _Pragma(#x)
#define OMP_FOR(i, istart, iend) DO_PRAGMA(omp parallel for) for (int i = istart; i < iend; i++)
#define OMP_FOR_REDUCE_SUM(i, istart, iend, SUM) DO_PRAGMA(omp parallel for schedule(static) reduction(+ : ##SUM)) for (int i = istart; i < iend; i++)
#define OMP_PARALLEL(...)                                                                          \
    DO_PRAGMA(omp parallel) { __VA_ARGS__ }

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    omp_set_num_threads(16 / size);
    cout << " max_threads = " << omp_get_max_threads() << "  max_proc =" << omp_get_num_procs()
         << "  " << omp_get_proc_bind() << endl;

    double Tstart = MPI_Wtime();
    int LEN = 100000000 / size;
    complex<double> *cvA = new complex<double>[LEN];
    OMP_FOR(i, 0, LEN)
    {
        cvA[i] = complex<double>(i / (double) LEN + 0.05, -i / (double) LEN - 0.05);
    }
    OMP_FOR(i, 0, LEN) { cvA[i] = cvA[i] * conj(cvA[i]); }
    complex<double> res(0);
    OMP_FOR(i, 0, LEN) { cvA[i] = cvA[i] * conj(cvA[i]); }
    // OMP_FOR_REDUCE_SUM(i, 0, LEN, res) {}
    OMP_FOR(x, 0, LEN) { res += cvA[x]; }
    double Tend = MPI_Wtime();

    OMP_PARALLEL({
        ostringstream oss;
        int thr = omp_get_thread_num();
        oss << "rank = " << rank << ";  thread = " << thr << ";  time = " << Tend - Tstart << endl;
        cout << oss.str();
    })

    MPI_Finalize();
    return 0;
}
