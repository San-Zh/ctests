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
    // std::map<std::string, Func_ptr_solver> SolverMap;
    // SolverMap["CGSolver"] = CGSolver;

    CGParams *P;

    P = new CGParams();
    double *src, *des;
    Solver<CGParams>(des, src, *P, zgemm, CGSolver);

    printf("Done\n");

    return EXIT_SUCCESS;
}