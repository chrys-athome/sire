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

#include "class.h"
#include "none.h"
#include "obj.h"
#include "siren.hpp"

using namespace Siren;

typedef Hash<String,const detail::ClassData*>::Type ObjectRegistry;

static ObjectRegistry object_registry;
static Mutex registry_mutex;

/** Return the class data for the class with specified type name */
static const detail::ClassData* getClassData(const char *type_name)
{
    MutexLocker lkr(&registry_mutex);
    
    ObjectRegistry::const_iterator it = object_registry.constFind( String(type_name) );
    
    if (it == object_registry.constEnd())
    {
        sirenDebug() << "CANNOT FIND OBJECT" << type_name;
        return 0;
    }
    
    return it.value();
}

/** Return the class data for the class with specified type name */
static const detail::ClassData* getClassData(const String &type_name)
{
    return getClassData( type_name.toAscii().data() );
}

/** Register the passed class metadata. This ensures that only one copy 
    of metadata exists for each Siren class */
static const detail::ClassData* registerClassData(const detail::ClassData &data)
{
    if (data.typeName() == 0)
    {
        sirenDebug() << "REGISTERING A NULL TYPE";
    }

    String type_name( data.typeName() );

    MutexLocker lkr(&registry_mutex);

    ObjectRegistry::const_iterator it = object_registry.constFind(type_name);
    
    if (it.value() != &data)
    {
        sirenDebug() << "Trying to register a class twice!"
                 << type_name << &data << it.value();
                 
        return it.value();
    }

    object_registry.insert(type_name, &data);
    
    return &data;
}

/** Return a full list of the registered Siren classes in this program */
StringList Class::registeredTypes()
{
    MutexLocker lkr( &registry_mutex );
    return object_registry.keys();
}

/** Null constructor */
Class::Class() : super(), d(0)
{}

/** Construct the class handle for the passed object */
Class::Class(const Object &object) : super(), d(0)
{
    this->operator=(object.getClass());
}

/** Construct the class handle for the passed class metadata */
Class::Class(const detail::ClassData &data) : super(), d(0)
{
    d = registerClassData(data);
}

/** Construct the class handle for the class with specified name */
Class::Class(const char *type_name) : super(), d( getClassData(type_name) )
{}

/** Construct the class handle for the class with specified name */
Class::Class(const String &type_name) : super(), d( getClassData(type_name) )
{}

/** Copy constructor */
Class::Class(const Class &other) : super(other), d(other.d)
{} 

/** Destructor */
Class::~Class()
{}

/** Copy assignment operator */
Class& Class::operator=(const Class &other)
{
    d = other.d;
    return *this;
}

/** Comparison operator */
bool Class::operator==(const Class &other) const
{
    return d == other.d;
}

/** Comparison operator */
bool Class::operator!=(const Class &other) const
{
    return d != other.d;
}

/** Check that the class is not null. This will ensure that
    a null Class describes the Siren::None class */
void Class::checkNotNull() const
{
    if (d == 0)
    {
        None none;
    
        const_cast<Class*>(this)->operator=(none.getClass());
    }
}

/** Return the name of the class */
String Class::name() const
{
    checkNotNull();
    return d->typeName();
}

/** Return whether or not the class has a super class (should do!) */
bool Class::hasSuper() const
{
    checkNotNull();
    return d->baseTypeName() != 0;
}

/** Return the class object that represents the superclass of this class */
Class Class::superClass() const
{
    checkNotNull();
    return Class( d->baseTypeName() );
}

/** Return whether or not this class inherits from the type "type_name",
    or implements the interface with that type */
bool Class::implements(const String &type_name) const
{
    if (this->name() == type_name)
        return true;
        
    else if (this->interfaces().contains(type_name))
        return true;

    else if (this->hasSuper())
        return this->superClass().implements(type_name);
    
    else
        return false;
}

/** Return whether or not this class inherits from the type "type_name",
    or implements the interface with that type */
bool Class::implements(const char* type_name) const
{
    return this->implements( String(type_name) );
}

/** Return the list of interfaces supported by this class */
StringList Class::interfaces() const
{
    checkNotNull();
    
    StringList ifaces;
    
    for (int i=0; i < d->nInterfaces(); ++i)
    {
        ifaces.append( String(d->interfaces()[i]) );
    }
    
    return ifaces;
}

/** Return whether or not this class is a concrete type */
bool Class::isConcrete() const
{
    checkNotNull();
    return d->isConcrete();
}

/** Return whether or not objects of this type can be created */
bool Class::canCreate() const
{
    return this->isConcrete();
}

/** Return a newly created object of this class */
Obj Class::createObject() const
{
    if (this->isConcrete())
        return Obj( d->createObject() );
    else
    {
        sirenDebug() << "CANNOT PRODUCE OBJECT OF VIRTUAL TYPE" << this->name();
        return Obj();
    }
}

/** Return whether or not this type can be cast to the class "type_name" */
bool Class::canCast(const String &type_name) const
{
    return this->implements(type_name);
}

/** Return whether or not this type can be cast to the class "type_name" */
bool Class::canCast(const char *type_name) const
{
    return this->implements(type_name);
}

/** Assert that is is possible to cast this type to the class "type_name" */
void Class::assertCanCast(const String &type_name) const
{
    if (not this->canCast(type_name))
        sirenDebug() << "Cannot cast from type" << this->name() << "to" << type_name;
}

/** Assert that it is possible to cast this type to the class "type_name" */
void Class::assertCanCast(const char *type_name) const
{
    assertCanCast( String(type_name) );
}
