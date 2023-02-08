#pragma once

#include "paramsclass.h"

typedef void (*Func_ptr_Ax)(double *, double *);

typedef int (*Func_ptr_solver)(double *, double *, SolverParams *params, Func_ptr_Ax Ax);

void zgemm(double *des, double *src)
{
    printf("zgemm(): Y=AX\n");
}

int CGSolver(double *des, double *src, CGParams &CGParams, Func_ptr_Ax Ax)
{
    (*Ax)(des, src);
    printf("CG solver done\n");
    return EXIT_SUCCESS;
}

int BiCGSolver(double *des, double *src, BiCGParams &BiCGParams, Func_ptr_Ax Ax)
{
    (*Ax)(des, src);
    printf("CG solver done\n");
    return EXIT_SUCCESS;
}

// template <typename ParamType>
// int Solver(double *des, double *src, ParamType const &Params, Func_ptr_solver SolverMethod)
// {
//     (*SolverMethod)(des, src, Params, Ax);
//     printf("Solver API\n");
//     return EXIT_SUCCESS;
// }

int Solver(double *src, double *des, SolverParams *Params, Func_ptr_Ax Ax,
           Func_ptr_solver SolverMethod)
{
    (*SolverMethod)(des, src, Params, Ax);
    printf("Solver API\n");
    return EXIT_SUCCESS;
}
