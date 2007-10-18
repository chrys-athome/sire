/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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

#include <boost/python.hpp>

#include "SireBase/sharedpolypointer.hpp"

#include <QDebug>

using namespace boost::python;

namespace SireBase
{

class SharedBase : public QSharedData
{
public:
    SharedBase() : QSharedData()
    {
        qDebug() << "SharedBase::SharedBase()";
    }

    SharedBase(const SharedBase&) : QSharedData()
    {
        qDebug() << "SharedBase::SharedBase(const SharedBase&)";
    }

    virtual ~SharedBase()
    {}

    virtual void helloWorld() const=0;

    virtual SharedBase* clone() const=0;

    virtual const char* what() const=0;

    static const char* typeName()
    {
        return "SireBase::SharedBase";
    }
};

class SharedFoo : public SharedBase
{
public:
    SharedFoo() : SharedBase()
    {
        qDebug() << "SharedFoo::SharedFoo";
    }

    SharedFoo(const SharedFoo &other) :  SharedBase(other)
    {
        qDebug() << "SharedFoo::SharedFoo(const SharedFoo&)";
    }

    ~SharedFoo()
    {}

    void helloWorld() const
    {
        qDebug() << "Hello World from Foo!";
    }

    SharedFoo* clone() const
    {
        return new SharedFoo(*this);
    }

    static const char* typeName()
    {
        return "SireBase::SharedFoo";
    }

    const char* what() const
    {
        return SharedFoo::typeName();
    }
};

class SharedBar : public SharedBase
{
public:
    SharedBar() : SharedBase()
    {
        qDebug() << "SharedBar::SharedBar";
    }

    SharedBar(const SharedBar &other) :  SharedBase(other)
    {
        qDebug() << "SharedBar::SharedBar(const SharedBar&)";
    }

    ~SharedBar()
    {}

    void helloWorld() const
    {
        qDebug() << "Hello World from Bar!";
    }

    SharedBar* clone() const
    {
        return new SharedBar(*this);
    }

    void goodbye() const
    {
        qDebug() << "See ya!";
    }

    static const char* typeName()
    {
        return "SireBase::SharedBar";
    }

    const char* what() const
    {
        return SharedBar::typeName();
    }
};

SharedPolyPointer<SharedBase> createFoo()
{
    return SharedPolyPointer<SharedBase>( new SharedFoo() );
}

SharedPolyPointer<SharedBase> createBar()
{
    return SharedPolyPointer<SharedBase>( new SharedBar() );
}

void SIREBASE_EXPORT export_SharedPolyPointer()
{
    class_<SharedBase, boost::noncopyable>("SharedBase", no_init)
        .def( "helloWorld", &SharedBase::helloWorld )
    ;


    class_<SharedFoo, bases<SharedBase> >("SharedFoo", init<>() )
        .def( init<const SharedFoo&>() )
    ;

    class_<SharedBar, bases<SharedBase> >("SharedBar", init<>() )
        .def( init<const SharedBar&>() )
        .def( "goodbye", &SharedBar::goodbye )
    ;

    def( "createFoo", &createFoo );
    def( "createBar", &createBar );

    register_ptr_to_python< SharedPolyPointer<SharedBase> >();
}

}
