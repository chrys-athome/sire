
#include <iostream>

using namespace std;

class A
{
public:
    A(int v0, int v1) : val0(v0), val1(v1)
    {
        cout << "A( " << val0 << ", " << val1 << " )\n";
    }
    
    ~A()
    {}

    A& operator=(int val)
    {
        val0 = val;
        val1 = val;
        
        cout << "A& operator=( " << val << " )\n";
    }

    void print() const
    {
        cout << "val0 = " << val0 << " and val1 = " << val1 << endl;
    }

private:
    int val0,val1;
};

int main(void)
{
    cout << "A1\n";
    A a1(4);
    a1.print();
    
    cout << "A2\n";
    A a2 = 5;
    a2.print();
    
    return 0;
}
