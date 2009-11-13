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

#include <QMutex>

#include "class.h"

#include "Siren/errors.h"

using namespace Siren;

void Class::buildInheritedTypes()
{
    if (not metatype)
        return;

    if (super_class)
        inherited_types = super_class->inherited_types;

    inherited_types.insert( metatype->typeName() );
    
    foreach (const QString &interface, interface_types )
    {
        inherited_types.insert( interface );
    }
}

typedef QHash<QString,const Class*> ClassRegistryType;

Q_GLOBAL_STATIC( ClassRegistryType, registered_types );

/** Null constructor */
Class::Class() 
      : Implements<Class, Object>(), metatype(0), super_class(0),
        is_concrete(false)
{}

/** Internal constructor used by Object to create its class type */
Class::Class(const detail::RegisterMetaTypeBase *r)
      : Implements<Class, Object>(), metatype(r), super_class(0),
        is_concrete(false)
{
    buildInheritedTypes();
    registered_types()->insert( this->name(), this );
}

/** Internal constructor used by Object to create a derived class type */
Class::Class(const detail::RegisterMetaTypeBase *r, 
             const Class &base_class, const QStringList &interfaces, 
             bool concrete)
      : Implements<Class, Object>(), metatype(r), super_class(&base_class),
        interface_types(interfaces), is_concrete(concrete)
{
    buildInheritedTypes();
    registered_types()->insert( this->name(), this );
}

/** Construct the class type object for the passed object */
Class::Class(const Object &object) : Implements<Class, Object>()
{
    this->operator=( object.getClass() );
}

/** Construct the Class object for the type with name 'type_name' 

    \throw Siren::unknown_type
*/
Class::Class(const QString &type_name) : Implements<Class, Object>()
{
    if (type_name.isEmpty())
    {
        this->operator=( None().getClass() );
    }
    else if (registered_types()->contains(type_name))
    {
        this->operator=( *(registered_types()->value(type_name)) );
    }
    else
    {
        //get the type that represents this name
        int id = QMetaType::type( type_name.toLatin1().constData() );

        if ( id == 0 or not QMetaType::isRegistered(id) )
            throw Siren::unknown_type( QObject::tr(
                  "Cannot create the Class for type \"%1\". "
                  "This could be because either this is not a concrete type, "
                  "or that the library or module containing "
                  "this type has been loaded and that it has been registered "
                  "with QMetaType.").arg(type_name), CODELOC );

        //create a default-constructed object of this type
        boost::shared_ptr<Object> ptr;
        
        ptr.reset( static_cast<Object*>(QMetaType::construct(id,0)) );

        if (ptr.get() == 0)
            throw Siren::program_bug( QObject::tr(
                    "Could not create an object of type \"%1\" despite "
                    "this type having been registered with QMetaType. This is "
                    "a program bug!!!").arg(type_name), CODELOC );

        this->operator=( ptr->getClass() );
    }
}

/** Copy constructor */
Class::Class(const Class &other)
      : Implements<Class, Object>(other),
        metatype(other.metatype), super_class(other.super_class),
        interface_types(other.interface_types),
        inherited_types(other.inherited_types),
        is_concrete(other.is_concrete)
{}

/** Destructor */
Class::~Class()
{}

/** Copy assignment operator */
Class& Class::operator=(const Class &other)
{
    if (this != &other)
    {
        Object::operator=(other);
        
        metatype = other.metatype;
        super_class = other.super_class;
        interface_types = other.interface_types;
        inherited_types = other.inherited_types;
        is_concrete = other.is_concrete;
    }
    
    return *this;
}

/** Comparison operator */
bool Class::operator==(const Class &other) const
{
    return metatype == other.metatype and super_class == other.super_class;
}

/** Comparison operator */
bool Class::operator!=(const Class &other) const
{
    return not Class::operator==(other);
}

/** Return the name of the class */
QString Class::name() const
{
    if (metatype)
        return metatype->typeName();
    else
        return QString::null;
}

/** Return whether or not this class has a superclass */
bool Class::hasSuper() const
{
    return super_class != 0;
}

/** Return the superclass of this type - this returns the 
    same type if it doesn't have a super class */
const Class& Class::super() const
{
    if (super_class)
        return *super_class;
    else
        return *this;
}

/** Return whether or not this class implements the type 'type_name' */
bool Class::implements(const QString &type_name) const
{
    return inherited_types.contains(type_name);
}

/** Return all of the direct interfaces for this type */
QStringList Class::interfaces() const
{
    return interface_types;
}

/** Return the UID for this class */
CLASS_UID Class::UID() const
{
    if (metatype)
        return metatype->UID();
    else
        return 0;
}

/** Return the version number of this class */
VERSION_ID Class::version() const
{
    if (metatype)
        return metatype->version();
    else
        return 0;
}

/** Create a default-constructed object of this class.
    This is only possible if this is a concrete class
    (isConcrete() returns true)
    
    \throw Siren::invalid_operation
*/
ObjRef Class::create() const
{
    if (not is_concrete)
        throw Siren::invalid_operation( QObject::tr(
                "You cannot create an object of type %1 as it is not "
                "concrete (it is virtual).")
                    .arg(this->name()), CODELOC );

    int type_id = QMetaType::type( this->name().toAscii().constData() );
    
    if (not type_id)
        throw Siren::program_bug( QObject::tr(
                "You cannot create an object of type %1 as it does not "
                "appear to be registered with QMetaType - has the programmer "
                "registered this type using RegisterMetaType<%1>()?")
                    .arg(this->name()), CODELOC );

    Object *obj = static_cast<Object*>( QMetaType::construct(type_id) );
    
    return ObjRef(obj);
}

/** Return whether or not we can create an object of this type */
bool Class::canCreate() const
{
    return is_concrete;
}

/** Return whether or not this is a concrete type */
bool Class::isConcrete() const
{
    return is_concrete;
}

/** Return whether or not this class can be cast to 'classname' */
bool Class::canCast(const QString &classname) const
{
    return inherited_types.contains(classname);
}

/** Assert that it is possible to cast from this class to 'classname'

    \throw Siren::invalid_cast
*/
void Class::assertCanCast(const QString &classname) const
{
    if (not this->canCast(classname))
        throw Siren::invalid_cast( QObject::tr(
                "It is not possible to cast from type %1 to type %2.")
                    .arg(this->name(), classname), CODELOC );
}

HASH_CODE Class::hashCode() const
{
    return qHash( this->name() ); 
}
