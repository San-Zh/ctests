#pragma once

#include "paramsclass.h"

using Func_ptr_Ax = void (*)(double *, double *);

template <typename ParamsType>
using Func_ptr_solver = int (*)(double *, double *, ParamsType const &params, Func_ptr_Ax Ax);

// template <typename ParamsType>
// class Solver {
//   private:
//     SolverParams *_param;

//   public:
//     using funcptr = int (*)(double *, double *, ParamsType *params, Func_ptr_Ax Ax);
//     Solver() = default;
//     ~Solver() {}
// };

void zgemm(double *des, double *src)
{
    printf("zgemm(): Y=AX\n");
}

int CGSolver(double *des, double *src, CGParams const &CGParams, Func_ptr_Ax Ax)
{
    (*Ax)(des, src);
    printf("CG solver done\n");
    return EXIT_SUCCESS;
}

int BiCGSolver(double *des, double *src, BiCGParams const &BiCGParams, Func_ptr_Ax Ax)
{
    (*Ax)(des, src);
    printf("BiCG solver done\n");
    return EXIT_SUCCESS;
}

// template <typename ParamType>
// int Solver(double *des, double *src, ParamType const &Params, Func_ptr_solver SolverMethod)
// {
//     (*SolverMethod)(des, src, Params, Ax);
//     printf("Solver API\n");
//     return EXIT_SUCCESS;
// }

template <typename ParamsType>
int Solver(double *src, double *des, ParamsType const &Params, Func_ptr_Ax Ax,
           Func_ptr_solver<ParamsType> SolverMethod)
{
    SolverMethod(des, src, Params, Ax);
    printf("Solver API\n");
    return EXIT_SUCCESS;
}
