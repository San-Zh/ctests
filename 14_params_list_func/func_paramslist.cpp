#include <stdio.h>
#include <list>

using namespace std;

template <typename T, int N>
class Vect {
  public:
    typedef T Dtype[N];
    typedef T Data;

    Vect(const Dtype &a)
    {
        for (int i = 0; i < N; i++) {
            _M[i] = a[i];
        }
    }

    Vect(Data *a)
    {
        for (int i = 0; i < N; i++) {
            _M[i] = a[i];
        }
    }

    // void params(list<T> a)
    // {
    //     for (int i = 0; i < N; i++) {
    //         printf("a[%d] = %d\n", i, a[i]);
    //     }
    // }

    void print()
    {
        for (int i = 0; i < N; i++) {
            printf("a[%d] = %d\n", i, _M[i]);
        }
    }

  private:
    Dtype _M;
};

/// @brief
/// @param argc
/// @param argv
/// @return
int main(int argc, char **argv)
{
    int a[3]{1, 2, 3};
    // params(a);

    Vect<int, 3> A(a);
    A.print();

    Vect<int, 3> B({4, 5, 6});
    // Vect<int, 2> A({4, 5, 6});
    B.print();

    return 0;
}