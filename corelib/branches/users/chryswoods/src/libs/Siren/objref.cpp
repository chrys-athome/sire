/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2009  Christopher Woods
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

#include "objref.h"
#include "class.h"
#include "datastream.h"
#include "xmlstream.h"

using namespace Siren;
using namespace Siren::detail;

/** Serialise to a binary datastream */
DataStream SIREN_EXPORT &operator<<(DataStream &ds, const ObjRef &obj)
{
    ds << obj.d;
    return ds;
}

/** Extract from a binary datastream */
DataStream SIREN_EXPORT &operator>>(DataStream &ds, ObjRef &obj)
{
    ds >> obj.d;
    return ds;
}

/** Serialise to a binary datastream */
QDataStream SIREN_EXPORT &operator<<(QDataStream &ds, const ObjRef &obj)
{
    DataStream sds(ds);
    sds << obj;
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREN_EXPORT &operator>>(QDataStream &ds, ObjRef &obj)
{
    DataStream sds(ds);
    sds >> obj;
    
    return ds;
}

/** Serialise to XML */
XMLStream SIREN_EXPORT &operator<<(XMLStream &xml, const ObjRef &obj)
{
    xml << obj.d;
    return xml;
}

/** Extract from XML */
XMLStream SIREN_EXPORT &operator>>(XMLStream &xml, ObjRef &obj)
{
    xml >> obj.d;
    return xml;
}

Q_GLOBAL_STATIC_WITH_ARGS( SharedPolyPointer<Object>, getNone, (None()) );

/** Construct a null reference - this points to 'None' */
ObjRef::ObjRef() : d( *(getNone()) )
{}

/** Construct a reference to the object 'obj' - this creates
    a reference to a shallow copy of 'obj' */
ObjRef::ObjRef(const Object &obj) : d(obj)
{}

/** Construct a reference to the object pointed to by 'obj'. This
    takes over ownership of this pointer and can delete the object
    whenever it likes */
ObjRef::ObjRef(Object *obj) : d(obj)
{
    if (obj == 0)
        d = *(getNone());
}

/** Copy constructor */
ObjRef::ObjRef(const ObjRef &other) : d(other.d)
{}

/** Destructor */
ObjRef::~ObjRef()
{}

/** Copy assignment operator */
ObjRef& ObjRef::operator=(const ObjRef &other)
{
    d = other.d;
    return *this;
}

/** Copy assignment operator */
ObjRef& ObjRef::operator=(const Object &other)
{
    d = other;
    return *this;
}

/** Comparison operator */
bool ObjRef::operator==(const ObjRef &other) const
{
    return d.constData() == other.d.constData() or
           d->equals(other);
}

/** Comparison operator */
bool ObjRef::operator!=(const ObjRef &other) const
{
    return not ObjRef::operator==(other);
}

/** Comparison operator */
bool ObjRef::operator==(const Object &other) const
{
    return d->equals(other);
}

/** Comparison operator */
bool ObjRef::operator!=(const Object &other) const
{
    return not ObjRef::operator==(other);
}

/** Return the Class type for the object */
Class ObjRef::getClass() const
{
    return d->getClass();
}

/** Return the type name for the object */
QString ObjRef::what() const
{
    return d->what();
}

/** Return a copy of the object */
ObjRef ObjRef::copy() const
{
    return d->copy();
}

/** Return a string representation of this object */
QString ObjRef::toString() const
{
    return d->toString();
}

/** Unit test the class type of this object, returning 
    whether or not all of the tests passed. The output of
    the tests will be printed to the default Logger */
bool ObjRef::test() const
{
    return d->test();
}

/** Unit test the class type of this object, returning
    whether or not all of the tsts passed. The output
    will be printed to the passed logger */
bool ObjRef::test(Logger &logger) const
{
    return d->test(logger);
}

/** Copy the passed object 'other' - note that this
    copies into the object referred to by this reference */
void ObjRef::copy(const Object &other)
{
    d->copy(other);
}

/** Return whether or not this object equals 'other' */
bool ObjRef::equals(const Object &other) const
{
    return d->equals(other);
}

/** Return the hash code for this object */
HASH_CODE ObjRef::hashCode() const
{
    return d->hashCode();
}

/** Allow automatic casting to an Object */
ObjRef::operator const Object&() const
{
    return *d;
}
