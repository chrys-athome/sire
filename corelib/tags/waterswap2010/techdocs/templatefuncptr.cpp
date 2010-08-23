
#include <iostream>
#include <boost/function.hpp>

using namespace std;
using namespace boost;

template<class T>
class Caller
{
public:
    Caller(T *obj)
    {}
    
    ~Caller()
    {}
    
    void call(function3<int, T*, int, int> func, int val1, int val2)
    {
        cout << func(obj,val1,val2) << endl;
    }
        
protected:
    T *obj;
};

class Calc
{
public:
    Calc()
    {}
    
    ~Calc()
    {}
    
    int multiply(int val1, int val2)
    {
        return val1 * val2;
    }
    
    int add(int val1, int val2)
    {
        return val1 + val2;
    }
    
};

int main(void)
{

    Caller<Calc> calculator(new Calc());
    
    calculator.call( &Calc::multiply, 3, 5 );
    calculator.call( &Calc::add, 2, 6 );
    calculator.call( &Calc::multiply, 7, 10 );

    return 0;
}
