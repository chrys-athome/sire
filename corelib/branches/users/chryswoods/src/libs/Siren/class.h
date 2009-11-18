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

#ifndef SIREN_CLASS_H
#define SIREN_CLASS_H

#include <QSet>
#include <QStringList>

#include "object.h"

SIREN_BEGIN_HEADER

namespace Siren
{

class HanRef;

/** This class is used to provide type-information about
    each of the Siren::Object derived classes
    
    @author Christopher Woods
*/
class SIREN_EXPORT Class : public Implements<Class,Object>
{

public:
    Class();
    
    Class(const Object &object);

    Class(const QString &type_name);
    
    Class(const Class &other);
    
    ~Class();
    
    Class& operator=(const Class &other);
    
    bool operator==(const Class &other) const;
    bool operator!=(const Class &other) const;
    
    HASH_CODE hashCode() const;

    QString toString() const;
    
    QString name() const;
    
    bool hasSuper() const;
    
    const Class& super() const;
    
    bool implements(const QString &type_name) const;

    template<class T>
    bool implements() const;
    
    QStringList interfaces() const;
    
    ObjRef createObject() const;
    HanRef createHandle() const;

    bool canCreate() const;
    
    bool isConcrete() const;
    
    bool isObject() const;
    bool isHandle() const;
    bool isPrimitive() const;
    
    bool canCast(const QString &classname) const;

    void assertCanCast(const QString &classname) const;
    
    template<class T>
    bool canCast() const;
    
    template<class T>
    void assertCanCast() const;

protected:
    bool test(Logger &logger) const;
    void stream(Stream &s);

private:
    friend class Object;   // so can call these constructors
    friend class Handle;   // so can call these constructors
    
    Class(const detail::RegisterMetaTypeBase *r);

    Class(const detail::RegisterMetaTypeBase *r, 
          const Class &base_class, const QStringList &interfaces, 
          bool is_concrete);
          
    void buildInheritedTypes();
          
    /** Pointer to the metatype for this class */
    const detail::RegisterMetaTypeBase *metatype;
          
    /** Pointer to this types superclass (null if this has
        no superclass) */
    const Class *super_class;

    /** The interfaces implemented directly in this type */
    QStringList interface_types;
    
    /** The complete set of all types inherited from and 
        implemented in this type */
    QSet<QString> inherited_types;
    
    /** Whether or not this is a concrete type */
    bool is_concrete;
    
    /** Whether or not this is derived from Siren::Object */
    bool is_object;
    
    /** Whether or not this is derived from Siren::Handle */
    bool is_handle;
    
    /** Whether or not this is a primitive type */
    bool is_primitive;
};

/** Return whether or not this class implements the type 'T' */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
bool Class::implements() const
{
    return this->implements( T::typeName() );
}

/** Return whether or not this class can be cast to type 'T' */
template<class T>
bool Class::canCast() const
{
    return this->canCast( T::typeName() );
}

/** Assert that it is possible to cast this class to type 'T'

    \throw Siren::invalid_cast
*/
template<class T>
void Class::assertCanCast() const
{
    this->assertCanCast( T::typeName() );
}

}

Q_DECLARE_METATYPE( Siren::Class )

SIREN_EXPOSE_CLASS( Siren::Class )

SIREN_END_HEADER

#endif
