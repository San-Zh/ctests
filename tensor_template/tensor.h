/**
 * @file tensor.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-05-05
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once

// #define NDEBUG
// #include <assert.h>

template <typename T, int N>
struct tensor_traits {
    typedef typename T::Type type[N];
};

// template <typename T, int N>
template <int N>
struct tensor_traits<double, N> {
    typedef double type[N];
    // typedef double *pointer;
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
    typedef Type *pointer;
    auto operator[](int n) { return ary[n]; }
    static constexpr pointer S_ptr(const Type &_t) { return const_cast<pointer>(_t); }
    // static constexpr pointer data() { return S_ptr(ary); }
    // pointer data(){return static_cast<pointer> ary};
    Type ary;
};

///////

template <typename T, int N>
struct Vector {
    static constexpr size_t size() noexcept { return N; }
    typedef T type[N];
};

template <typename T, int N>
struct TensorN {
};

template <int N>
struct TensorN<Vector<...>, N> {
};
