
#include <iostream>

using namespace std;

class Base
{
public:
    Base()
    {}

    /** Base Destructor needs to be virtual to make this class hierarchy into 
        a polymorphic class. If you remove this virtual then the 'whatAmI' function
        will not work. Try to remove this virtual to see for yourself! */    
    virtual ~Base()
    {}
};

class ClassA : public Base
{
public:
    ClassA() : Base()
    {}
    
    ~ClassA()
    {}
};

class ClassB : public Base
{
public:
    ClassB() : Base()
    {}
    
    ~ClassB()
    {}
};

void whatAmI(const Base &typ)
{
    cout << typeid(typ).name() << endl;
    
    if (typeid(typ) == typeid(ClassA))
        cout << "Found a ClassA!\n";
    else if (typeid(typ) == typeid(ClassB))
        cout << "Found a ClassB!\n";
}

int main(void)
{
    ClassA a;
    ClassB b;
    
    whatAmI(a);
    whatAmI(b);
    
    return 0;
}
