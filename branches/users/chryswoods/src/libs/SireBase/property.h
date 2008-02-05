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
#include <QSharedData>

SIRE_BEGIN_HEADER

namespace SireBase
{
class PropertyBase;
class Property;
class VariantProperty;
}

QDataStream& operator<<(QDataStream&, const SireBase::PropertyBase&);
QDataStream& operator>>(QDataStream&, SireBase::PropertyBase&);

QDataStream& operator<<(QDataStream&, const SireBase::VariantProperty&);
QDataStream& operator>>(QDataStream&, SireBase::VariantProperty&);

QDataStream& operator<<(QDataStream&, const SireBase::Property&);
QDataStream& operator>>(QDataStream&, SireBase::Property&);

XMLStream& operator<<(XMLStream&, const SireBase::Property&);
XMLStream& operator>>(XMLStream&, SireBase::Property&);

XMLStream& operator<<(XMLStream&, const SireBase::PropertyBase&);
XMLStream& operator>>(XMLStream&, SireBase::PropertyBase&);

QTextStream& operator<<(QTextStream&, const SireBase::PropertyBase&);
QTextStream& operator<<(QTextStream&, const SireBase::Property&);

namespace SireBase
{

class Property;

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

friend XMLStream& ::operator<<(XMLStream&, const PropertyBase&);
friend XMLStream& ::operator>>(XMLStream&, PropertyBase&);

public:
    PropertyBase();

    PropertyBase(const PropertyBase &other);

    virtual ~PropertyBase();

    virtual PropertyBase& operator=(const PropertyBase &other);

    PropertyBase& operator=(const Property &other);

    virtual bool operator==(const PropertyBase &other) const;
    virtual bool operator!=(const PropertyBase &other) const;

    bool operator==(const Property &other) const;
    bool operator!=(const Property &other) const;

    virtual PropertyBase* clone() const=0;
    virtual void copy(const PropertyBase &other)=0;
    virtual bool compare(const PropertyBase &other) const=0;

    virtual void save(QDataStream &ds) const=0;
    virtual void load(QDataStream &ds)=0;

    //virtual void save(XMLStream &xs) const=0;
    //virtual void load(XMLStream &xs)=0;

    virtual const char* what() const=0;

    static const char* typeName()
    {
        return "SireBase::PropertyBase";
    }

    template<class T>
    bool isA() const
    {
        return dynamic_cast<const T*>(this) != 0;
    }

    template<class T>
    const T& asA() const
    {
        const T* as_t = dynamic_cast<const T*>(this);
        
        if (not as_t)
            throwInvalidCast(T::typeName());
    
        return *as_t;
    }
    
    template<class T>
    T& asA()
    {
        T* as_t = dynamic_cast<T*>(this);
        
        if (not as_t)
            throwInvalidCast(T::typeName());
            
        return *as_t;
    }

protected:
    void throwInvalidCast(const PropertyBase &other) const;
    void throwInvalidCast(const char *typenam) const;
};

/** This is the second-to-top class of all Properties. Any instantiatable
    properties must inherit from this template class so that all of the
    virtual functions are supplied correctly, e.g. if you have a
    class called Derived, that inherits from Base, and Base derives
    from PropertyBase, then you need to inherit Derived from
    ConcreteProperty<Derived,Base>

    @author Christopher Woods
*/
template<class Derived, class Base>
class ConcreteProperty : public Base
{
public:
    ConcreteProperty() : Base()
    {}

    template<class T0>
    ConcreteProperty(const T0 &t0) : Base(t0)
    {}

    template<class T0, class T1>
    ConcreteProperty(const T0 &t0, const T1 &t1) : Base(t0,t1)
    {}

    template<class T0, class T1, class T2>
    ConcreteProperty(const T0 &t0, const T1 &t1,
                     const T2 &t2) : Base(t0,t1,t2)
    {}

    virtual ~ConcreteProperty()
    {}

    ConcreteProperty<Derived,Base>& operator=(const PropertyBase &other)
    {
        const Derived* other_t = dynamic_cast<const Derived*>(&other);

        if (!other_t)
            Base::throwInvalidCast(other);

        return static_cast<Derived*>(this)->operator=(*other_t);
    }

    bool operator==(const PropertyBase &other) const
    {
        const Derived* other_t = dynamic_cast<const Derived*>(&other);

        if (other_t)
            return static_cast<const Derived*>(this)->operator==(*other_t);
        else
            return false;
    }

    bool operator!=(const PropertyBase &other) const
    {
        const Derived* other_t = dynamic_cast<const Derived*>(&other);

        if (other_t)
            return static_cast<const Derived*>(this)->operator!=(*other_t);
        else
            return true;
    }

    const char* what() const
    {
        return Derived::typeName();
    }

    ConcreteProperty<Derived,Base>* clone() const
    {
        return new Derived( static_cast<const Derived&>(*this) );
    }

    void copy(const PropertyBase &other)
    {
        ConcreteProperty<Derived,Base>::operator=(other);
    }

    bool compare(const PropertyBase &other) const
    {
        return ConcreteProperty<Derived,Base>::operator==(other);
    }

    void save(QDataStream &ds) const
    {
        ds << static_cast<const Derived&>(*this);
    }

    void load(QDataStream &ds)
    {
        ds >> static_cast<Derived&>(*this);
    }

//     void save(XMLStream &) const
//     {
//         //xs << static_cast<const Derived&>(*this);
//     }
// 
//     void load(XMLStream &)
//     {
//         //xs >> static_cast<Derived&>(*this);
//     }

protected:
    ConcreteProperty<Derived,Base>&
    operator=(const ConcreteProperty<Derived,Base> &other)
    {
        Base::operator=(other);
        return *this;
    }

    bool operator==(const ConcreteProperty<Derived,Base> &other) const
    {
        return Base::operator==(other);
    }

    bool operator!=(const ConcreteProperty<Derived,Base> &other) const
    {
        return Base::operator!=(other);
    }
};

/** This is a simple property that holds any value as a QVariant. This
    is designed to be used for metadata that doesn't need any tight
    checking (e.g. the author of the molecule file, the source of
    the coordinates, the 'header' lines etc.)

    @author Christopher Woods
*/
class SIREBASE_EXPORT VariantProperty
          : public ConcreteProperty<VariantProperty,PropertyBase>, public QVariant
{
public:
    VariantProperty();

    VariantProperty(const QVariant &value);

    VariantProperty(const Property &other);

    VariantProperty(const VariantProperty &other);

    virtual ~VariantProperty();

    using PropertyBase::operator=;
    using PropertyBase::operator==;
    using PropertyBase::operator!=;

    VariantProperty& operator=(const QVariant &value);
    VariantProperty& operator=(const VariantProperty &other);

    bool operator==(const VariantProperty &other) const;
    bool operator!=(const VariantProperty &other) const;

    static const char* typeName()
    {
        return "SireBase::VariantProperty";
    }
};

/** This is the polymorphic pointer holder for the entire
    PropertyBase class hierarchy. This can hold implicitly 
    shared pointers to any property class.

    @author Christopher Woods
*/
class SIREBASE_EXPORT Property
{

friend QDataStream& ::operator<<(QDataStream&, const Property&);
friend QDataStream& ::operator>>(QDataStream&, Property&);

friend XMLStream& ::operator<<(XMLStream&, const Property&);
friend XMLStream& ::operator>>(XMLStream&, Property&);

public:
    Property();

    Property(const PropertyBase &property);
    
    Property(const Property &other);
    
    virtual ~Property();
    
    virtual Property& operator=(const PropertyBase &property);

    bool operator==(const Property &other) const;
    bool operator!=(const Property &other) const;

    const PropertyBase* operator->() const;
    const PropertyBase& operator*() const;
    
    const PropertyBase& read() const;
    PropertyBase& edit();
    
    bool isNull() const;

    const PropertyBase* data() const;
    const PropertyBase* constData() const;
    
    PropertyBase* data();
    
    operator const PropertyBase&() const;

protected:
    const PropertyBase& d() const;

    PropertyBase& d();

private:
    Property(const SharedPolyPointer<PropertyBase> &p);
    
    /** Shared pointer to the actual property */
    SharedPolyPointer<PropertyBase> ptr;
};

}

Q_DECLARE_METATYPE(SireBase::Property);
Q_DECLARE_METATYPE(SireBase::VariantProperty);

SIRE_END_HEADER

#endif
