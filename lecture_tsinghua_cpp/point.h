#include <iostream>
#include <string>
using namespace std;

/**
 * @brief Class point
 *
 */
class Point
{
public:
    Point(int initx=0, int inity=0)
    {
        x = initx;
        y = inity;
    }

    Point(Point &p)
    {
        x = p.x;
        y = p.y;
        cout << "Copy Constructor" << endl;
    }

    int getX() { return x; }
    int getY() { return y; }

private:
    int x, y;
};