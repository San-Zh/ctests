#include <iostream>
#include <complex>
#include <climits>
#include <omp.h>

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
    cout << "hello world!" << endl;
    omp_set_num_threads(Num_Threads);

    cout << " max_threads = " << omp_get_max_threads()  << "  max_proc =" << omp_get_num_procs() << "  " << omp_get_proc_bind() << endl;
    int tid, mcpu;
    int sum = 0;
#pragma omp parallel num_threads(Num_Threads) private(tid, sum, mcpu)
    {
        mcpu = omp_get_num_threads();
        tid = omp_get_thread_num();
        for (int i = 0; i < 10; i++) {
            sum += i;
        }
        sum += tid;
        printf(" Tid: %02d/%02d, sum = %d\n", tid, mcpu, sum);
    }

    int LEN = 100000000;
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
    OMP_PARALLEL({
        ostringstream oss;
        int thr = omp_get_thread_num();
        oss << "\nthread_num = " << thr << ";  res = " << res << "  " << omp_get_proc_bind()
            << endl;
        cout << oss.str();
    })
    return 0;
}
