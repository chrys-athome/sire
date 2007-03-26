/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007  Christopher Woods
  *
  *  This program is free software; you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation; either version 2 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program; if not, write to the Free Software
  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  *  For full details of the license please see the COPYING file
  *  that should have come with this distribution.
  *
  *  You can contact the authors via the developer's mailing list
  *  at http://siremol.org
  *
\*********************************************/

#include <QSharedData>
#include <QByteArray>
#include <QDataStream>

#include "testsharedpolypointer.h"

#include "SireBase/sharedpolypointer.hpp"

#include "SireError/errors.h"
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
    {
        qDebug() << "Foo is being copied!";
    }

    virtual ~Foo()
    {}

    virtual Foo* clone() const=0;

    static const char* typeName()
    {
        return "Foo";
    }

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
    {
        qDebug() << "Bar is being copied";
    }

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
    {
        qDebug() << "Bar2 is being copied";
    }

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

static const RegisterMetaType<Bar> r_bar;
static const RegisterMetaType<Bar2> r_bar2;

static const RegisterMetaType<Foo> r_foo(MAGIC_ONLY, "Foo");

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

    //see if assignment from the stack works... (it should clone
    //an object from the stack)
    do
    {
        Bar bar;

        Bar *barptr = &bar;

        qDebug() << "Assigning from the stack...";
        fooptr = Bar();

        BOOST_CHECK( barptr != fooptr.constData() );

        qDebug() << "Construct from the stack...";
        SharedPolyPointer<Foo> fooptr2( bar );

        BOOST_CHECK( barptr != fooptr2.constData() );
    }
    while(false);

    BOOST_CHECK( fooptr.constData() != 0 );

    fooptr = new Bar();

    BOOST_CHECK( fooptr->value() == 0 );
    BOOST_CHECK( fooptr->special() == 42 );
    BOOST_CHECK( fooptr->what() == Bar::typeName() );

    //see if assignment from another pointer reference works
    //(it should avoid cloning)
    qDebug() << "Construct from obj from a ptr...";
    SharedPolyPointer<Foo> fooptr2( *fooptr );

    BOOST_CHECK_EQUAL( fooptr.constData(), fooptr2.constData() );

    fooptr2 = 0;
    BOOST_CHECK( !fooptr2 );

    qDebug() << "Assign from obj from a ptr...";
    fooptr2 = *fooptr;
    qDebug() << "Great!";

    BOOST_CHECK_EQUAL( fooptr.constData(), fooptr2.constData() );

    BOOST_CHECK( fooptr2->value() == 0 );
    BOOST_CHECK( fooptr2->special() == 42 );
    BOOST_CHECK( fooptr2->what() == Bar::typeName() );

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

    fooptr = fooptr2;

    BOOST_CHECK( fooptr->value() == 0 );
    BOOST_CHECK( fooptr->special() == 150 );
    BOOST_CHECK( fooptr->what() == Bar2::typeName() );

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

    BOOST_CHECK( fooptr2->value() == 35*15 );
    BOOST_CHECK( fooptr2->special() == 150 );
    BOOST_CHECK( fooptr2->what() == Bar2::typeName() );

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

    BOOST_CHECK( fooptr->value() == 35*15 );
    BOOST_CHECK( fooptr->special() == 150 );
    BOOST_CHECK( fooptr->what() == Bar2::typeName() );

    //test polymorphic casting
    SharedPolyPointer<Bar> barptr = fooptr2;

    BOOST_CHECK( barptr->value() == 15*5 );
    BOOST_CHECK( barptr->special() == 42 );
    BOOST_CHECK( barptr->what() == Bar::typeName() );

    SharedPolyPointer<Bar2> bar2ptr = fooptr;

    BOOST_CHECK( bar2ptr->value() == 35*15 );
    BOOST_CHECK( bar2ptr->special() == 150 );
    BOOST_CHECK( bar2ptr->what() == Bar2::typeName() );

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

    BOOST_CHECK( fooptr2->value() == 35*15 );
    BOOST_CHECK( fooptr2->special() == 150 );
    BOOST_CHECK( fooptr2->what() == Bar2::typeName() );

    barptr = fooptr;
    bar2ptr = fooptr2;

    qDebug() << "TestSharedPolyPointer::runTests() complete";

    }
    catch(const SireError::exception &e)
    {
        qDebug() << e.toString();
        throw;
    }
}
