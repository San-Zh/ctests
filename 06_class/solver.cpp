
#include "solver.h"

void zgemm(double *des, double *src)
{
    printf("zgemm(): Y=AX\n");
}

int CGSolver(double *des, double *src, Func_ptr_Ax Ax, const CGParams &CGParams)
{
    Ax(des, src);
    printf("CG solver done\n");
    return EXIT_SUCCESS;
}

int BiCGSolver(double *des, double *src, Func_ptr_Ax Ax, const BiCGParams &BiCGParams)
{
    Ax(des, src);
    printf("BiCG solver done\n");
    return EXIT_SUCCESS;
}