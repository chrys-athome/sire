
#include <iostream>

using namespace std;

class A
{
public:
    A()
    {
        cout << "Constructing an A (" << (int)this << ")\n";
    }
    
    A(const A &other)
    {
        cout << "Copy constructor (" << (int)this << ")\n";
    }

    A(int)
    {
        cout << "int copy constructor (" << (int)this << ")\n";
    }
    
    ~A()
    {
        cout << "A destructor (" << (int)this << ")\n";
    }
 
};

int main(void)
{
    A a;
    
    cout << "About to copy...\n";
    a = A();
    cout << "Copy complete!\n";
    
    
    cout << "Copy an int\n";
    a = 3;
    cout << "Copy complete!\n";
    
    return 0;

}
