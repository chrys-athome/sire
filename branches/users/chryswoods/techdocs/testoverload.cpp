
#include <iostream>

using namespace std;

class TestBase
{
public:
    TestBase()
    {
        cout << "TestBase() this = " << (int)this << endl;
    }
    
    virtual ~TestBase()
    {}
    
    virtual void* clone(const void *original) const=0;
};

class Test : public TestBase
{
public:
    Test()
    {
            cout << "Test() this = " << (int)this << endl;
    }
    
    ~Test()
    {}
    
    void* clone(const void *original) const
    {
        cout << "void clone\n";
        if (original)
            return new double( *(static_cast<const double*>(original)) );
        else
            return 0;
    }
    
    double* clone(const double *original) const
    {
        cout << "double* clone\n";
        if (original)
            return new double(*original);
        else
            return 0;
    }
};

int main(void)
{
    double orig = 2.0;

    Test t;
    
    TestBase *tb = &t;

    double *d = t.clone(&orig);
    
    double *d2 = static_cast<double*>(tb->clone(&orig));
    
    double *d3 = static_cast<double*>(tb->clone(0));
    
    double *d4 = t.clone( (double*)0 );
    
    cout << *d << " " << *d2 << " " << d3 << " " << d4 << endl; 
    

    return 0;
}
