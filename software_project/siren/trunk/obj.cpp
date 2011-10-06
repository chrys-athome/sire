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
//#include "Siren/stream.h"
#include "Siren/siren.hpp"

using namespace Siren;

static Obj none( new None() );

/** Null constructor - this holds a pointer to None */
Obj::Obj() : d(none.d)
{}

/** Construct to hold a clone of the passed object */
Obj::Obj(const Object &object)
{
    this->operator=( object.clone() );
}

/** Construct to hold a pointer to the passed object - the Obj
    will take over ownership of the pointer and will delete the 
    object when it no-longer references it */
Obj::Obj(Object *object) : d(object)
{
    if (object == 0)
        this->operator=(none);
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
    if (d.get() != &other)
    {
        this->operator=(other.clone());
    }
    
    return *this;
}

/** Assign this reference to hold the passed pointer to an object.
    This reference will take over ownership of the object and will
    delete it when it is no longer referenced */
Obj& Obj::operator=(Object *object)
{
    if (object == 0)
        this->operator=(none);

    else
        d.reset( object );
    
    return *this;
}

/** Comparison operator */
bool Obj::operator==(const Obj &other) const
{
    return d.get() == other.d.get() or d->equals(other);
}

/** Comparison operator */
bool Obj::operator!=(const Obj &other) const
{
    return d.get() != other.d.get() and (not d->equals(other));
}

/** Comparison operator */
bool Obj::operator==(const Object &other) const
{
    return d.get() == &other or d->equals(other);
}

/** Comparison operator */
bool Obj::operator!=(const Object &other) const
{
    return d.get() != &other and (not d->equals(other));
}

/** Return the class type of the referenced object */
Class Obj::getClass() const
{
    return d->getClass();
}

/** Return the type of the passed object */
const char* Obj::what() const
{
    return d->what();
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
    d->save(s);
}

/** Load this object from the passed stream */
void Obj::load(Stream &s)
{
    //this->operator=( s.loadNextObject() );
}

/** Test the contained class */
bool Obj::test() const
{
    return d->test();
}

/** Return a string representation of the contained object */
String Obj::toString() const
{
    return d->toString();
}

/** Test the contained object */
bool Obj::test(Logger &logger) const
{
    return d->test(logger);
}

/** Return the documentation for the contained object */
String Obj::docString() const
{
    return d->docString();
}

/** Return the documentation for the named function of 
    the contained object */
String Obj::docString(const String &function) const
{
    return d->docString(function);
}

/** Return a hashcode for the contained object */
uint Obj::hashCode() const
{
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

/** Allow automatic casting to Siren::Object */
Obj::operator const Object&() const
{
    return *d;
}

/** Allow automatic casting to a pointer to Siren::Object */
Obj::operator const Object*() const
{
    return d.get();
}
