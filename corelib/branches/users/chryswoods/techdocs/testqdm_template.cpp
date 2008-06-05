
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

}

Q_DECLARE_METATYPE( Test::Foo<int> );
Q_DECLARE_METATYPE( Test::Bar );
Q_DECLARE_METATYPE( Test::FooBar );

using namespace Test;

int main(void)
{
    cout << Foo<int>::typeName() << endl;
    cout << Foo<Bar>::typeName() << endl;
    cout << Bar::typeName() << endl;
    
    Foo<Bar> foobar;
    
    return 0;
}

