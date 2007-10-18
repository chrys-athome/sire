
#include <iostream>

using namespace std;

class A
{
public:
    A(double val) : v(val)
    {}
    
    ~A()
    {}
    
    A& operator=(const A &other)
    {
        v = other.v;
        return *this;
    }
    
    double value() const
    {
        return v;
    }
    
private:
    double v;
};

class B
{
public:
    B(int val) : v(val)
    {}
    
    ~B()
    {}
    
    B& operator=(const B &other)
    {
        v = other.v;
        return *this;
    }
    
    int value() const
    {
        return v;
    }
    
private:
    int v;
};

class Multi : public A, public B
{
public:
    Multi() : A(0.0), B(0)
    {}
    
    Multi(double a, int b) : A(a), B(b)
    {}
    
    ~Multi()
    {}
    
    Multi& operator=(const A &a)
    {
        A::operator=(a);
        return *this;
    }
    
    Multi& operator=(const B &b)
    {
        B::operator=(b);
        return *this;
    }
    
    void print() const
    {
        cout << "A = " << A::value() << " B = " << B::value() << endl;
    }
};

int main(void)
{
    Multi m;
    m.print();  

    m = A(3.0);
    m.print();
    
    m = B(5);
    m.print();
  
    m = A(9.0);
    m.print();
  
    m = Multi(-3.0,-10);
    m.print();
  
    return 0;
}
