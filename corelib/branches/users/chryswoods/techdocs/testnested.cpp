
#include <iostream>

using namespace std;

class Foo
{
public:
    Foo();
    ~Foo();
    
    class Bar;

    void helloWorld() const;

private:
    Bar *mybar;

};

class Foo::Bar
{
public:
    Bar()
    {}
    
    ~Bar()
    {}
    
    void helloWorld() const
    {
        cout << "Hello World!\n";
    }
};

Foo::Foo() : mybar( new Foo::Bar() )
{}

Foo::~Foo()
{
    delete mybar;
}

void Foo::helloWorld() const
{
    mybar->helloWorld();
}


int main(void)
{
    Foo foo;
    
    foo.helloWorld();
    
    return 0;
}

