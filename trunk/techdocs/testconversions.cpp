
#include <iostream>

using namespace std;

class A
{
public:
    A(int val=1) : v(val)
    {}
    
    ~A()
    {}

    int v;
};

class B
{
public:
    B(double val=3.0) : v(val)
    {}
    
    ~B()
    {}

    double v;
};

class C
{
public:
    C(const A &aval, const B &bval) : a(aval), b(bval)
    {}
    ~C()
    {}
    
    void print()
    {
        cout << "A == " << a.v << " B == " << b.v << endl;
    }
    
private:
    A a;
    B b;
};

class E
{
public:
    explicit E(double val=3.0) : v(val)
    {}
    
    ~E()
    {}

    double v;
};

class F
{
public:
    F(const A &aval, const E &bval) : a(aval), b(bval)
    {}
    ~F()
    {}
    
    void print()
    {
        cout << "A == " << a.v << " B == " << b.v << endl;
    }
    
private:
    A a;
    E b;
};

int main(void)
{
    //test implicit conversion for multiple constructor arguments...
    C c(1, 4.5);
    
    c.print();

    c = C('1', 4.5); //this works, but is not what we want...
    c.print();
    
    //explicit version of B (called E) does not allow implicit conversion
    //F f(1, 4.5); <- this causes a compiler error
    
    F f(1, E(4.5)); // <- this doesn't
    f.print();
}

