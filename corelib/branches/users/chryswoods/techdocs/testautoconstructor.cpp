
#include <iostream>

using namespace std;

class Foo
{
public:
    Foo()
    {
        cout << "Foo::Foo()\n";
    }

    Foo(const Foo&)
    {
        cout << "Foo::Foo(const Foo&)\n";
    }

    ~Foo()
    {}

    Foo& operator=(const Foo&)
    {
        cout << "Foo& operator=(const Foo&)\n";
        return *this;
    }
};

class Bar
{
public:
    Bar()
    {}

    ~Bar()
    {}

    Foo foo1;
    double val1;
    Foo foo2;
    int val2;
};

int main(int argc, const char **argv)
{
    Bar bar1;

    Bar bar2;

    cout << "Copying...\n";

    bar1 = bar2;

    cout << "Copy constructor...\n";

    Bar bar3 = bar2;

    return 0;
}

