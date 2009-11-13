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

SIREN_BEGIN_HEADER

namespace Siren
{
class Object;
class Class;

class None;

class DataStream;
class XMLStream;

class Logger;

namespace detail
{
    class ObjPtrBase;
    class GlobalObjPtrBase;
}

template<class T> class ObjPtr;
template<class T> class GlobalObjPtr;

template<class Derived,class Base> class Implements;

class ObjRef;
}

class QDomNode;

using Siren::XMLStream;
using Siren::DataStream;

QDataStream& operator<<(QDataStream&, const Siren::Object&);
QDataStream& operator>>(QDataStream&, Siren::Object&);

DataStream& operator<<(DataStream&, const Siren::Object&);
DataStream& operator>>(DataStream&, Siren::Object&);

XMLStream& operator<<(XMLStream&, const Siren::Object&);
XMLStream& operator>>(XMLStream&, Siren::Object&);

QDataStream& operator<<(QDataStream&, const Siren::detail::ObjPtrBase&);
QDataStream& operator>>(QDataStream&, Siren::detail::ObjPtrBase&);

DataStream& operator<<(DataStream&, const Siren::detail::ObjPtrBase&);
DataStream& operator>>(DataStream&, Siren::detail::ObjPtrBase&);

XMLStream& operator<<(XMLStream&, const Siren::detail::ObjPtrBase&);
XMLStream& operator>>(XMLStream&, Siren::detail::ObjPtrBase&);

template<class T>
QDataStream& operator<<(QDataStream&, const Siren::ObjPtr<T>&);
template<class T>
QDataStream& operator>>(QDataStream&, Siren::ObjPtr<T>&);

template<class T>
DataStream& operator<<(DataStream&, const Siren::ObjPtr<T>&);
template<class T>
DataStream& operator>>(DataStream&, Siren::ObjPtr<T>&);

template<class T>
XMLStream& operator<<(XMLStream&, const Siren::ObjPtr<T>&);
template<class T>
XMLStream& operator>>(XMLStream&, Siren::ObjPtr<T>&);

QDataStream& operator<<(QDataStream&, const Siren::detail::GlobalObjPtrBase&);
QDataStream& operator>>(QDataStream&, Siren::detail::GlobalObjPtrBase&);

DataStream& operator<<(DataStream&, const Siren::detail::GlobalObjPtrBase&);
DataStream& operator>>(DataStream&, Siren::detail::GlobalObjPtrBase&);

XMLStream& operator<<(XMLStream&, const Siren::detail::GlobalObjPtrBase&);
XMLStream& operator>>(XMLStream&, Siren::detail::GlobalObjPtrBase&);

template<class T>
QDataStream& operator<<(QDataStream&, const Siren::GlobalObjPtr<T>&);
template<class T>
QDataStream& operator>>(QDataStream&, Siren::GlobalObjPtr<T>&);

template<class T>
DataStream& operator<<(DataStream&, const Siren::GlobalObjPtr<T>&);
template<class T>
DataStream& operator>>(DataStream&, Siren::GlobalObjPtr<T>&);

template<class T>
XMLStream& operator<<(XMLStream&, const Siren::GlobalObjPtr<T>&);
template<class T>
XMLStream& operator>>(XMLStream&, Siren::GlobalObjPtr<T>&);

QDataStream& operator<<(QDataStream&, const Siren::ObjRef&);
QDataStream& operator>>(QDataStream&, Siren::ObjRef&);

DataStream& operator<<(DataStream&, const Siren::ObjRef&);
DataStream& operator>>(DataStream&, Siren::ObjRef&);

XMLStream& operator<<(XMLStream&, const Siren::ObjRef&);
XMLStream& operator>>(XMLStream&, Siren::ObjRef&);

namespace Siren
{

namespace detail
{
template<class T> class SharedPolyPointer;
template<class T> class GlobalSharedPointer;
}

/** This is the base class of all Siren virtual objects.
    
    All Siren objects are inherited from either this class
    (if they have virtual functions or wish to use metatyping
     or serialization), or, if they are basic, they are inherited
    from Siren::Primitive
    
    A Siren object provides the single root of all non-primitive
    objects, together with a .metaClass() function that returns
    a Siren::Class object that contains information about the type.
    
    In addition, all Siren::Objects support in-place unit testing,
    with each object containing a .test() method that is called
    via the Siren::Class object, e.g.
    
    obj.Class().test();
    
    or
    
    obj.Class().test( logger );
    
    This returns true if all tests passed. This allows unit tests
    to be written with the object, in the same implementation
    file as the object (and to have full access to the private
    data of the object).
    
    Furthermore, each Siren::Object contains a .invariant() method
    that returns whether or not the object is currently maintaining
    its invariant. This is useful for design-by-contract or 
    debugging.
    
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
    
    ObjectPtr new_obj = obj.clone();  // clone the current object
    
    ObjectPtr new_obj = obj.getClass().create();  // create a new object
    
    obj.isA<T>();   // return whether 'obj' is of type 'T'
    
    obj.asA<T>();   // return 'obj' cast as an object of type 'T'
    
    QDataStream ds;
    
    ds << obj;   // save 'obj' to a binary stream
    
    ObjectPtr new_obj;
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
    
    Siren::Object is a virtual class. While you can override
    the virtual methods manually, it is much easier if
    you inherit from Implements<D,B>, where D is your
    new concrete class and B is the class from which it inherits.
    
    For example, if NewObj is a concrete class derived from Object,
    the it should inherit from Implements<NewObj,Object>. 
    The Implements class does all of the hard work of 
    filling in the virtual functions of Object.
    
    Each Siren::Object is immutable (unchangeable). If you want
    to create a mutable object, you *must* inherit from the
    Mutable interface. This will provide you with functions
    that will allow you to save and restore the state of
    the mutable object (e.g. to protect an invariant if
    an exception is thrown).
    
    Each Siren::Object must also be non-shared. This means that
    as long as access to the object is protected by a mutex,
    then the object is thread-safe. This also means that the 
    object cannot be changed 'behind-the-back', e.g. a shared
    object can be changed using any of the references/handles to that object.
    If you want a shared object, then you will need to
    inherit from the Shared interface, that provides
    methods that allow a shared object to be detached.
    
    @author Christopher Woods
*/
class SIREN_EXPORT Object : private QSharedData
{

friend class detail::SharedPolyPointer<Object>;
friend class detail::GlobalSharedPointer<Object>;

friend DataStream& ::operator<<(DataStream&, const Object&);
friend DataStream& ::operator>>(DataStream&, Object&);

friend XMLStream& ::operator<<(XMLStream&, const Object&);
friend XMLStream& ::operator>>(XMLStream&, Object&);

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
    
    static QStringList listInterfaces();

    static QMutex& globalLock();

    virtual void load(const QDomNode &dom);
    virtual void save(QDomNode &dom) const;

    virtual void load(DataStream &ds);
    virtual void save(DataStream &ds) const;

    static void throwUnregisteredMetaTypeError(const QString &type_name);
    
    static void throwInvalidCast(const QString &from_type, const QString &to_type);

    static Class* registerConcreteClass( const detail::RegisterMetaTypeBase *r,
                                         const Class &base_class,
                                         const QStringList &interfaces);
                                        
    static Class* registerVirtualClass( const detail::RegisterMetaTypeBase *r,
                                        const Class &base_class,
                                        const QStringList &interfaces );

private:
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
    static ObjPtr<Class> class_typeinfo;
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

    Implements<Derived,Base>* clone() const;

    const Class& getClass() const;

    void copy(const Object &other);
    bool equals(const Object &other) const;

protected:
    static const Class& createTypeInfo();

    Implements<Derived,Base>&
    operator=(const Implements<Derived,Base> &other);

    bool operator==(const Implements<Derived,Base> &other) const;
    bool operator!=(const Implements<Derived,Base> &other) const;

    Base& super();
    const Base& super() const;

private:
    static ObjPtr<Class> class_typeinfo;
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

} // end of namespace Siren

#include "class.h"

#include "detail/sharedpolypointer.hpp"
#include "detail/globalsharedpointer.hpp"

namespace Siren
{

namespace detail
{

/** This is base class of the polymorphic pointer holder for the entire
    Object class hierarchy. This can hold implicitly 
    shared pointers to any Object class.

    @author Christopher Woods
*/
class SIREN_EXPORT ObjPtrBase
{

friend DataStream& ::operator<<(DataStream&, const ObjPtrBase&);
friend DataStream& ::operator>>(DataStream&, ObjPtrBase&);

friend XMLStream& ::operator<<(XMLStream&, const ObjPtrBase&);
friend XMLStream& ::operator>>(XMLStream&, ObjPtrBase&);

public:
    ~ObjPtrBase();

    bool operator==(const Object &object) const;
    bool operator!=(const Object &object) const;

    bool operator==(const ObjPtrBase &other) const;
    bool operator!=(const ObjPtrBase &other) const;

    void detach();

    bool unique() const;

    operator const Object&() const;

protected:
    ObjPtrBase(const Object &object);
    ObjPtrBase(Object *object);
    
    ObjPtrBase(const ObjPtrBase &other);

    ObjPtrBase& operator=(const ObjPtrBase &other);

    const Object& read() const;
    
    Object& write();
    Object& edit();

    static void throwCastingError(const char *got_type, const char *want_type);

private:
    /** Shared pointer to the actual Object */
    detail::SharedPolyPointer<Object> ptr;
};

/** This is base class of the global polymorphic pointer holder for the entire
    Object class hierarchy. This can hold implicitly 
    shared pointers to any Object class.

    @author Christopher Woods
*/
class SIREN_EXPORT GlobalObjPtrBase
{

friend DataStream& ::operator<<(DataStream&, const GlobalObjPtrBase&);
friend DataStream& ::operator>>(DataStream&, GlobalObjPtrBase&);

friend XMLStream& ::operator<<(XMLStream&, const GlobalObjPtrBase&);
friend XMLStream& ::operator>>(XMLStream&, GlobalObjPtrBase&);

public:
    ~GlobalObjPtrBase();

    bool operator==(const Object &object) const;
    bool operator!=(const Object &object) const;

    bool operator==(const GlobalObjPtrBase &other) const;
    bool operator!=(const GlobalObjPtrBase &other) const;

    bool unique() const;

    operator const Object&() const;

protected:
    GlobalObjPtrBase(const Object &object);
    GlobalObjPtrBase(Object *object);
    
    GlobalObjPtrBase(const GlobalObjPtrBase &other);

    GlobalObjPtrBase& operator=(const GlobalObjPtrBase &other);

    const Object& read() const;

    static void throwCastingError(const char *got_type, const char *want_type);

private:
    /** Global shared pointer to the actual object */
    detail::GlobalSharedPointer<Object> ptr;
};

} // end of namespace detail

/** This template class provides a typed pointer to an 
    object derived from Siren::Object. This pointer,
    by default, provides read-only access to the object.
    To get write-access to the object, you need to use
    the ObjPtr.write() function. This is an implicitly
    shared, copy-on-write pointer, so using ObjPtr.write()
    will implicitly create a new copy of the object if it 
    is pointed to by other ObjPtr pointers
    
    @author Christopher Woods
*/
template<class T>
class SIREN_EXPORT ObjPtr : public detail::ObjPtrBase
{
public:
    ObjPtr();

    ObjPtr(const T &obj);
    ObjPtr(T *obj);
    
    ObjPtr(const Object &object);

    ObjPtr(const ObjPtrBase &other);

    ObjPtr(const ObjPtr<T> &other);
    
    ~ObjPtr();
    
    ObjPtr<T>& operator=(const T &obj);
    ObjPtr<T>& operator=(T *obj);
    
    ObjPtr<T>& operator=(const Object &object);

    ObjPtr<T>& operator=(const ObjPtr<T> &other);
    ObjPtr<T>& operator=(const ObjPtrBase &object);

    const T* operator->() const;
    const T& operator*() const;
    
    const T& read() const;
    T& edit();

    const T* data() const;
    const T* constData() const;

    T* data();
    
    operator const T&() const;

    bool isNull() const;

    static ObjPtr<T> null();

protected:
    void assertSane() const;
};

/** This template class provides a global typed pointer to an 
    object derived from Siren::Object. This pointer,
    by default, provides read-only access to the object.
    To get write-access to the object, you need to use
    the ObjPtr.write() function. This is an implicitly
    shared, copy-on-write pointer, so using ObjPtr.write()
    will implicitly create a new copy of the object if it 
    is pointed to by other ObjPtr pointers
    
    @author Christopher Woods
*/
template<class T>
class SIREN_EXPORT GlobalObjPtr : public detail::GlobalObjPtrBase
{
public:
    GlobalObjPtr();

    GlobalObjPtr(const T &obj);
    GlobalObjPtr(T *obj);
    
    GlobalObjPtr(const Object &object);

    GlobalObjPtr(const GlobalObjPtrBase &other);

    GlobalObjPtr(const GlobalObjPtr<T> &other);
    
    ~GlobalObjPtr();
    
    GlobalObjPtr<T>& operator=(const T &obj);
    GlobalObjPtr<T>& operator=(T *obj);
    
    GlobalObjPtr<T>& operator=(const Object &object);

    GlobalObjPtr<T>& operator=(const GlobalObjPtr<T> &other);
    GlobalObjPtr<T>& operator=(const GlobalObjPtrBase &object);

    const T* operator->() const;
    const T& operator*() const;
    
    const T& read() const;

    const T* data() const;
    const T* constData() const;
    
    operator const T&() const;

    bool isNull() const;

    static GlobalObjPtr<T> null();

protected:
    void assertSane() const;
};

/** This is a light-weight reference to an object. This class is 
    used when a new object is returned from a function. This class
    will automatically cast itself back into a real const Object& 
    reference
*/
class SIREN_EXPORT ObjRef
{
public:
    ObjRef(const Object &obj);
    
    ObjRef(const ObjRef &other);
    
    ~ObjRef();
    
    ObjRef& operator=(const ObjRef &other);
    
    bool operator==(const ObjRef &other) const;
    bool operator!=(const ObjRef &other) const;

    Class getClass() const;

    QString what() const;
    
    QString toString() const;

    void copy(const Object &other);

    bool equals(const Object &other) const;
    
    quint32 hashCode() const;

    template<class T>
    bool isA() const;

    template<class T>
    const T& asA() const;
    
    operator const Object&() const;
    
private:
    /** The shared pointer to the actual object */
    detail::SharedPolyPointer<Object> d;
};

#ifndef SIREN_SKIP_INLINE_FUNCTIONS

//////
////// Implementation of 'Extends'
//////

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
                         : Base(t0, t1, t2)
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
        
            Extends<Derived,Base>::class_typeinfo = 
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
    return static_cast<Base&>( *this );
}

/** Return the superclass of this type */
template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
const Base& Implements<Derived,Base>::super() const
{
    return static_cast<const Base&>( *this );
}

#endif // SIREN_SKIP_INLINE_FUNCTIONS

} // end of namespace Siren

namespace Siren
{

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
        return this->getClass().implements( T::typeName() );
    }
}

/** Cast this object to type 'T'. This raises a Siren::invalid_cast
    exception if this cast is not possible */
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
        this->getClass().assertCanCast( T::typeName() );

        return *(static_cast<const T*>(this));
    }
}

} // end of namespace Siren

SIREN_EXPOSE_CLASS( Siren::Object )

SIREN_END_HEADER

#endif
