#include <iostream>
#include <climits>

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

    cout << "\narray[]\n" << endl;
    ARRAY<4> a = {1, 2, 3, 4};
    cout << "sizeof(a) = " << sizeof(a) << endl;
    array(a);
    return 0;
}
