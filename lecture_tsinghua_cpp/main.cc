#include <iostream>
#include <string>
#include "clock.h"
#include "point.h"
using namespace std;

int main()
{
    Clock myClock(0, 0, 0);
    myClock.setTime(16, 55, 55);
    myClock.showTime();
    return 0;
}