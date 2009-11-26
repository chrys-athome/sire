
#include <iostream>

using namespace std;

template<class T>
class RegisterMetaType
{
public:
    RegisterMetaType()
    {
        T::registerInheritance();
    }

    ~RegisterMetaType()
    {}
};

class Foo
{
public:
    Foo()
    {}

    virtual ~Foo()
    {}

    static const char* typeName()
    {
        return "Foo";
    }

protected:
    static void registerSuperClass()
    {}

    static void registerInterfaces()
    {}
};

class Interface
{
public:
    Interface()
    {}

    virtual ~Interface()
    {}

    static const char* typeName()
    {
        return "Interface";
    }

protected:
    static bool IS_INTERFACE()
    {
        return true;
    }
};

class FooInterface : public virtual Interface
{
public:
    FooInterface() : Interface()
    {}

    ~FooInterface()
    {}

    static const char* typeName()
    {
        return "FooInterface";
    }
};

template<class Derived, class Base>
class Implements : public Base
{

friend class RegisterMetaType<Derived>;

public:
    Implements() : Base()
    {}

    ~Implements()
    {}

protected:
    static void registerInheritance()
    {
        Derived::registerSuperClass();
        Derived::registerInterfaces();
    }

    static void registerSuperClass()
    {
        cout << "Registering " << Base::typeName() << " as a superclass of "
             << Derived::typeName() << "\n";

        Base::registerSuperClass();
    }

    static void registerInterfaces()
    {
        Base::registerInterfaces();
    }
};

template<class Derived, class Base>
class Extends : public Base
{
public:
    Extends() : Base()
    {}

    ~Extends()
    {}

protected:
    static void registerSuperClass()
    {
        cout << "Registering " << Base::typeName() << " as a superclass of "
             << Derived::typeName() << "\n";
    }
};

class None
{
protected:
    static bool IS_INTERFACE()
    {
        return false;
    }

    static const char* typeName()
    {
         return 0;
    }
};

class None_1 : public None
{};

template<class Derived, class I0, class I1=None_1>
class Interfaces : public virtual I0, public virtual I1
{
public:
    Interfaces() : I0(), I1()
    {}

    ~Interfaces()
    {}

protected:
    static void registerInterfaces()
    {
        if (I0::IS_INTERFACE())
            cout << "Registering interface " << I0::typeName()
                 << " for class " << Derived::typeName() << "\n";
    
        if (I1::IS_INTERFACE())
            cout << "Registering interface " << I1::typeName()
                 << " for class " << Derived::typeName() << "\n";
    }
};

class MoreFoo : public Extends<MoreFoo,Foo>
{
public:
    MoreFoo() : Extends<MoreFoo,Foo>()
    {}

    ~MoreFoo()
    {}

    static const char* typeName()
    {
        return "MoreFoo";
    }
};

class Bar : public Implements<Bar,MoreFoo>, 
            public virtual Interfaces<Bar,FooInterface,Interface>
{

public:
    Bar() : Interfaces<Bar,FooInterface,Interface>(), Implements<Bar,MoreFoo>()
    {}

    ~Bar()
    {}

    static const char* typeName()
    {
        return "Bar";
    }

protected:
    friend class Implements<Bar,MoreFoo>;

    static void registerInterfaces()
    {
        Interfaces<Bar,FooInterface,Interface>::registerInterfaces();
        MoreFoo::registerInterfaces();
    }
};

static const RegisterMetaType<Bar> r_bar;

int main(int argc, const char **argv)
{
    Bar b;

    cout << "Here is a Bar " << b.typeName() << "\n";

    cout << "sizeof(Foo) == " << sizeof(Foo) << "\n";
    cout << "sizeof(FooInteface) == " << sizeof(FooInterface) << "\n";
    cout << "sizeof(Bar) == " << sizeof(Bar) << "\n";

    return 0;
}
