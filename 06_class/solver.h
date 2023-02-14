#pragma once
#include "paramsclass.h"

using Func_ptr_Ax = void (*)(double *, double *);

void zgemm(double *des, double *src);

int CGSolver(double *des, double *src, Func_ptr_Ax Ax, const CGParams &CGParams);

int BiCGSolver(double *des, double *src, Func_ptr_Ax Ax, const BiCGParams &BiCGParams);

template <typename ParamsType>
using Func_ptr_solver = int (*)(double *, double *, Func_ptr_Ax Ax, const ParamsType &params);

template <typename ParamsType>
int Solver(double *src, double *des, Func_ptr_Ax Ax, ParamsType const &Params,
           Func_ptr_solver<ParamsType> SolverMethod)
{
    SolverMethod(des, src, Ax, Params);
    printf("Solver API\n");
    return EXIT_SUCCESS;
}
