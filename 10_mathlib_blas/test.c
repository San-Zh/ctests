#include <stdio.h>
#include "cblas.h"

int main()
{
        const int dim=2;
        double a[4]={1.0,1.0,1.0,1.0},b[4]={2.0,2.0,2.0,2.0},c[4];
        int m=dim,n=dim,k=dim,lda=dim,ldb=dim,ldc=dim;
        double al=1.0,be=0.0;
        cblas_dgemm(101,111,111,m,n,k,al,a,lda,b,ldb,be,c,ldc);
        printf("the matrix c is:%f,%f\n%f,%f\n",c[0],c[1],c[2],c[3]);
        return 0;
}
