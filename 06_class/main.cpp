/**
 * @brief 
 * 
 * @param des 
 * @param src 
 */
#include "paramsclass.h"
#include "solver.h"
#include <string>
#include <map>
#include <typeinfo>

using std::cout;
using std::endl;
using std::map;

int main(int argc, int **argv)
{
    // std::map<std::string, Func_ptr_solver> SolverMap;
    // SolverMap["CGSolver"] = CGSolver;

    CGParams *P;
    P = new CGParams();

    cout << typeid(P).name() << " " << P->MaxIter << endl;

    double *src, *des;
    Solver(des, src, zgemm, *P, CGSolver);

    printf("Done\n");

    return EXIT_SUCCESS;
}