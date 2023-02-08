/**
 * @brief 
 * 
 * @param des 
 * @param src 
 */
#include "paramsclass.h"
#include "solverptr.h"
#include <string>
#include <map>

int main(int argc, int **argv)
{
    std::map<std::string, Func_ptr_solver> SolverMap;
    SolverMap["CGSolver"] = CGSolver;

    SolverParams *P;

    P = new CGParams();
    double *src, *des;
    // Solver<zgemm>(des, src, cgpar, CGSolver);
    Solver(des, src, P, zgemm, &CGSolver);
    // Solver(des, src, cgpar, zgemm, BiCGSolver);

    printf("Done\n");

    return EXIT_SUCCESS;
}