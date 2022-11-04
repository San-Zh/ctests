/**
 * @file strcut.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-07-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <iostream>
#include <cstdlib>

using namespace std;

struct Sample_t {
    double x;
    double y;
    double z = x + y;
};

void endian()
{
    union {
        char c[2];
        unsigned char b;
    } a = {'0', '1'};
#define ENDIAN ((char) a.b)
    if (a.b == '0')
        cout << "Little Endian" << endl;
    else
        cout << "Little Endian" << endl;

    cout << a.c[0] << a.c[1] << endl;
}

int main(int argc, char **argv)
{
    Sample_t st;
    st.x = 1;
    st.y = 2;
    cout << "x = " << st.x << endl;
    cout << "y = " << st.y << endl;
    cout << "z = " << st.z << endl;

    st.z = 5;
    cout << "z = " << st.z << endl;

    endian();

    return 0;
}