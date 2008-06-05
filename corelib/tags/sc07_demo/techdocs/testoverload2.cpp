
#include <iostream>

using namespace std;

class A
{
public:
    A()
    {}
    
    ~A()
    {}
    
    int func() const
    {
        cout << "A::func()\n";
        return 42;
    }
};

class B : public A
{
public:
    B() : A()
    {}
    
    ~B()
    {}
    
    const char* func() const
    {
        cout << "B::func()\n";
        return "forty two";
    }
};

int main(void)
{
    A a;
    cout << a.func() << endl;
    
    B b;
    cout << b.func() << endl;
    
    cout << static_cast<const A&>(b).func() << endl;

    return 0;
}
