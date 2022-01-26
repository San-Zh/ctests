#include <exception>
#include <iostream>
using namespace std;
/**
 * @brief class MyException2 
 * 
 */
class MyException2 {
  public:
    MyException2(const char *message) : messages_(message) { cout << "MyException2 ... " << endl; }
    MyException2(const MyException2 &other) : messages_(other.messages_)
    {
        cout << "Copy MyException2 ..." << endl;
    }
    virtual ~MyException2() { cout << "~MyException2()" << endl; }

    const char *what() const { return messages_.c_str(); }

  private:
    string messages_;
};

class MyExceptionD : public MyException2 {
  private:
  public:
    MyExceptionD(const char *messages) : MyException2(messages)
    {
        cout << "MyExceptionD ..." << endl;
    }
    MyExceptionD(const MyExceptionD &other) : MyException2(other)
    {
        cout << "Copy MyExceptionD ..." << endl;
    }
    ~MyExceptionD() { cout << "~MyExceptionD ..." << endl; }
};

void func(int n) throw(int, MyException2, MyExceptionD)
{
    if (n == 1) {
        throw 1;
    } else if (n == 2) {
        throw MyException2("test Exception2");
    } else if (n == 3) {
        throw MyExceptionD("test ExceptionD");
    }
}

void func2() throw()
{
}

int main(int argc, char const *argv[])
{
    try {
        func(2);
        cout << "catch dfa" << endl;
        func(1);
        func(3);
    } catch (int n) {
        cout << "catch int ..." << endl;
        cout << "n = " << n << endl;
    } catch (MyExceptionD &e) {
        cout << "catch MyExceptionD ..." << endl;
        cout << e.what() << endl;
    } catch (MyException2 &e) {
        cout << "catch MyException2 ..." << endl;
        cout << e.what() << endl;
    }
    return 0;
}
