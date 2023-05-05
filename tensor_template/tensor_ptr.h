/**
 * @file tensor_ptr.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-04-27
 * 
 * @copyright Copyright (c) 2023
 * 
 */

// #define NDEBUG
#include <assert.h>

template <class T, int Ndim>
class tensor_ptr {
  public:
    typedef T *PtrType;
    tensor_ptr() { assert(Ndim >= 0); }
    tensor_ptr(const PtrType *ov)
    {
        assert(Ndim >= 0);
        for (int i = 0; i < Ndim; i++) {
            ptr[i] = ov[i];
        }
    }
    tensor_ptr(T **ov, int Nvec) : ptr(ov) { assert(Ndim == Nvec); }
    ~tensor_ptr() {}

    PtrType &operator[](int i) { return this->ptr[i]; }

  private:
    PtrType ptr[Ndim];
};

template <typename T, int N>
struct tensor_traits {
    typedef typename T::Type type[N];
};

// template <typename T, int N>
template <int N>
struct tensor_traits<double, N> {
    typedef double type[N];
    typedef double *pointer;
};

// // template <int N>
// template <typename T, int N>
// struct tensor_traits<float, N> {
//     typedef float[N] type;
// };

// // template <int N>
// template <typename T, int N>
//
//     typedef int[N] type;
// };

template <typename T, int N>
class tesnor {
  public:
    typedef typename tensor_traits<T, N>::type Type;
    auto operator[](int n) { return ary[n]; }
    pointer data(){return static_cast<pointer> ary};
    Type ary;
};
