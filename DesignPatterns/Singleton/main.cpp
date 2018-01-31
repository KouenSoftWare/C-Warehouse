#include <iostream>
#include <string>
#include "singleton.h"
using namespace std;

struct A{
    A(const string&){cout << "lvaue" << endl;}
    A(string&& x){cout << "rvaue" << x << endl;}
};

struct B{
    B(const string&){cout << "lvaue" << endl;}
    B(string&& x){cout << "rvaue" << x << endl;}
};

struct C{
    C(int x, double y){ cout << "C" << x << y << endl;}
    void Fun(){cout << "test" << endl;}
};

int main()
{
    auto str = "bb";
    Singleton<A>::Instance(str);
    Singleton<B>::Instance(std::move(str));
    Singleton<C>::Instance(1, 3.14);
    Singleton<C>::GetInstance()->Fun();

    Singleton<A>::DestroyInstance();

    return 0;
}
