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

#ifndef SIREBASE_PROPERTY_H
#define SIREBASE_PROPERTY_H

#include "sharedpolypointer.hpp"

#include <QVariant>

SIRE_BEGIN_HEADER

namespace SireBase
{
class PropertyBase;
class Property;
}

QDataStream& operator<<(QDataStream&, const SireBase::PropertyBase&);
QDataStream& operator>>(QDataStream&, SireBase::PropertyBase&);

QDataStream& operator<<(QDataStream&, const SireBase::Property&);
QDataStream& operator>>(QDataStream&, SireBase::Property&);

namespace SireBase
{

class Property;
class Molecule;

/** This is the base class of all properties that may be attached to a
    molecule. Properties are used to assign extra information to a molecule,
    which may then be carried by the molecule throughout its passage
    through the simulation. Examples of properties may include the file
    from which the molecule was read, the charge parameters on the atoms,
    the PDB code etc.

    Properties form a polymorphic hierarchy which are implicitly shared
    via SireBase::SharedPolyPointer smart pointers.

    @author Christopher Woods
*/
class SIREBASE_EXPORT PropertyBase : public QSharedData
{

friend QDataStream& ::operator<<(QDataStream&, const PropertyBase&);
friend QDataStream& ::operator>>(QDataStream&, PropertyBase&);

public:
    PropertyBase();

    PropertyBase(const PropertyBase &other);

    virtual ~PropertyBase();

    PropertyBase& operator=(const PropertyBase &other);

    virtual PropertyBase* clone() const=0;

    virtual const char* what() const=0;

    static Property null_property();

    virtual bool isCompatibleWith(const Molecule &molecule) const=0;

    void assertCompatibleWith(const Molecule &molecule) const;

    template<class T>
    bool isA() const
    {
        return dynamic_cast<const T*>(this) != 0;
    }

    template<class T>
    const T& asA() const
    {
        return dynamic_cast<const T&>(*this);
    }
};

/** This is a simple property that holds any value as a QVariant. This
    is designed to be used for metadata that doesn't need any tight
    checking (e.g. the author of the molecule file, the source of
    the coordinates, the 'header' lines etc.)

    @author Christopher Woods
*/
class SIREBASE_EXPORT VariantProperty : public PropertyBase, public QVariant
{
public:
    VariantProperty();

    VariantProperty(const QVariant &value);

    VariantProperty(const Property &other);

    VariantProperty(const VariantProperty &other);

    ~VariantProperty();

    VariantProperty& operator=(const QVariant &value);
    VariantProperty& operator=(const Property &property);
    VariantProperty& operator=(const VariantProperty &other);

    static const char* typeName()
    {
        return "SireBase::VariantProperty";
    }

    const char* what() const
    {
        return VariantProperty::typeName();
    }

    VariantProperty* clone() const
    {
        return new VariantProperty(*this);
    }

    /** A variant property is compatible with everything! */
    bool isCompatibleWith(const Molecule&) const
    {
        return true;
    }
};

/** This is the visible holder class for PropertyBase. This is just
    a thin wrapper around SireBase::SharedPolyPointer<PropertyBase>
    that has extra tests that ensure that this is never null.

    @author Christopher Woods
*/
class SIREBASE_EXPORT Property
{

friend QDataStream& ::operator<<(QDataStream&, const Property&);
friend QDataStream& ::operator>>(QDataStream&, Property&);

public:
    Property();

    Property(const SireBase::SharedPolyPointer<PropertyBase> &ptr);

    Property(const PropertyBase &property);
    Property(PropertyBase *property);

    Property(const Property &other);

    ~Property();

    Property& operator=(const Property &other);

    const PropertyBase& base() const
    {
        return *d;
    }

    template<class T>
    bool isA() const
    {
        return d->isA<T>();
    }

    template<class T>
    const T& asA() const
    {
        return d->asA<T>();
    }

private:
    /** Shared pointer to the actual property */
    SharedPolyPointer<PropertyBase> d;
};

}

Q_DECLARE_METATYPE(SireBase::Property);
Q_DECLARE_METATYPE(SireBase::VariantProperty);

SIRE_END_HEADER

#endif
