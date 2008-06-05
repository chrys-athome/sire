
#include <iostream>

#include <QMetaType>

using namespace std;

namespace Test
{

template<class T>
class Foo
{
public:
    Foo()
    {}
    
    ~Foo()
    {}
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId< Foo<T> >() );
    }
};

class Bar
{
public:
    Bar()
    {}
    
    ~Bar()
    {}
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<Bar>() );
    }
};

typedef Foo<Bar> FooBar;

template<>
class Foo<Bar>
{
public:
    Foo();
    
    ~Foo();
    
    static const char* typeName();
};

Foo<Bar>::Foo()
{
    cout << "Creating a FooBar!\n";
}

Foo<Bar>::~Foo()
{}

const char* Foo<Bar>::typeName()
{
    cout << "FooBar::typeName()\n";
    return QMetaType::typeName( qMetaTypeId<FooBar>() );
}

}

Q_DECLARE_METATYPE( Test::Foo<int> );
Q_DECLARE_METATYPE( Test::Bar );
Q_DECLARE_METATYPE( Test::FooBar );
