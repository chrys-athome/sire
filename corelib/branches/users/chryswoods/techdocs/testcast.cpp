
#include <iostream>

#include <boost/shared_ptr.hpp>

using namespace std;

class Base
{
public:
    Base()
    {}
    
    ~Base()
    {}
    
    virtual void print() const=0;
};

typedef boost::shared_ptr<Base> BasePtr;

class B : public Base
{
public:
    B(int val, const char *desc) : Base(), v(val), d(desc)
    {}
    
    ~B()
    {}
    
    void print() const
    {
        cout << "B: v = " << v << " and description = \"" << d << "\"\n";
    }

private:
    int v;
    const char *d;
};

class A
{
public:
    A(int val) : v(val)
    {}
    
    ~A()
    {}
  
    operator BasePtr() const
    {
        return BasePtr(new B(v, "A pretending to be B"));
    }
    
private:

    int v;
};

void printBase(const BasePtr &baseptr)
{
    baseptr->print();
}

int main(void)
{
    A a(3);
    
    BasePtr b(new B(3,"real B"));
    
    printBase(b);
    printBase(a);

    return 0;
}
