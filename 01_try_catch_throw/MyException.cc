#include <exception>
#include <iostream>
using namespace std;

struct MyException : public exception {
    const char *what() const throw() { return "C++ MyException"; }
};

int main()
{
    try {
        throw MyException();
    } catch (const exception &e) {
        cout << "My Exception  Caught" << std::endl;
        cerr << e.what() << '\n';
    } catch (std::exception &e) {
        cout << "其他错误" << endl;
    }
}
