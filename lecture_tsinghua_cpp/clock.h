#include <iostream>
#include <string>
using namespace std;

class Clock
{
public:
    /// 构造函数
    Clock(int newH = 0, int newM = 0, int newS = 0)
    {
        hour = newH;
        min = newM;
        sec = newS;
    }
    /// 析构函数
    ~Clock() {}
    void setTime(int newH, int newM, int newS);
    void showTime();

private:
    int hour;
    int min;
    int sec;
};
