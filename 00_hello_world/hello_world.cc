#include <iostream>
#include <climits>
#include <time.h>

using namespace std;

#define FOR12(e)                                                                                   \
    {                                                                                              \
        e e e e e e e e e e e e                                                                    \
    }

template <int N>
using ARRAY = int[N];

template <int N>
void array(ARRAY<N> &in)
{
    cout << "sizeof(in) = " << sizeof(in[N]) << endl;
    for (size_t i = 0; i < sizeof(in) / sizeof(int); i++) {
        cout << "a[" << i << "]" << in[i] << endl;
    }
}

int main(int argc, char **argv)
{
    cout << "hello world!" << endl;
    cout << INT_MAX << endl;
    FOR12(cout << "12 hello" << endl;);
    
    double A = 0.987654321;
    float B = 0.123456789;
    double FtoD = B; 
    float DtoF = A;
    printf("DtoF %.10f\n",DtoF);
    printf("FtoD %.10f\n",FtoD);
    cout << "double A = " << A << "   float B " << B << endl;
    cout << "float DtoF = " << DtoF << "   double  FtoD " <<  FtoD << endl;
    
    cout << "\narray[]\n" << endl;
    ARRAY<4> a = {1, 2, 3, 4};
    cout << "sizeof(a) = " << sizeof(a) << endl;
    array(a);

    char str[50];
    time_t now = time(NULL);
    strftime(str, 50, "%Y%m%d.%H%M%S  %X", localtime(&now));
    cout << str << endl;

    double pi = 3.141592653 / 10000;
    printf("pi double %16.8f   sci %16.8e \n", pi,pi);

    return 0;
}
