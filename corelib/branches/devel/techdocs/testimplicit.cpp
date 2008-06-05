
#include <iostream>

using namespace std;

class Foo
{
public:
    Foo()
    {}
    
    ~Foo()
    {}
    
    int value() const
    {
        return 42;
    }
};

class Bar
{
public:
    Bar()
    {}
    
    ~Bar()
    {}
    
    operator const Foo&() const
    {
       return foo;
    }
private:
    Foo foo;
};

void printFoo(const Foo &foo)
{
    cout << "Foo == " << foo.value() << " " << &foo << endl;
}

void copyFoo(Foo foo)
{
    cout << "Copy == " << foo.value() << " " << &foo << endl;
}

int main(void)
{
    Foo f;
    Bar b;
    
    cout << &b << " " << &f << endl;
    
    printFoo(f);
    printFoo(b);
    
    copyFoo(b);
    
    return 0;
}
