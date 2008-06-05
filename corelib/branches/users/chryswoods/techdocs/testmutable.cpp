
#include <iostream>

using namespace std;

class Base
{
public:
    Base(int i = 42) : val(i)
    {}
    
    Base(const Base &other) : val(other.val)
    {}
    
    ~Base()
    {}

    Base& operator=(const Base &other)
    {
        val = other.val;
        return *this;
    }

protected:
    int val;
};


class Derived : public Base
{
public:
    Derived(int i = 42) : Base(i)
    {}
    
    Derived(const Derived &other) : Base(other)
    {}
    
    ~Derived()
    {}
    
    int value() const
    {
        return val;
    }

private:
    Derived& operator=(const Derived &other)
    {
        Base::operator=(other);
        return *this;
    }
    
};

void badConst(Derived &derived)
{
    static_cast<Base&>(derived) = Base(92);
}

void breakConst(Base &base)
{
    base = Base(64);
}

int main(void)
{
    Derived d;
    
    cout << "Value of Derived == " << d.value() << endl;
    
    breakConst(d);
    
    cout << "Value of Derived == " << d.value() << endl;
    
    badConst(d);
    
    cout << "Value of Derived == " << d.value() << endl;

    return 0;
}
