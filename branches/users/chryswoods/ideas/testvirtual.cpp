
#include <iostream>

using namespace std;

class A
{
public:
    A(){}
    
    virtual ~A(){}
    
    virtual void greet() const=0;
    
    void sayHello(A &obj);
};

class B : public A
{
public:

    B();
    virtual ~B(){}
    
    virtual void greet() const;
};

B::B() : A()
{}

void B::greet() const
{
    cout << "Hello, I am B()\n";
}

class C : public B
{
public:

    C();
    virtual ~C(){}
    
    virtual void greet() const;
};

C::C() : B()
{}

void C::greet() const
{
    cout << "Hello, I am C()\n";
}

void greet(const A &obj)
{
    obj.greet();
}

void A::sayHello(A &obj)
{
    cout << "Hello, I say I am A. You say; ";
    obj.greet();
}

int main(int argc, char **argv)
{
    B b;
    C c;
    
    greet(b);
    greet(c);
    
    greet( B() );
    greet( C() );

    b.sayHello(C());
    c.sayHello(B());

    return 0;
}

