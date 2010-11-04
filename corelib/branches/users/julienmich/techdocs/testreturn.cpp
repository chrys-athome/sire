
#include <iostream>

using namespace std;

class A
{
public:

    A()
    {}
    
    ~A()
    {}
    
    int call(int i)
    {
        cout << "Calling int!\n";
        return i;
    }
    
    double call(double i)
    {
        cout << "Calling double\n";
        return i;
    }
    
    A call(A &i)
    {
        cout << "Calling A\n";
        return i;
    }
    
    int call()
    {
        cout << "Unspecified call...\n";
        return 0;
    }
    
//    double call()
//    {
//        cout << "Not allowed overload on return value!\n";
//        return 0.0;
//    }
    
};

int main(void)
{
    int i = 3;
    
    A a;
    
    i = a.call(i);
    
    double d = 4.0;
    
    d = a.call(d);
    
    a = a.call(a);

    i = a.call();

    return 0;
}
