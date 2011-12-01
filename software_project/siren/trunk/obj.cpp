/********************************************\
  *
  *  Siren - C++ metaobject library
  *
  *  Copyright (C) 2011  Christopher Woods
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

#include "Siren/obj.h"
#include "Siren/none.h"
#include "Siren/testreport.h"
//#include "Siren/stream.h"
#include "Siren/static.h"
#include "Siren/siren.hpp"
#include "Siren/exceptions.h"

using namespace Siren;

/** Null constructor - this holds a pointer to None */
Obj::Obj()
{}

/** Construct to hold Siren::None */
Obj::Obj(const None&)
{}

/** Construct to hold a clone of the passed object */
Obj::Obj(const Object &object)
{
    if (not object.isNone())
        this->operator=( object.clone() );
}

/** Construct to hold a pointer to the passed object - the Obj
    will take over ownership of the pointer and will delete the 
    object when it no-longer references it */
Obj::Obj(Object *object) : d(object)
{
    if (object == 0)
        d.reset();
}

/** Copy constructor */
Obj::Obj(const Obj &other) : d(other.d)
{}

/** Destructor */
Obj::~Obj()
{}

/** Copy assignment operator */
Obj& Obj::operator=(const Obj &other)
{
    d = other.d;
    return *this;
}

/** Assign this reference to equal a clone of the passed object */
Obj& Obj::operator=(const Object &other)
{
    if (other.isNone())
        d.reset();

    else if (d.get() != &other)
        this->operator=(other.clone());
    
    return *this;
}

/** Assign this reference to hold the passed pointer to an object.
    This reference will take over ownership of the object and will
    delete it when it is no longer referenced */
Obj& Obj::operator=(Object *object)
{
    if (object == 0)
        d.reset();

    else if (object->isNone())
        d.reset();

    else
        d.reset( object );
    
    return *this;
}

/** Comparison operator */
bool Obj::operator==(const Obj &other) const
{
    if (this->isNone())
        return other.isNone();
    else
        return d.get() == other.d.get() or d->equals(other);
}

/** Comparison operator */
bool Obj::operator!=(const Obj &other) const
{
    return not Obj::operator==(other);
}

/** Comparison operator */
bool Obj::operator==(const Object &other) const
{
    if (this->isNone())
        return other.isNone();
    else
        return d.get() == &other or d->equals(other);
}

/** Comparison operator */
bool Obj::operator!=(const Object &other) const
{
    return not operator==(other);
}

/** Return the class type of the referenced object */
Class Obj::getClass() const
{
    if (this->isNone())
        return None::typeClass();
    else
        return d->getClass();
}

/** Return the version number of the class of the referenced object */
int Obj::getClassVersion() const
{
    if (this->isNone())
        return None::typeClassVersion();
    else
        return d->getClassVersion();
}

/** Return the type of the passed object */
const char* Obj::what() const
{
    if (this->isNone())
        return None::typeName();
    else
        return d->what();
}

/** Return whether or not this object is None */
bool Obj::isNone() const
{
    return d.get() == 0;
}

/** Return whether the object is an error (object derived from Siren::Exception) */
bool Obj::isError() const
{
    return dynamic_cast<const Siren::Exception*>(d.get()) != 0;
}

/** If this object is an error, then throw that error */
void Obj::throwError() const
{
    const Siren::Exception *error = dynamic_cast<const Siren::Exception*>(d.get());
    
    if (error)
        error->throwSelf();
}

/** Copy the passed object into this object */
void Obj::copy(const Object &other)
{
    this->operator=(other);
}

/** Return whether or not this object equals 'other' */
bool Obj::equals(const Object &other) const
{
    return this->operator==(other);
}

/** Return a clone of this object */
Obj Obj::clone() const
{
    return *this;
}

/** Save this object to the passed stream */
void Obj::save(Stream &s) const
{
    if (this->isNone())
    {
        None n;
        n.save(s);
    }
    else
        d->save(s);
}

/** Load this object from the passed stream */
void Obj::load(Stream &s)
{
    //this->operator=( s.loadNextObject() );
}

/** Test the contained class */
TestReport Obj::test() const throw()
{
    if (this->isNone())
    {
        None n;
        return n.test();
    }
    else
        return d->test();
}

/** Stress-test the contained class */
TestReport Obj::stressTest() const throw()
{
    if (this->isNone())
    {
        None n;
        return n.stressTest();
    }
    else
        return d->stressTest();
}

/** Return a string representation of the contained object */
String Obj::toString() const
{
    if (this->isNone())
    {
        None n;
        return n.toString();
    }
    else
        return d->toString();
}

/** Return the documentation for the contained object */
String Obj::docString() const
{
    if (this->isNone())
    {
        None n;
        return n.docString();
    }
    else
        return d->docString();
}

/** Return the documentation for the named function of 
    the contained object */
String Obj::docString(const String &function) const
{
    if (this->isNone())
    {
        None n;
        return n.docString(function);
    }
    else
        return d->docString(function);
}

/** Return a hashcode for the contained object */
uint Obj::hashCode() const
{
    if (this->isNone())
    {
        None n;
        return n.hashCode();
    }
    else
        return d->hashCode();
}

/** Stream the contained object */
void Obj::stream(Stream &s)
{
    //if (s.isWriting())
    //    this->save(s);
    //else
    //    this->load(s);
}

SIREN_STATIC( None, getNone );

const None& Obj::globalNone() const
{
    exp_shared_ptr<None>::Type n = getNone();
    
    if (not n)
        throw Siren::invalid_state( String::tr(
                "Cannot get the global none object during program "
                "shutdown or startup."), CODELOC );
                
    return *n;
}

/** Allow automatic casting to Siren::Object */
Obj::operator const Object&() const
{
    if (this->isNone())
        return globalNone();
    else
        return *d;
}

/** Allow automatic casting to a pointer to Siren::Object */
Obj::operator const Object*() const
{
    if (this->isNone())
        return &(globalNone());
    else
        return d.get();
}
