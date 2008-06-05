
#include <iostream>

using namespace std;

class A
{
public:
    A(double val) : v(val)
    {}
    
    ~A()
    {}
    
    void print()
    {
        cout << " A = " << v << endl;
    }
    
    template<class T>
    friend T operator-(const T&);
    
private:
    double v;
};

template<class T>
T operator-(const T &a)
{
    T ret( -a.v );
    return ret;
}

int main(void)
{
    A a(4.0);
    a.print();
    
    A b = -a;
    b.print();
    
    return 0;
}

