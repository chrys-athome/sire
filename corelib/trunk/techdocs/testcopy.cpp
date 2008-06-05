
#include <iostream>

using namespace std;

class A
{

public:
    A(int i=42) : val(i)
    {}
    
    A(const A &other) : val(other.val)
    {
        cout << "A(const A&other)\n";
    }
    
    A(A &other) : val(other.val)
    {
        cout << "A(A &other)\n";
    }
    
    A& operator=(A other)
    {
        val = other.val;
        cout << "operator=(A)\n";
        return *this;
    }

private:
    int val;
};

A makeA()
{
    return A(35);
}

int main(void)
{

    A a;
    
    cout << "line44\n";
    a = A(42);

    cout << "line47\n";
    a = makeA();

    cout << "line50\n";
    a = A(a);

    cout << "line53\n";
    const A b(95);
    a = b;

    cout << "line57\n";
    A c( *(&a) );

    return 0;
}
