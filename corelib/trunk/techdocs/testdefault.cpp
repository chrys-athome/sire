
#include <iostream>

using namespace std;

class A
{
public:
    A()
    {}
    
    ~A()
    {}
    
    void printMe(const float &val = 5.0) const
    {
        cout << "Hello World (" << val << ")\n";
    }
};

int main(void)
{

    A a;
    
    a.printMe(42.5);
    a.printMe();
    
    return 0;

}
