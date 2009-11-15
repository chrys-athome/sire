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

#ifndef SIREN_OBJECT_H
#define SIREN_OBJECT_H

#include "sirenglobal.h"

#include <QSharedData>
#include <QMutex>

#include <boost/assert.hpp>

SIREN_BEGIN_HEADER

namespace Siren
{
class Object;
class Class;

class None;

class DataStream;
class XMLStream;

class Logger;

template<class Derived,class Base> class Implements;

class ObjRef;
}

class QDomNode;
class QMutex;

using Siren::XMLStream;
using Siren::DataStream;

QDataStream& operator<<(QDataStream&, const Siren::Object&);
QDataStream& operator>>(QDataStream&, Siren::Object&);

DataStream& operator<<(DataStream&, const Siren::Object&);
DataStream& operator>>(DataStream&, Siren::Object&);

XMLStream& operator<<(XMLStream&, const Siren::Object&);
XMLStream& operator>>(XMLStream&, Siren::Object&);

namespace Siren
{

namespace detail
{
template<class T> class SharedPolyPointerHelper;
template<class T> class SharedPolyPointer;

class GlobalSharedPointerBase;
template<class T> class GlobalSharedPointer;
}

QMutex& globalRegistrationLock();

/** This is the base class of all Siren virtual objects.
    
    All Siren objects are inherited from either this class
    (if they have virtual functions or wish to use metatyping
     or serialization), or, if they are basic, they are inherited
    from Siren::Primitive
    
    A Siren object provides the single root of all non-primitive,
    non-handle objects, together with a .getClass() function that returns
    a Siren::Class object that contains information about the type.
    
    In addition, all Siren::Objects support in-place unit testing,
    with each object containing a .test() method, e.g.
    
    obj.test();
    
    or
    
    obj.test( logger );
    
    This returns true if all tests passed. This allows unit tests
    to be written with the object, in the same implementation
    file as the object (and to have full access to the private
    data of the object).
    
    Object derived classes have three constraints;
    
      (1) They must have a well-defined state. This means that their
          state must be savable, and can be restored at a later point.
          This means that temporal data cannot be placed into an object
          (as the state of a clock cannot be saved, then restored, as then
           its time would be wrong! - however, if the current time was not
           part of the Clocks state, then it could be saved and restored)
    
      (2) Their state must not rely on shared data. This is because the  
          state of an Object can only be changed via its public interface.
          If it could contain shared data, then its state could be changed
          by any other object that contains a reference to that shared data.
          If you want to hold shared data, then you need to use the 
          Handle classes. (note that an Object can hold a Handle to 
          some shared data if, and only if, this shared data is not
          part of the state of the object, e.g. if the handle was to a
          Node one which the object will be computed, or to a Logger
          to which output should be directed.
          
      (3) All Object derived classes *MUST* be able to be saved and restored
          to binary and XML.
    
    Finally, a full metatype system is used, so that objects can 
    be constructed, cloned, queried, and serialized and deserialized 
    to/from binary streams and saved/restored to XML without any 
    knowledge of the type. This is achieved using;

    obj.what();    // return the type name of this object

    obj.getClass();   // Return information about the type of this object

    obj.getClass().super();  // Return information about the superclass type
    
    obj.getClass().UID();  // Return the unique ID number for this class type
    
    obj.getClass().version();  // Return the version number for the data in 
                               // this class (used for serialisation)
    
    ObjRef new_obj = obj.copy();  // clone the current object
    
    ObjRef new_obj = obj.getClass().create();  // create a new object
    
    obj.isA<T>();   // return whether 'obj' is of type 'T'
    
    obj.asA<T>();   // return 'obj' cast as an object of type 'T'
    
    DataStream ds;
    
    ds << obj;   // save 'obj' to a binary stream
    
    ObjRef new_obj;
    ds >> new_obj;   // read 'new_obj' from a binary stream
    
    XMLStream xds;
    
    xds << obj;   // save 'obj' to an XML stream
    
    xds >> new_obj;  // read 'new_obj' from an XML stream
    
    You may not multiply inherit from Object. There must be a single
    inheritance path back to Object. The only exception are that
    you can multiply inherit Object with types derived from 
    Siren::Interface. Siren::Interface provides the basis of
    interface types, which are pure virtual base classes with
    no member data (similar to Interfaces in Java).
    
    Siren::Object is a virtual class. You must inherit from
    Implements<D,B>, where D is your new concrete class and 
    B is the class from which it inherits.
    
    For example, if NewObj is a concrete class derived from Object,
    the it should inherit from Implements<NewObj,Object>. 
    The Implements class does all of the hard work of 
    filling in the virtual functions of Object.
    
    If you are deriving a new virtual class from Object, then 
    you must inherit from Extends<D,B>, e.g. if NewObj is a 
    virtual class derived from Object, then it should inherit
    from Extends<NewObj,Object>.
    
    Each Siren::Object is immutable (unchangeable). If you want
    to create a mutable object, you *must* inherit from the
    Mutable interface. This will provide you with functions
    that will allow you to save and restore the state of
    the mutable object (e.g. to protect an invariant if
    an exception is thrown).
    
    For example, let imagine Foo is a concrete class derived
    from Bar, and Foo implements the Mutable interface. Also, 
    Bar is a virtual class derived from Object. The inheritance
    hierarchy would look like this;
    
    class Bar : public Extends<Bar,Object>{ ... };
    
    class Foo : public Implements<Foo,Bar>, public Interfaces<Foo,Mutable> { ... };
    
    Each Siren::Object must also be non-shared. This means that
    as long as access to the object is protected by a mutex,
    then the object is thread-safe. This also means that the 
    object cannot be changed 'behind-the-back', e.g. a shared
    object can be changed using any of the references/handles to that object.

    If you want a shared object, then you will need to
    create a class that is based on Siren::Handle.
    
    @author Christopher Woods
*/
class SIREN_EXPORT Object : private QSharedData
{
public:
    Object();
    Object(const Object &other);
    
    virtual ~Object();

    /** Reimplement this function to return the Class
        that represents the type of this object */
    virtual const Class& getClass() const=0;

    static QString typeName();

    /** Reimplement this function to return the 
        fully qualified type name of this object
        (same as object.getClass().fullName()) */
    virtual QString what() const=0;

    virtual ObjRef copy() const;
    
    virtual QString toString() const;

    virtual bool test() const;
    virtual bool test(Logger &logger) const;

    /** Reimplement this function to copy the 
        contents of 'other' into this object */
    virtual void copy(const Object &other)=0;

    /** Reimplement this function to return whether
        or not this object equals (by value) 'other' */
    virtual bool equals(const Object &other) const=0;
    
    /** Reimplement this function to return a hash
        code for this object - this is necessary to 
        allow this object to be used in a hash/dictionary. 
    
        For example, use the following algorithm
        (from Effective Java by Joshua Bloch)
        
        Start with a number, e.g. result = 17;
        
        For each significant field (f), calculate a hash
        code (c) for that field
        
        Field type                  Calculation
        bool                        c = (f ? 0 : 1)
        byte, char, short, int      c = (int)f
        long                        c = (int)(f ^ (f >> 32))
        float                       c = Float.floatToIntBits(f)
        double                      l = Double.doubleToLongBits(f)  c = hash(l)
        object                      c = object.hashCode()
        Array / container           c = sum( hash(contents) )
        
        Then return the hash code hash = 37 * (result + sum(c))
    */
    virtual HASH_CODE hashCode() const=0;

    template<class T>
    bool isA() const;

    template<class T>
    const T& asA() const;

protected:
    static const Class& createTypeInfo();

    static QMutex& globalLock();

    static void throwUnregisteredMetaTypeError(const QString &type_name);
    
    static void throwInvalidCast(const QString &from_type, const QString &to_type);

    static Class* registerConcreteClass( const detail::RegisterMetaTypeBase *r,
                                         const Class &base_class,
                                         const QStringList &interfaces);
                                        
    static Class* registerVirtualClass( const detail::RegisterMetaTypeBase *r,
                                        const Class &base_class,
                                        const QStringList &interfaces );

    static QStringList listInterfaces();

    /** Return a clone of this object. */
    virtual Object* clone() const=0;

    virtual void load(const QDomNode &dom);
    virtual void save(QDomNode &dom) const;

    virtual void load(DataStream &ds);
    virtual void save(DataStream &ds) const;

    Object& operator=(const Object &other);
    
    bool operator==(const Object &other) const;
    bool operator!=(const Object &other) const;

private:
    friend class detail::SharedPolyPointerHelper<Object>;
    friend class detail::SharedPolyPointer<Object>;
    friend class detail::GlobalSharedPointer<Object>;
    friend class detail::GlobalSharedPointerBase;

    friend DataStream& ::operator<<(DataStream&, const Object&);
    friend DataStream& ::operator>>(DataStream&, Object&);

    friend XMLStream& ::operator<<(XMLStream&, const Object&);
    friend XMLStream& ::operator>>(XMLStream&, Object&);

    friend void qAtomicAssign(Object *&d, Object *x);

    bool private_implements(const QString &class_type) const;
    void private_assertCanCast(const QString &class_type) const;

    static Class* class_typeinfo;
};

/** Derive virtual classes using this template type */
template<class Derived, class Base>
class Extends : public Base
{
public:
    Extends();

    template<class T0>
    Extends(const T0 &t0);

    template<class T0, class T1>
    Extends(const T0 &t0, const T1 &t1);

    template<class T0, class T1, class T2>
    Extends(const T0 &t0, const T1 &t1, const T2 &t2);

    template<class T0, class T1, class T2, class T3>
    Extends(const T0 &t0, const T1 &t1, const T2 &t2, const T3 &t3);

    template<class T0, class T1, class T2, class T3, class T4>
    Extends(const T0 &t0, const T1 &t1, const T2 &t2, const T3 &t3,
                   const T4 &t4);

    template<class T0, class T1, class T2, class T3, class T4, class T5>
    Extends(const T0 &t0, const T1 &t1, const T2 &t2, const T3 &t3,
                   const T4 &t4, const T5 &t5);

    template<class T0, class T1, class T2, class T3, class T4, 
             class T5, class T6>
    Extends(const T0 &t0, const T1 &t1, const T2 &t2, const T3 &t3,
                   const T4 &t4, const T5 &t5, const T6 &t6);

    template<class T0, class T1, class T2, class T3, class T4, 
             class T5, class T6, class T7>
    Extends(const T0 &t0, const T1 &t1, const T2 &t2, const T3 &t3,
                   const T4 &t4, const T5 &t5, const T6 &t6, const T7 &t7);

    ~Extends();

protected:
    static const Class& createTypeInfo();

    Base& super();
    const Base& super() const;

private:
    static const Class* class_typeinfo;
};

/** Derive concrete classes using this template type */
template<class Derived, class Base>
class Implements : public Base
{
public:
    Implements();

    template<class T0>
    Implements(const T0 &t0);

    template<class T0, class T1>
    Implements(const T0 &t0, const T1 &t1);

    template<class T0, class T1, class T2>
    Implements(const T0 &t0, const T1 &t1, const T2 &t2);

    template<class T0, class T1, class T2, class T3>
    Implements(const T0 &t0, const T1 &t1, const T2 &t2, const T3 &t3);

    template<class T0, class T1, class T2, class T3, class T4>
    Implements(const T0 &t0, const T1 &t1, const T2 &t2, const T3 &t3,
                   const T4 &t4);

    template<class T0, class T1, class T2, class T3, class T4, class T5>
    Implements(const T0 &t0, const T1 &t1, const T2 &t2, const T3 &t3,
                   const T4 &t4, const T5 &t5);

    template<class T0, class T1, class T2, class T3, class T4, 
             class T5, class T6>
    Implements(const T0 &t0, const T1 &t1, const T2 &t2, const T3 &t3,
                   const T4 &t4, const T5 &t5, const T6 &t6);

    template<class T0, class T1, class T2, class T3, class T4, 
             class T5, class T6, class T7>
    Implements(const T0 &t0, const T1 &t1, const T2 &t2, const T3 &t3,
                   const T4 &t4, const T5 &t5, const T6 &t6, const T7 &t7);

    virtual ~Implements();

    Implements<Derived,Base>& operator=(const Object &other);

    bool operator==(const Object &other) const;

    bool operator!=(const Object &other) const;

    static QString typeName();

    QString what() const;

    ObjRef copy() const;

    const Class& getClass() const;

    void copy(const Object &other);
    bool equals(const Object &other) const;

protected:
    static const Class& createTypeInfo();

    Implements<Derived,Base>* clone() const;

    Implements<Derived,Base>&
    operator=(const Implements<Derived,Base> &other);

    bool operator==(const Implements<Derived,Base> &other) const;
    bool operator!=(const Implements<Derived,Base> &other) const;

    Base& super();
    const Base& super() const;

private:
    static const Class* class_typeinfo;
};

/** This class is used to represent nothing - it is the equivalent
    of Python's None object */
class None : public Implements<None,Object>
{
public:
    None();
    None(const None &other);
    
    ~None();
    
    None& operator=(const None &other);
    
    bool operator==(const None &other) const;
    bool operator!=(const None &other) const;
    
    HASH_CODE hashCode() const;
    
    QString toString() const;
    
    bool test(Logger &logger) const;

protected:
    void save(DataStream &ds) const;
    void load(DataStream &ds);
};

#ifndef SIREN_SKIP_INLINE_FUNCTIONS

//////
////// Implementation of 'Extends'
//////

template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
Extends<Derived,Base>::Extends() : Base()
{}

template<class Derived, class Base>
template<class T0>
SIREN_OUTOFLINE_TEMPLATE
Extends<Derived,Base>::Extends(const T0 &t0) : Base(t0)
{}

template<class Derived, class Base>
template<class T0, class T1>
SIREN_OUTOFLINE_TEMPLATE
Extends<Derived,Base>::Extends(const T0 &t0, const T1 &t1) : Base(t0,t1)
{}

template<class Derived, class Base>
template<class T0, class T1, class T2>
SIREN_OUTOFLINE_TEMPLATE
Extends<Derived,Base>::Extends(const T0 &t0, const T1 &t1, const T2 &t2)
                      : Base(t0, t1, t2)
{}

template<class Derived, class Base>
template<class T0, class T1, class T2, class T3>
SIREN_OUTOFLINE_TEMPLATE
Extends<Derived,Base>::Extends(const T0 &t0, const T1 &t1, const T2 &t2, const T3 &t3)
                      : Base(t0, t1, t2, t3)
{}

template<class Derived, class Base>
template<class T0, class T1, class T2, class T3, class T4>
SIREN_OUTOFLINE_TEMPLATE
Extends<Derived,Base>::Extends(const T0 &t0, const T1 &t1, const T2 &t2, const T3 &t3,
                               const T4 &t4)
                      : Base(t0, t1, t2, t3, t4)
{}

template<class Derived, class Base>
template<class T0, class T1, class T2, class T3, class T4, class T5>
SIREN_OUTOFLINE_TEMPLATE
Extends<Derived,Base>::Extends(const T0 &t0, const T1 &t1, const T2 &t2, const T3 &t3,
                               const T4 &t4, const T5 &t5)
                      : Base(t0, t1, t2, t3, t4, t5)
{}

template<class Derived, class Base>
template<class T0, class T1, class T2, class T3, class T4, 
         class T5, class T6>
SIREN_OUTOFLINE_TEMPLATE
Extends<Derived,Base>::Extends(const T0 &t0, const T1 &t1, const T2 &t2, const T3 &t3,
                               const T4 &t4, const T5 &t5, const T6 &t6)
                      : Base(t0, t1, t2, t3, t4, t5, t6)
{}

template<class Derived, class Base>
template<class T0, class T1, class T2, class T3, class T4, 
         class T5, class T6, class T7>
SIREN_OUTOFLINE_TEMPLATE
Extends<Derived,Base>::Extends(const T0 &t0, const T1 &t1, const T2 &t2, const T3 &t3,
                               const T4 &t4, const T5 &t5, const T6 &t6, const T7 &t7)
                      : Base(t0, t1, t2, t3, t4, t5, t6, t7)
{}

template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
Extends<Derived,Base>::~Extends()
{}

template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
Base& Extends<Derived,Base>::super()
{
    return *this;
}

template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
const Base& Extends<Derived,Base>::super() const
{
    return *this;
}

/** Return the class typeinfo object for 'Derived' */
template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
const Class& Extends<Derived,Base>::createTypeInfo()
{
    if ( Extends<Derived,Base>::class_typeinfo == 0 )
    {
        QMutexLocker lkr( &(Object::globalLock()) );
     
        if ( Extends<Derived,Base>::class_typeinfo == 0 )
        {
            const Class &base_class = Base::createTypeInfo();
            QStringList interfaces = Derived::listInterfaces();

            const detail::RegisterMetaTypeBase *r
                    = RegisterMetaType<Derived>::getRegistration();
                    
            if (not r)
                Object::throwUnregisteredMetaTypeError( Derived::typeName() );
        
            Extends<Derived,Base>::class_typeinfo = 
                    Object::registerVirtualClass(r, base_class, interfaces);
        }
    }

    return *( Extends<Derived,Base>::class_typeinfo );
}

//////
////// Implementation of 'Implements'
//////

template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
Implements<Derived,Base>::Implements() : Base()
{}

template<class Derived, class Base>
template<class T0>
SIREN_OUTOFLINE_TEMPLATE
Implements<Derived,Base>::Implements(const T0 &t0) : Base(t0)
{}

template<class Derived, class Base>
template<class T0, class T1>
SIREN_OUTOFLINE_TEMPLATE
Implements<Derived,Base>::Implements(const T0 &t0, const T1 &t1)
                         : Base(t0, t1)
{}

template<class Derived, class Base>
template<class T0, class T1, class T2>
SIREN_OUTOFLINE_TEMPLATE
Implements<Derived,Base>::Implements(const T0 &t0, const T1 &t1, const T2 &t2)
                         : Base(t0, t1, t2)
{}

template<class Derived, class Base>
template<class T0, class T1, class T2, class T3>
SIREN_OUTOFLINE_TEMPLATE
Implements<Derived,Base>::Implements(const T0 &t0, const T1 &t1, const T2 &t2, 
                                     const T3 &t3)
                         : Base(t0, t1, t2, t3)
{}

template<class Derived, class Base>
template<class T0, class T1, class T2, class T3, class T4>
SIREN_OUTOFLINE_TEMPLATE
Implements<Derived,Base>::Implements(const T0 &t0, const T1 &t1, const T2 &t2, 
                                     const T3 &t3, const T4 &t4)
                         : Base(t0, t1, t2, t3, t4)
{}

template<class Derived, class Base>
template<class T0, class T1, class T2, class T3, class T4, class T5>
SIREN_OUTOFLINE_TEMPLATE
Implements<Derived,Base>::Implements(const T0 &t0, const T1 &t1, const T2 &t2, 
                                     const T3 &t3, const T4 &t4, const T5 &t5)
                         : Base(t0, t1, t2, t3, t4, t5)
{}

template<class Derived, class Base>
template<class T0, class T1, class T2, class T3, class T4, 
         class T5, class T6>
SIREN_OUTOFLINE_TEMPLATE
Implements<Derived,Base>::Implements(const T0 &t0, const T1 &t1, const T2 &t2, 
                                     const T3 &t3, const T4 &t4, const T5 &t5, 
                                     const T6 &t6)
                         : Base(t0, t1, t2, t3, t4, t5, t6)
{}

template<class Derived, class Base>
template<class T0, class T1, class T2, class T3, class T4, 
         class T5, class T6, class T7>
SIREN_OUTOFLINE_TEMPLATE
Implements<Derived,Base>::Implements(const T0 &t0, const T1 &t1, const T2 &t2, 
                                     const T3 &t3, const T4 &t4, const T5 &t5, 
                                     const T6 &t6, const T7 &t7)
                         : Base(t0, t1, t2, t3, t4, t5, t6, t7)
{}

template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
Implements<Derived,Base>::~Implements()
{}

template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
QString Implements<Derived,Base>::typeName()
{
    return QMetaType::typeName( qMetaTypeId<Derived>() );
}

template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
QString Implements<Derived,Base>::what() const
{
    return Implements<Derived,Base>::typeName();
}

template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
Implements<Derived,Base>* Implements<Derived,Base>::clone() const
{
    const Derived* derived_this = dynamic_cast<const Derived*>(this);

    BOOST_ASSERT( derived_this );
    
    return new Derived( *derived_this );
}

template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
void Implements<Derived,Base>::copy(const Object &other)
{
    Implements<Derived,Base>::operator=( other.asA<Derived>() );
}

template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
bool Implements<Derived,Base>::equals(const Object &other) const
{
    return Implements<Derived,Base>::operator==( other.asA<Derived>() );
}

/** Return the class typeinfo object for 'Derived' */
template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
const Class& Implements<Derived,Base>::createTypeInfo()
{
    if ( Implements<Derived,Base>::class_typeinfo == 0 )
    {
        QMutexLocker lkr( &(Object::globalLock()) );
        
        if ( Implements<Derived,Base>::class_typeinfo == 0 )
        {
            const Class &base_class = Base::createTypeInfo();
            QStringList interfaces = Derived::listInterfaces();

            const detail::RegisterMetaTypeBase *r
                    = RegisterMetaType<Derived>::getRegistration();
                    
            if (not r)
                Object::throwUnregisteredMetaTypeError( Derived::typeName() );
        
            Implements<Derived,Base>::class_typeinfo = 
                    Object::registerConcreteClass(r, base_class, interfaces);
        }
    }

    return *(Implements<Derived,Base>::class_typeinfo);
}

template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
const Class& Implements<Derived,Base>::getClass() const
{
    return Implements<Derived,Base>::createTypeInfo();
}

template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
Implements<Derived,Base>&
Implements<Derived,Base>::operator=(const Implements<Derived,Base> &other)
{
    const Derived* other_t = dynamic_cast<const Derived*>(&other);

    BOOST_ASSERT( other_t );

    return static_cast<Derived*>(this)->operator=(*other_t);
}

template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
bool Implements<Derived,Base>::operator==(const Implements<Derived,Base> &other) const
{
    const Derived* other_t = dynamic_cast<const Derived*>(&other);

    BOOST_ASSERT( other_t );

    return static_cast<const Derived*>(this)->operator==(*other_t);
}

template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
bool Implements<Derived,Base>::operator!=(const Implements<Derived,Base> &other) const
{
    const Derived* other_t = dynamic_cast<const Derived*>(&other);

    BOOST_ASSERT( other_t );

    return static_cast<const Derived*>(this)->operator!=(*other_t);
}

/** Return the superclass of this type */
template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
Base& Implements<Derived,Base>::super()
{
    return *this;
}

/** Return the superclass of this type */
template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
const Base& Implements<Derived,Base>::super() const
{
    return *this;
}

//////
////// Implementation of 'Object'
//////

/** Return whether or not this object is an object of type 'T' */
template<class T>
SIREN_INLINE_TEMPLATE
bool Object::isA() const
{
    if (dynamic_cast<const T*>(this) != 0)
    {
        return true;
    }
    else
    {
        return this->private_implements( T::typeName() );
    }
}

/** Cast this object to type 'T'. This raises a Siren::invalid_cast
    exception if this cast is not possible 
    
    \throw Siren::invalid_cast
*/
template<class T>
SIREN_INLINE_TEMPLATE
const T& Object::asA() const
{
    const T *obj = dynamic_cast<const T*>(this);
    
    if (obj)
    {
        return *obj;
    }
    else
    {
        this->private_assertCanCast( T::typeName() );

        return *(static_cast<const T*>(this));
    }
}

#endif // SIREN_SKIP_INLINE_FUNCTIONS

} // end of namespace Siren

#ifndef SIREN_SKIP_INLINE_FUNCTIONS

/** Accessor function so that Siren::Objects can be placed
    into a QSet or used as keys in a QHash */
inline uint qHash(const Siren::Object &object)
{
    return object.hashCode();
}

#endif // SIREN_SKIP_INLINE_FUNCTIONS

Q_DECLARE_METATYPE( Siren::None )

SIREN_EXPOSE_CLASS( Siren::Object )
SIREN_EXPOSE_CLASS( Siren::None )

SIREN_END_HEADER

#endif
