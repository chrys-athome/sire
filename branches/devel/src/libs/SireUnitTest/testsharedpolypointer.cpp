/** 
  * @file
  *
  * C++ Implementation: TestSharedPolyPointer
  *
  * Description: 
  * Implementation for TestSharedPolyPointer
  *
  * @author Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */
  
#include <QSharedData>
#include <QByteArray>
#include <QDataStream>
  
#include "testsharedpolypointer.h"

#include "SireBase/sharedpolypointer.hpp"

#include "SireStream/datastream.h"

using namespace SireTest;
using namespace SireBase;
using namespace SireStream;

class Foo;
class Bar;
class Bar2;

QDataStream& operator<<(QDataStream&, const Foo&);
QDataStream& operator>>(QDataStream&, Foo&);

QDataStream& operator<<(QDataStream&, const Bar&);
QDataStream& operator>>(QDataStream&, Bar&);

QDataStream& operator<<(QDataStream&, const Bar2&);
QDataStream& operator>>(QDataStream&, Bar2&);

class Foo : public QSharedData
{

friend QDataStream& ::operator<<(QDataStream&, const Foo&);
friend QDataStream& ::operator>>(QDataStream&, Foo&);

public:
    Foo() : d(0)
    {}
    
    Foo(const Foo &other) : d(other.d)
    {}
    
    virtual ~Foo()
    {}
    
    virtual Foo* clone() const=0;
    
    virtual const char* what() const=0;
    
    void setValue(int val)
    {
        d = val;
    }
    
    virtual int value() const
    {
        return d;
    }
    
    virtual int special() const=0;
    
protected:
    int d;
};

class Bar : public Foo
{

friend QDataStream& ::operator<<(QDataStream&, const Bar&);
friend QDataStream& ::operator>>(QDataStream&, Bar&);

public:
    Bar() : Foo()
    {}
    
    Bar(const Bar &other) : Foo(other)
    {}
    
    ~Bar()
    {}
    
    Bar* clone() const
    {
        return new Bar(*this);
    }
    
    static const char* typeName()
    {
        return "Bar";
    }
    
    const char* what() const
    {
        return Bar::typeName();
    }
    
    int special() const
    {
        return 42;
    }
    
    int value() const
    {
        return 5*d;
    }
};

class Bar2 : public Foo
{

friend QDataStream& ::operator<<(QDataStream&, const Bar2&);
friend QDataStream& ::operator>>(QDataStream&, Bar2&);

public:
    Bar2() : Foo()
    {}
    
    Bar2(const Bar2 &other) : Foo(other)
    {}
    
    ~Bar2()
    {}
    
    Bar2* clone() const
    {
        return new Bar2(*this);
    }
    
    static const char* typeName()
    {
        return "Bar2";
    }
    
    const char* what() const
    {
        return Bar2::typeName();
    }
    
    int special() const
    {
        return 150;
    }
    
    int value() const
    {
        return 15*d;
    }
};

Q_DECLARE_METATYPE(Bar);
Q_DECLARE_METATYPE(Bar2);

static const RegisterMetaType<Bar> r_bar("Bar");
static const RegisterMetaType<Bar2> r_bar2("Bar2");

static const RegisterMetaType<Foo> r_foo("Foo", MAGIC_ONLY);

/** Serialise to a binary data stream */
QDataStream &operator<<(QDataStream &ds, const Foo &foo)
{
    writeHeader(ds, r_foo, 1) << foo.d;
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream &operator>>(QDataStream &ds, Foo &foo)
{
    VersionID v = readHeader(ds, r_foo);
    
    if (v == 1)
    {
        ds >> foo.d;
    }
    else
        throw version_error(v, "1", r_foo, CODELOC);
    
    return ds;
}

/** Serialise to a binary data stream */
QDataStream &operator<<(QDataStream &ds, const Bar &bar)
{
    writeHeader(ds, r_bar, 1) << static_cast<const Foo&>(bar);
    
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream &operator>>(QDataStream &ds, Bar &bar)
{
    VersionID v = readHeader(ds, r_bar);
    
    if (v == 1)
    {
        ds >> static_cast<Foo&>(bar);
    }
    else
        throw version_error(v, "1", r_bar, CODELOC);
    
    return ds;
}

/** Serialise to a binary data stream */
QDataStream &operator<<(QDataStream &ds, const Bar2 &bar2)
{
    writeHeader(ds, r_bar2, 1) << static_cast<const Foo&>(bar2);
    
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream &operator>>(QDataStream &ds, Bar2 &bar2)
{
    VersionID v = readHeader(ds, r_bar2);
    
    if (v == 1)
    {
        ds >> static_cast<Foo&>(bar2);
    }
    else
        throw version_error(v, "1", r_bar2, CODELOC);
    
    return ds;
}

TestSharedPolyPointer::TestSharedPolyPointer() : TestBase()
{}

TestSharedPolyPointer::~TestSharedPolyPointer()
{}

void TestSharedPolyPointer::initialise(test_suite *test)
{
    test->add( BOOST_TEST_CASE( &TestSharedPolyPointer::runTests ) );
}

void TestSharedPolyPointer::runTests()
{
    try
    {
    
    qDebug() << "TestSharedPolyPointer::runTests() begin...";

    SharedPolyPointer<Foo> fooptr;
    
    BOOST_CHECK( fooptr == 0 );

    fooptr = new Bar();
    
    BOOST_CHECK( fooptr->value() == 0 );
    BOOST_CHECK( fooptr->special() == 42 );
    BOOST_CHECK( fooptr->what() == Bar::typeName() );
    
    BOOST_CHECK( fooptr.isA<Bar>() );
    
    SharedPolyPointer<Foo> fooptr2 = fooptr;
    
    BOOST_CHECK( fooptr2->value() == 0 );
    BOOST_CHECK( fooptr2->special() == 42 );
    BOOST_CHECK( fooptr2->what() == Bar::typeName() );

    BOOST_CHECK( fooptr2.isA<Bar>() );

    fooptr->setValue(5);
    
    BOOST_CHECK( fooptr->value() == 25 );
    BOOST_CHECK( fooptr->special() == 42 );
    BOOST_CHECK( fooptr->what() == Bar::typeName() );
    
    BOOST_CHECK( fooptr2->value() == 0 );
    BOOST_CHECK( fooptr2->special() == 42 );
    BOOST_CHECK( fooptr2->what() == Bar::typeName() );

    fooptr2 = new Bar2();
    
    BOOST_CHECK( fooptr2->value() == 0 );
    BOOST_CHECK( fooptr2->special() == 150 );
    BOOST_CHECK( fooptr2->what() == Bar2::typeName() );

    BOOST_CHECK( fooptr2.isA<Bar2>() );

    fooptr = fooptr2;
    
    BOOST_CHECK( fooptr->value() == 0 );
    BOOST_CHECK( fooptr->special() == 150 );
    BOOST_CHECK( fooptr->what() == Bar2::typeName() );

    BOOST_CHECK( fooptr.isA<Bar2>() );

    fooptr->setValue(5);
    
    BOOST_CHECK( fooptr2->value() == 0 );
    BOOST_CHECK( fooptr2->special() == 150 );
    BOOST_CHECK( fooptr2->what() == Bar2::typeName() );
    
    BOOST_CHECK( fooptr->value() == 15*5 );
    BOOST_CHECK( fooptr->special() == 150 );
    BOOST_CHECK( fooptr->what() == Bar2::typeName() );

    //test streaming
    QByteArray bytedata;
    
    fooptr = new Bar();
    fooptr->setValue(15);
    fooptr2->setValue(35);
    
    BOOST_CHECK( fooptr->value() == 15*5 );
    BOOST_CHECK( fooptr->special() == 42 );
    BOOST_CHECK( fooptr->what() == Bar::typeName() );
    BOOST_CHECK( fooptr.isA<Bar>() );
    
    BOOST_CHECK( fooptr2->value() == 35*15 );
    BOOST_CHECK( fooptr2->special() == 150 );
    BOOST_CHECK( fooptr2->what() == Bar2::typeName() );
    BOOST_CHECK( fooptr2.isA<Bar2>() );
    
    QDataStream ds(&bytedata, QIODevice::WriteOnly);
    
    ds << fooptr << fooptr2;

    fooptr = 0;
    fooptr2 = 0;

    ds.unsetDevice();
    
    QDataStream ds2(bytedata);
    
    ds2 >> fooptr2 >> fooptr;
    
    BOOST_CHECK( fooptr2->value() == 15*5 );
    BOOST_CHECK( fooptr2->special() == 42 );
    BOOST_CHECK( fooptr2->what() == Bar::typeName() );
    BOOST_CHECK( fooptr2.isA<Bar>() );
    
    BOOST_CHECK( fooptr->value() == 35*15 );
    BOOST_CHECK( fooptr->special() == 150 );
    BOOST_CHECK( fooptr->what() == Bar2::typeName() );
    BOOST_CHECK( fooptr.isA<Bar2>() );
    
    //test polymorphic casting
    SharedPolyPointer<Bar> barptr = fooptr2;
    
    BOOST_CHECK( barptr->value() == 15*5 );
    BOOST_CHECK( barptr->special() == 42 );
    BOOST_CHECK( barptr->what() == Bar::typeName() );
    BOOST_CHECK( barptr.isA<Bar>() );
    
    SharedPolyPointer<Bar2> bar2ptr = fooptr;
    
    BOOST_CHECK( bar2ptr->value() == 35*15 );
    BOOST_CHECK( bar2ptr->special() == 150 );
    BOOST_CHECK( bar2ptr->what() == Bar2::typeName() );
    BOOST_CHECK( bar2ptr.isA<Bar2>() );
    
    try
    {
        barptr = bar2ptr;
        bool should_not_get_here(false);
        BOOST_CHECK( should_not_get_here );
    }
    catch(const SireError::invalid_cast&)
    {}
    
    try
    {
        bar2ptr = barptr;
        bool should_not_get_here(false);
        BOOST_CHECK( should_not_get_here );
    }
    catch(const SireError::invalid_cast&)
    {}
    
    fooptr = barptr;
    fooptr2 = bar2ptr;
    
    BOOST_CHECK( fooptr->value() == 15*5 );
    BOOST_CHECK( fooptr->special() == 42 );
    BOOST_CHECK( fooptr->what() == Bar::typeName() );
    BOOST_CHECK( fooptr.isA<Bar>() );
    
    BOOST_CHECK( fooptr2->value() == 35*15 );
    BOOST_CHECK( fooptr2->special() == 150 );
    BOOST_CHECK( fooptr2->what() == Bar2::typeName() );
    BOOST_CHECK( fooptr2.isA<Bar2>() );
    
    qDebug() << "TestSharedPolyPointer::runTests() complete";
    
    }
    catch(const SireError::exception &e)
    {
        qDebug() << e.toString();
        throw;
    }
}
