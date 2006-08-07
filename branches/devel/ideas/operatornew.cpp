
#include <memory>
#include <iostream>

using std::cout;

class TestObj
{
public:
    TestObj();
    ~TestObj();
    
    float a,b,c,d;
    
    static int total;
};

int TestObj::total=0;

TestObj::TestObj()
{
    TestObj::total += sizeof(this);
}

TestObj::~TestObj()
{
    TestObj::total -= sizeof(this);
}

int main(void)
{
    for (int i=0; i<10000000; i++)
    {
        TestObj *t = new TestObj();
        delete t;
    }
    
    cout << "TestObj size = " << TestObj::total << " units\n";
    
    return 0;
}

