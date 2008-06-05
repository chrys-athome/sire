
#include <iostream>

using namespace std;

class A
{
public:
    A(int val=42) : i(val)
    {}
    
    
    ~A()
    {}

    A constGet() const
    {
        return A(*this);
    }
    
    A get()
    {
        return A(*this);
    }
    
    A weird()
    {
        return *this;
    }
    
    A constWeird() const
    {
        return *this;
    }
    
    A& operator=(const A &a)
    {
        cout << "Const operator=\n";
        i = a.i;
        return *this;
    }
    
    A& operator=(A &a)
    {
        cout << "operator=\n";
        i = a.i;
        return *this;
    }

    int i;
};

A tester()
{
    return A();
}


int main(void)
{
    cout << "hello\n";
    A a = tester();
    
    cout << "hello 2\n";
    A a2 = a.get();
    
    cout << "hello 3\n";
    a2 = a.constGet();

    cout << "hello 4\n";
    a2 = a.weird();

    cout << "hello 5\n";
    a2 = a.constWeird();

    cout << "constructor\n";
    
    A a3 = a.weird();

    cout << "copy\n";
    
    a2 = a3;

    return 0;
}
