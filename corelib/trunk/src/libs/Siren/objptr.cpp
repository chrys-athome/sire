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

#include "objptr.hpp"
#include "class.h"
#include "datastream.h"
#include "xmlstream.h"

#include "Siren/errors.h"

using namespace Siren;
using namespace Siren::detail;

////////
//////// Implementation of ObjPtrBase
////////

/** Construct the pointer to the passed object */
ObjPtrBase::ObjPtrBase(const Object &object) : ptr(object)
{}

/** Construct the pointer to the passed object
     - this takes over ownership of 'object' */
ObjPtrBase::ObjPtrBase(Object *object) : ptr(object)
{}

/** Copy constructor */
ObjPtrBase::ObjPtrBase(const ObjPtrBase &other) : ptr(other.ptr)
{}

/** Destructor */
ObjPtrBase::~ObjPtrBase()
{}

/** Copy assignment operator */
ObjPtrBase& ObjPtrBase::operator=(const ObjPtrBase &other)
{
    ptr = other.ptr;
    return *this;
}

/** Comparison operator - this just performs pointer based
    comparison (so compares memory addresses) */
bool ObjPtrBase::operator==(const Object &object) const
{
    return ptr.constData() == &object;
}

/** Comparison operator - this just performs pointer based
    comparison (so compares memory addresses) */
bool ObjPtrBase::operator!=(const Object &object) const
{
    return not ObjPtrBase::operator==(object);
}

/** Comparison operator - this just performs pointer based
    comparison (so compares memory addresses) */
bool ObjPtrBase::operator==(const ObjPtrBase &other) const
{
    return ptr.constData() == other.ptr.constData();
}

/** Comparison operator - this just performs pointer based
    comparison (so compares memory addresses) */
bool ObjPtrBase::operator!=(const ObjPtrBase &other) const
{
    return not ObjPtrBase::operator==(other);
}

/** Detach this pointer from shared storage */
void ObjPtrBase::detach()
{
    ptr.detach();
}

/** Return whether or not this is the only pointer
    to the object */
bool ObjPtrBase::unique() const
{
    return ptr.unique();
}

/** Return whether or not this pointer is null */
bool ObjPtrBase::isNull() const
{
    return ptr.constData() == 0;
}

Q_GLOBAL_STATIC_WITH_ARGS( SharedPolyPointer<Object>, getNone, (None()) );

/** Allow automatic casting to an Object - this returns None
    if this is a null pointer */
ObjPtrBase::operator const Object&() const
{
    if (ptr.constData() != 0)
        return *ptr;
    else
        return *( getNone()->constData() );
}

/** Assert that this is not a null pointer 

    \throw Siren::nullptr_error
*/
void ObjPtrBase::assertNotNull() const
{
    if (ptr.constData() == 0)
        throw Siren::nullptr_error( QObject::tr(
            "You cannot dereference a null pointer."), CODELOC );
}

/** Return a read-only reference to this object

    \throw Siren::nullptr_error
*/
const Object& ObjPtrBase::read() const
{
    assertNotNull();
    return *ptr;
}

/** Return the raw pointer */
const Object* ObjPtrBase::data() const
{
    return ptr.data();
}

/** Return the raw pointer */
const Object* ObjPtrBase::constData() const
{
    return ptr.constData();
}

/** Return the raw pointer */
Object* ObjPtrBase::data()
{
    return ptr.data();
}

/** Return an editable reference to the object

    \throw Siren::nullptr_error
*/
Object& ObjPtrBase::write()
{
    assertNotNull();
    return *ptr;
}

/** Return an editable reference to the object

    \throw Siren::nullptr_error
*/
Object& ObjPtrBase::edit()
{
    return this->write();
}

void ObjPtrBase::throwCastingError(QString got_type, QString want_type)
{
    throw Siren::invalid_cast( QObject::tr(
            "You cannot case from an Object of type %1 to an Object of type %2.")
                .arg(got_type, want_type), CODELOC );
}

////////
//////// Implementation of GlobalObjPtrBase
////////

/** Construct to hold a pointer to 'object' */
GlobalObjPtrBase::GlobalObjPtrBase(const Object &object)
                 : ptr(object)
{}

/** Construct to hold a pointer to 'object' - this takes over
    ownership of the pointer */
GlobalObjPtrBase::GlobalObjPtrBase(Object *object)
                 : ptr(object)
{}

/** Copy constructor */
GlobalObjPtrBase::GlobalObjPtrBase(const GlobalObjPtrBase &other)
                 : ptr(other.ptr)
{}

/** Destructor */
GlobalObjPtrBase::~GlobalObjPtrBase()
{}

/** Copy assignment operator */
GlobalObjPtrBase& GlobalObjPtrBase::operator=(const GlobalObjPtrBase &other)
{
    ptr = other.ptr;
    return *this;
}

/** Comparison operator */
bool GlobalObjPtrBase::operator==(const GlobalObjPtrBase &other) const
{
    return ptr.constData() == other.ptr.constData();
}

/** Comparison operator */
bool GlobalObjPtrBase::operator!=(const GlobalObjPtrBase &other) const
{
    return not GlobalObjPtrBase::operator==(other);
}

/** Comparison operator */
bool GlobalObjPtrBase::operator==(const Object &other) const
{
    return ptr.constData() == &other;
}

/** Comparison operator */
bool GlobalObjPtrBase::operator!=(const Object &other) const
{
    return not GlobalObjPtrBase::operator==(other);
}

/** Is this a unique pointer to the object? */
bool GlobalObjPtrBase::unique() const
{
    return ptr.unique();
}

/** Allow automatic casting to a Object */
GlobalObjPtrBase::operator const Object&() const
{
    if (ptr.constData() != 0)
        return *ptr;
    else
        return *( getNone()->constData() );
}

/** Return whether or not this pointer is null */
bool GlobalObjPtrBase::isNull() const
{
    return ptr.constData() == 0;
}

/** Assert that this is not a null pointer 

    \throw Siren::nullptr_error
*/
void GlobalObjPtrBase::assertNotNull() const
{
    if (ptr.constData() == 0)
        throw Siren::nullptr_error( QObject::tr(
            "You cannot dereference a null pointer."), CODELOC );
}

/** Return a read-only reference to this object

    \throw Siren::nullptr_error
*/
const Object& GlobalObjPtrBase::read() const
{
    assertNotNull();
    return *ptr;
}

/** Return the raw pointer */
const Object* GlobalObjPtrBase::data() const
{
    return ptr.data();
}

/** Return the raw pointer */
const Object* GlobalObjPtrBase::constData() const
{
    return ptr.constData();
}

void GlobalObjPtrBase::throwCastingError(QString got_type, QString want_type)
{
    throw Siren::invalid_cast( QObject::tr(
            "You cannot case from an Object of type %1 to an Object of type %2.")
                .arg(got_type, want_type), CODELOC );
}

/////////
///////// Instantiate template classes
/////////

template class ObjPtr<Object>;
template class GlobalObjPtr<Object>;
