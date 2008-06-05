
#include <iostream>
using namespace std;

class A
{
public:
    A(int v)
    {
        if (v == 0)
            return;
            
        cout << v << " is not 0!\n";
    }
    
    ~A()
    {}
};

int main(void)
{

    A a1(1);
    A a3(3);
    A a0(0);
    A a2(2);

    return 0;
}
