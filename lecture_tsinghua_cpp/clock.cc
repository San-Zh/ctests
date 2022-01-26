#include "clock.h"
#include <iostream>
#include <string>
using namespace std;

void Clock::setTime(int newH, int newM, int newS)
{
    hour = newH;
    min = newM;
    sec = newS;
}

void Clock::showTime()
{
    cout << "Time is " << hour << ":" << min << ":" << sec << endl;
}

