
#include <iostream>

using namespace std;

class A
{
public:
    A()
    {}
    
    virtual ~A()
    {}
    
    virtual void printMe() const
    {
        cout << "This is an A\n";
    }
};

class B : public A
{
public:
    B() : A()
    {}
    
    ~B()
    {}
    
    void printMe() const
    {
        cout << "This is a B\n";
    }
};

class DataStream
{
public:
    DataStream()
    {}
    
    ~DataStream()
    {}
};

DataStream& operator<<(DataStream &ds, const A &a)
{
    cout << "operator<<(DataStream&, const A&)\n";
    a.printMe();
    return ds;
}

DataStream& operator>>(DataStream &ds, A &a)
{
    cout << "operator>>(DataStream&, A&)\n";
    a.printMe();
    return ds;
}

DataStream& operator>>(DataStream &ds, B &b)
{
    cout << "operator>>(DataStream&, B&)\n";
    b.printMe();
    return ds;
}

DataStream& operator<<(DataStream &ds, const B &b)
{
    cout << "operator<<(DataStream&, const B&)\n";
    b.printMe();
    return ds;
}

int main(void)
{
    DataStream ds;
    
    A a;
    B b;

    ds << a;
    ds << b;

    ds >> a;
    ds >> b;

    cout << "\nTest virtual...\n\n";
        
    A *aptr = &a;
    A *bptr = &b;
    
    ds << *aptr;
    ds >> *aptr;
    
    ds << *bptr;
    ds >> *bptr;

    cout << "\nThis demonstrates that virtual class hierarchies should\n"
         << "be serialised/deserialised using virtual dump() and load() functions.\n";

    return 0;
}
