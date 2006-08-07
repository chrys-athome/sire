
#include <iostream>

using namespace std;


class A
{
public:
    A()
    {}
    
    ~A()
    {}
    
    virtual void printMe()
    {
        cout << "Hello, I am A\n";
    }

    static A* create()
    {
        return new A();
    }
};

class B : public A
{
public:
    B() : A()
    {}
    
    ~B()
    {}
    
    void printMe()
    {
        cout << "Hello, I am B\n";
    }
    
    static A* create()
    {
        return new B();
    }
};

typedef A* (*createFunc)();

class Registry
{
public:
    Registry()
    {}
    
    ~Registry()
    {}
    
    void registerA( int id, createFunc func )
    {
        funcs[id] = func;
    }

    A* create(int id)
    {
        (*(funcs[id]))();
    }

private:
    
    createFunc funcs[10];
};    


int main(void)
{
    Registry registry;
    
    registry.registerA( 0, &A::create );
    registry.registerA( 1, &B::create );
    
    A* a = registry.create(0);
    A* b = registry.create(1);
    
    a->printMe();
    b->printMe();
    
    return 0;
}

