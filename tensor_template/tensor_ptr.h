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
    ~tensor_ptr() {}

    PtrType operator[](int i) { return ptr[i]; }

  private:
    PtrType *ptr[Ndim];
};
