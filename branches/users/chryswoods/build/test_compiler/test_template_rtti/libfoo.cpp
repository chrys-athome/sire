
#include "libfoo.h"

FooBase::FooBase()
{}

FooBase::FooBase(const FooBase&)
{}

FooBase::~FooBase()
{}

FooBase Q_DECL_EXPORT *makeFoo_Int()
{
    return new Foo<int>(42);
}

FooBase Q_DECL_EXPORT *makeFoo_Double()
{
    return new Foo<double>(365.25);
}

bool Q_DECL_EXPORT testFoo_Int(const FooBase &foo)
{
    return foo.isA< Foo<int> >();
}

bool Q_DECL_EXPORT testFoo_Double(const FooBase &foo)
{
    return foo.isA< Foo<double> >();
}
