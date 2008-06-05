
#include <iostream>

using namespace std;

template<class K, class V>
K getValue(const V &val);

class A
{

public:
    A() : d(0.0), i(0)
    {}
    
    A(double a, int b) : d(a), i(b)
    {}
    
    ~A()
    {}
    
    int intVal() const
    {
    	return i;
    }
    
    double doubleVal() const
    {
    	return d;
    }

private:

    double d;
    int i;
};

template<>
double getValue(const A &val)
{
    return val.doubleVal();
}

template<>
int getValue(const A &val)
{
    return val.intVal();
}


int main(void)
{
    A a1(6.7, 42);
    
    cout << "Double = " << getValue<double,A>(a1)
         << " Int = " << getValue<int,A>(a1) << endl;

    return 0;
}
