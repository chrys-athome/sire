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
#include <QDebug>

SIRE_BEGIN_HEADER

namespace SireBase
{
class Property;
class VariantProperty;
class NullProperty;

class PropPtrBase;

template<class T>
class PropPtr;

}

QDataStream& operator<<(QDataStream&, const SireBase::Property&);
QDataStream& operator>>(QDataStream&, SireBase::Property&);

QDataStream& operator<<(QDataStream&, const SireBase::VariantProperty&);
QDataStream& operator>>(QDataStream&, SireBase::VariantProperty&);

QDataStream& operator<<(QDataStream&, const SireBase::NullProperty&);
QDataStream& operator>>(QDataStream&, SireBase::NullProperty&);

QDataStream& operator<<(QDataStream&, const SireBase::PropPtrBase&);
QDataStream& operator>>(QDataStream&, SireBase::PropPtrBase&);

template<class T>
QDataStream& operator<<(QDataStream&, const SireBase::PropPtr<T>&);
template<class T>
QDataStream& operator>>(QDataStream&, SireBase::PropPtr<T>&);

class QMutex;

namespace SireBase
{

QMutex* globalLock();

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
class SIREBASE_EXPORT Property : public QSharedData
{

friend QDataStream& ::operator<<(QDataStream&, const Property&);
friend QDataStream& ::operator>>(QDataStream&, Property&);

public:
    Property();

    Property(const Property &other);

    virtual ~Property();

    virtual Property* clone() const=0;
    virtual void copy(const Property &other)=0;
    virtual bool equals(const Property &other) const=0;

    virtual void save(QDataStream &ds) const=0;
    virtual void load(QDataStream &ds)=0;

    //virtual void save(XMLStream &xs) const=0;
    //virtual void load(XMLStream &xs)=0;

    virtual const char* what() const=0;

    static const char* typeName()
    {
        return "SireBase::Property";
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

    static const NullProperty& null();

protected:
    Property& operator=(const Property &other);

    bool operator==(const Property &other) const;
    bool operator!=(const Property &other) const;

    void throwInvalidCast(const Property &other) const;
    void throwInvalidCast(const char *typenam) const;
};

/** This is the second-to-top class of all Properties. Any instantiatable
    properties must inherit from this template class so that all of the
    virtual functions are supplied correctly, e.g. if you have a
    class called Derived, that inherits from Base, and Base derives
    from Property, then you need to inherit Derived from
    ConcreteProperty<Derived,Base>

    @author Christopher Woods
*/
template<class Derived, class Base>
class SIREBASE_EXPORT ConcreteProperty : public Base
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

    ConcreteProperty<Derived,Base>& operator=(const Property &other)
    {
        const Derived* other_t = dynamic_cast<const Derived*>(&other);

        if (!other_t)
            Base::throwInvalidCast(other);

        return static_cast<Derived*>(this)->operator=(*other_t);
    }

    bool operator==(const Property &other) const
    {
        const Derived* other_t = dynamic_cast<const Derived*>(&other);

        if (other_t)
            return static_cast<const Derived*>(this)->operator==(*other_t);
        else
            return false;
    }

    bool operator!=(const Property &other) const
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

    void copy(const Property &other)
    {
        ConcreteProperty<Derived,Base>::operator=(other);
    }

    bool equals(const Property &other) const
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
          : public ConcreteProperty<VariantProperty,Property>, public QVariant
{
public:
    VariantProperty();

    VariantProperty(const QVariant &value);

    VariantProperty(const Property &other);

    VariantProperty(const VariantProperty &other);

    virtual ~VariantProperty();

    using Property::operator=;
    using Property::operator==;
    using Property::operator!=;

    VariantProperty& operator=(const QVariant &value);
    VariantProperty& operator=(const VariantProperty &other);

    bool operator==(const VariantProperty &other) const;
    bool operator!=(const VariantProperty &other) const;

    static const char* typeName()
    {
        return "SireBase::VariantProperty";
    }
    
    template<class T>
    T convertTo() const
    {
        if (not this->canConvert<T>())
            this->throwInvalidCast( QMetaType::typeName( qMetaTypeId<T>() ) );
            
        return this->value<T>();
    }

private:
    void throwInvalidCast(const QString &typname) const;
};

/** This is a null property */
class SIREBASE_EXPORT NullProperty
              : public ConcreteProperty<NullProperty,Property>
{
public:
    NullProperty();
    
    NullProperty(const NullProperty &other);

    ~NullProperty();

    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<NullProperty>() );
    }
};

/** This is base class of the polymorphic pointer holder for the entire
    Property class hierarchy. This can hold implicitly 
    shared pointers to any property class.

    @author Christopher Woods
*/
class SIREBASE_EXPORT PropPtrBase
{

friend QDataStream& ::operator<<(QDataStream&, const PropPtrBase&);
friend QDataStream& ::operator>>(QDataStream&, PropPtrBase&);

public:
    ~PropPtrBase();

    bool operator==(const PropPtrBase &other) const;
    bool operator!=(const PropPtrBase &other) const;

    void detach();

    operator const Property&() const;

protected:
    PropPtrBase(const Property &property);
    
    PropPtrBase(const PropPtrBase &other);

    PropPtrBase& operator=(const PropPtrBase &other);

    const Property& read() const;
    Property& edit();

    static void throwCastingError(const char *got_type, const char *want_type);

private:
    /** Shared pointer to the actual property */
    SharedPolyPointer<Property> ptr;
};

/** This is the specialised pointer that is used to hold a hierarchy
    of properties that are derived from type 'T'
    
    @author Christopher Woods
*/
template<class T>
class SIREBASE_EXPORT PropPtr : public PropPtrBase
{

friend QDataStream& ::operator<<<>(QDataStream&, const PropPtr<T>&);
friend QDataStream& ::operator>><>(QDataStream&, PropPtr<T>&);

public:
    PropPtr();

    PropPtr(const T &obj);
    PropPtr(const Property &property);

    PropPtr(const PropPtrBase &other);

    PropPtr(const PropPtr<T> &other);
    
    ~PropPtr();
    
    PropPtr<T>& operator=(const T &obj);
    PropPtr<T>& operator=(const Property &property);

    PropPtr<T>& operator=(const PropPtr<T> &other);
    PropPtr<T>& operator=(const PropPtrBase &property);

    const T* operator->() const;
    const T& operator*() const;
    
    const T& read() const;
    T& edit();

    const T* data() const;
    const T* constData() const;

    T* data();
    
    operator const T&() const;

    bool isNull() const;

    static PropPtr<T> null();

protected:
    void assertSane() const;
};

/** Assert that this is sane - this is to make sure that the 
    Property really is derived from 'T' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void PropPtr<T>::assertSane() const
{
    const Property &p = PropPtrBase::read();

    if (not p.isA<T>())
    {
        PropPtrBase::throwCastingError( p.what(), T::typeName() );
    }
}

/** Return the null object */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
PropPtr<T> PropPtr<T>::null()
{
    return PropPtr<T>( T::null() );
}

/** Return whether this is the null object */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool PropPtr<T>::isNull() const
{
    return PropPtr<T>::operator=( T::null() );
}

/** Null constructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
PropPtr<T>::PropPtr() : PropPtrBase( PropPtr<T>::null() )
{}

/** Construct from the object 'obj' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
PropPtr<T>::PropPtr(const T &obj) : PropPtrBase(obj)
{}

/** Construct from the passed property

    \throw SireError::invalid_cast
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
PropPtr<T>::PropPtr(const Property &property) : PropPtrBase(property)
{
    this->assertSane();
}

/** Construct from a pointer of another type */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
PropPtr<T>::PropPtr(const PropPtrBase &other) : PropPtrBase(other)
{
    this->assertSane();
}

/** Copy constructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
PropPtr<T>::PropPtr(const PropPtr<T> &other) : PropPtrBase(other)
{}

/** Destructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
PropPtr<T>::~PropPtr()
{}

/** Copy assignment operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
PropPtr<T>& PropPtr<T>::operator=(const PropPtr<T> &other)
{
    PropPtrBase::operator=(other);
    return *this;
}

/** Create a pointer that points to the object 'obj' (or a copy, if this
    object is on the stack) */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
PropPtr<T>& PropPtr<T>::operator=(const T &obj)
{
    return this->operator=( PropPtr<T>(obj) );
}

/** Construct from the passed property

    \throw SireError::invalid_cast
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
PropPtr<T>& PropPtr<T>::operator=(const Property &property)
{
    return this->operator=( PropPtr<T>(property) );
}

/** Construct from the passed pointer to a property */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
PropPtr<T>& PropPtr<T>::operator=(const PropPtrBase &property)
{
    return this->operator=( PropPtr<T>(property) );
}

/** Return a reference to the object */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const T& PropPtr<T>::read() const
{
    //This is only safe as everything in this class ensures that 
    //the base pointer really is a pointer to the derived type 'T' */
    return static_cast<const T&>( PropPtrBase::read() );
}

/** Return an editable reference to the object */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
T& PropPtr<T>::edit()
{
    //This is only safe as everything in this class ensures that 
    //the base pointer really is a pointer to the derived type 'T' */
    return static_cast<T&>( PropPtrBase::edit() );
}

/** Return the raw pointer to the object */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const T* PropPtr<T>::operator->() const
{
    return &(this->read());
}

/** Return a reference to the object */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const T& PropPtr<T>::operator*() const
{
    return this->read();
}

/** Return the raw pointer to the object */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const T* PropPtr<T>::data() const
{
    return &(this->read());
}

/** Return the raw pointer to the object */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const T* PropPtr<T>::constData() const
{
    return &(this->read());
}

/** Return a modifiable pointer to the data */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
T* PropPtr<T>::data()
{
    return &(this->edit());
}

/** Allow automatic casting to a T() */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
PropPtr<T>::operator const T&() const
{
    return this->read();
}

/** This is the specialised pointer that is used to hold a basic
    Property object
    
    @author Christopher Woods
*/
template<>
class SIREBASE_EXPORT PropPtr<Property> : public PropPtrBase
{
public:
    PropPtr();

    PropPtr(const Property &property);

    PropPtr(const PropPtrBase &other);

    PropPtr(const PropPtr<Property> &other);
    
    ~PropPtr();
    
    PropPtr<Property>& operator=(const Property &property);

    PropPtr<Property>& operator=(const PropPtr<Property> &other);
    PropPtr<Property>& operator=(const PropPtrBase &property);

    const Property* operator->() const;
    const Property& operator*() const;
    
    const Property& read() const;
    Property& edit();

    const Property* data() const;
    const Property* constData() const;

    Property* data();
    
    operator const Property&() const;

    bool isNull() const;

    static PropPtr<Property> null();

    void assertSane() const
    {}
};

/** Create the type to hold a pointer to a generic Property */
typedef PropPtr<Property> PropertyPtr;

} // end of namespace SireBase

/** Serialise a property pointer to a binary datastream */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
QDataStream& operator<<(QDataStream &ds, const SireBase::PropPtr<T> &prop)
{
    ds << static_cast<const SireBase::PropPtrBase&>(prop);
    return ds;
}

/** Read a property pointer from a binary datastream */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
QDataStream& operator>>(QDataStream &ds, SireBase::PropPtr<T> &prop)
{
    SireBase::PropPtr<T> ptr;
    
    ds >> static_cast<SireBase::PropPtrBase&>(ptr);
    
    ptr.assertSane();
    
    prop = ptr;
    
    return ds;
}

Q_DECLARE_METATYPE(SireBase::NullProperty);
Q_DECLARE_METATYPE(SireBase::VariantProperty);

SIRE_EXPOSE_CLASS( SireBase::Property )
SIRE_EXPOSE_CLASS( SireBase::NullProperty )
SIRE_EXPOSE_CLASS( SireBase::VariantProperty )

SIRE_EXPOSE_PROPERTY( SireBase::PropertyPtr, SireBase::Property )

SIRE_END_HEADER

#endif
