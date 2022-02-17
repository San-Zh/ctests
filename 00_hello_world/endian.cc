#include <iostream>
#include <cstdlib>
#include <stdio.h>

using namespace std;

int main(int argc, char **argv)
{
    union {
        short s;
        char c[sizeof(short)];
    } un;
    un.s = 0x0102;
    char cc = 'c';
    cout << "un.s = " << un.s << " un.c[0,1] = " << un.c[0] << "," << un.c[1] << " cc = " << cc
         << endl;
    if (sizeof(short) == 2) {
        if (un.c[0] == 1 && un.c[1] == 2) {
            cout << "Big Endian" << endl;
        } else if (un.c[0] == 2 && un.c[1] == 1) {
            cout << "Little Endian" << endl;
        } else {
            cout << "Unkown Endian" << endl;
        }
    }

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    cout << "__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__" << endl;
#elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    cout << "__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__" << endl;
#endif
    return 0;
}