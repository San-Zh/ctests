#include <iostream>

using namespace std;

struct stC {
    int st0 = 7;
    double st1 = 0.8;
};

void voidptr(void **p)
{
    int *a = (int *) (p[0]);
    double *b = (double *) (p[1]);
    cout << a[0] << ", " << a[1] << ", " << a[2] << ", " << a[3] << ", " << a[4] << endl;
    cout << b[0] << ", " << b[1] << ", " << b[2] << ", " << b[3] << ", " << b[4] << endl;
    stC *c = (stC *) p[2];
    cout << c->st0 << ", " << c->st1 << endl;
    // double *c = (double *) p[3];
    // cout << c[0] << ", " << c[1] << endl;
}

int main(int argc, char **argv)
{

    stC C;
    int A[5] = {1, 2, 3, 4, 5};
    double B[5] = {0.1, 0.2, 0.3, 0.4, 0.5};
    void *ptr[3] = {A, B, &C};
    voidptr(ptr);
}