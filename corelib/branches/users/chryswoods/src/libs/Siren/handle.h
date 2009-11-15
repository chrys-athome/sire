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

#ifndef SIREN_HANDLE_H
#define SIREN_HANDLE_H

#include <QMutex>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/noncopyable.hpp>

#include "sirenglobal.h"

SIREN_BEGIN_HEADER

namespace Siren
{

namespace detail{ class HanPtrBase; }

class HanRef;
template<class T> class HanPtr;

class WeakHandle;

class Class;
class Logger;

template<class T> class Handles;
template<class Derived, class Base> class ExtendsHandle;
template<class Derived, class Base> class ImplementsHandle;

/** This is the base class of all Handle objects. A Handle provides
    a shared holder to a single global resource, e.g. a file, 
    network connection, compute node or specific Object. A Handle
    allows shared access to that resource, which can be made
    thread-safe by blocking on the Handle's lock
    
    To provide a Handle class you must inherit from
    Handles<MyData> where MyData is the single resource class
    being handled, e.g.
    
    class Node : public ImplementsHandle<Node, Handles<NodeData> > { ... };
    
    class File : public ImplementsHandle<File, Handles<QFile> > { ... };
    
    class ObjectHandle : public ImplementsHandle<ObjectHandle, Handles<ObjRef> > { ... };
    
    Handles are mutable and explicitly shared.
    
    If you want to inherit from a handle you must use
    ExtendsHandle (for virtual classes) or InheritsHandle
    (for concrete classes), e.g.
    
    class Listener : public ExtendsHandle<Listener, Handles<ObjRef> >{ ... };
    
    class ClickListener
           : public ImplementsHandle<ClickListener,Listener>{ ... };
    
    You can add interfaces to Handles using the same mechanism 
    as for Object, e.g.
    
    class MyHandle : public ImplementsHandle<MyHandle, Handles<MyData> >,
                     Interfaces<MyHandle,MyInterface>{ ... };
    
    @author Christopher Woods
*/
class SIREN_EXPORT Handle
{
public:
    Handle();
    Handle(const Handle &other);
    
    virtual ~Handle();

    virtual const Class& getClass() const=0;

    static QString typeName();
    
    /** Reimplement this function to return the 
        fully qualified type name of this object
        (same as handle.getClass().fullName()) */
    virtual QString what() const=0;

    bool isNull() const;

    virtual HanRef copy() const;
    
    virtual QString toString() const;

    virtual bool test() const;
    virtual bool test(Logger &logger) const;

    /** Reimplement this function to copy the 
        contents of 'other' into this object */
    virtual void copy(const Handle &other)=0;

    /** Reimplement this function to return whether
        or not this Handle equals 'other' (points to 
        the same resource) */
    virtual bool equals(const Handle &other) const=0;

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

    void lock();
    void unlock();
    
    bool tryLock();
    bool tryLock(int ms);

    template<class T>
    bool isA() const;

    template<class T>
    T& asA();

    template<class T>
    const T& asA() const;

    void assertNotNull() const;

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

    /** Return a clone of this Handle - this clones the Handle,
        not the object being Handled. */
    virtual Handle* clone() const=0;

    Handle& operator=(const Handle &other);
    
    bool operator==(const Handle &other) const;
    bool operator!=(const Handle &other) const;

    void setValidResource();

    friend class HandleLocker;
    friend class WeakHandle;
    friend class HanRef;
    friend class detail::HanPtrBase;
    
    virtual void neuter();
    virtual void* createWeakHandle() const=0;
    virtual void* copyWeakHandle(void *weak_handle)=0;
    virtual void deleteWeakHandle(void *weak_handle)=0;
    virtual bool restoreFromWeakHandle(void *weak_handle)=0;

private:
    bool private_implements(const QString &class_type) const;
    void private_assertCanCast(const QString &class_type) const;

    static Class* class_typeinfo;

    /** Shared pointer to the mutex used to 
        lock access to this resource */
    boost::shared_ptr<QMutex> resource_lock;
};

/** Inherit from this class to provide your handle. */
template<class T>
class SIREN_EXPORT Handles : public Handle
{
public:
    Handles();
    
    Handles(T *resource);
    
    Handles(const Handles<T> &other);
    
    ~Handles();
    
protected:
    Handles& operator=(const Handles &other);
    
    bool operator==(const Handles &other) const;
    bool operator!=(const Handles &other) const;

    T& resource();
    const T& resource() const;

    const T& constResource() const;
    
    void neuter();
    void* createWeakHandle() const;
    void* copyWeakHandle(void *weak_ptr);
    void deleteWeakHandle(void *weak_ptr);
    bool restoreFromWeakHandle(void *weak_ptr);
    
private:
    /** Shared pointer to the resource */
    boost::shared_ptr<T> resource_ptr;
};

/** This is a small class used to provide a locker
    class for all Handle objects. This class locks
    the handle in the constructor and unlocks
    the handle in the destructor
*/
class SIREN_EXPORT HandleLocker : public boost::noncopyable
{
public:
    HandleLocker();
    HandleLocker(Handle &handle);
    
    ~HandleLocker();
    
    void unlock();
    void relock();

private:
    /** Pointer to the mutex being locked */
    QMutex *resource_mutex;
    
    /** Whether or not the mutex is locked */
    bool is_locked;
};

/** This is a small class that provides a weak Handle to 
    a resource. A weak Handle remains valid while other 
    Handles point to the resource, but is made invalid
    as soon as there are no other handles to the resource.
    
    @author Christopher Woods
*/
class SIREN_EXPORT WeakHandle
{
public:
    WeakHandle();
    WeakHandle(const Handle &handle);
    
    WeakHandle(const WeakHandle &other);
    
    ~WeakHandle();
    
    WeakHandle& operator=(const WeakHandle &other);
    
    bool operator==(const WeakHandle &other) const;
    bool operator!=(const WeakHandle &other) const;
    
    HanRef lock();
    
private:
    /** A neutered copy of the Handle */
    boost::shared_ptr<Handle> neutered_handle;
    
    /** Pointer to the boost::weak_ptr<T> for 
        the handle data */
    void *weak_handle;
    
    /** Weak handle to the mutex used to lock this object */
    boost::weak_ptr<QMutex> weak_lock;
};


/** Derive virtual Handles using this template type */
template<class Derived, class Base>
class ExtendsHandle : public Base
{
public:
    ExtendsHandle();

    template<class T0>
    ExtendsHandle(const T0 &t0);

    template<class T0, class T1>
    ExtendsHandle(const T0 &t0, const T1 &t1);

    template<class T0, class T1, class T2>
    ExtendsHandle(const T0 &t0, const T1 &t1, const T2 &t2);

    template<class T0, class T1, class T2, class T3>
    ExtendsHandle(const T0 &t0, const T1 &t1, const T2 &t2, const T3 &t3);

    template<class T0, class T1, class T2, class T3, class T4>
    ExtendsHandle(const T0 &t0, const T1 &t1, const T2 &t2, const T3 &t3,
                  const T4 &t4);

    template<class T0, class T1, class T2, class T3, class T4, class T5>
    ExtendsHandle(const T0 &t0, const T1 &t1, const T2 &t2, const T3 &t3,
                  const T4 &t4, const T5 &t5);

    template<class T0, class T1, class T2, class T3, class T4, 
             class T5, class T6>
    ExtendsHandle(const T0 &t0, const T1 &t1, const T2 &t2, const T3 &t3,
                  const T4 &t4, const T5 &t5, const T6 &t6);

    template<class T0, class T1, class T2, class T3, class T4, 
             class T5, class T6, class T7>
    ExtendsHandle(const T0 &t0, const T1 &t1, const T2 &t2, const T3 &t3,
                  const T4 &t4, const T5 &t5, const T6 &t6, const T7 &t7);

    ~ExtendsHandle();

protected:
    static const Class& createTypeInfo();

    Base& super();
    const Base& super() const;

private:
    static const Class* class_typeinfo;
};

/** Derive concrete Handles using this template type */
template<class Derived, class Base>
class ImplementsHandle : public Base
{
public:
    ImplementsHandle();

    template<class T0>
    ImplementsHandle(const T0 &t0);

    template<class T0, class T1>
    ImplementsHandle(const T0 &t0, const T1 &t1);

    template<class T0, class T1, class T2>
    ImplementsHandle(const T0 &t0, const T1 &t1, const T2 &t2);

    template<class T0, class T1, class T2, class T3>
    ImplementsHandle(const T0 &t0, const T1 &t1, const T2 &t2, const T3 &t3);

    template<class T0, class T1, class T2, class T3, class T4>
    ImplementsHandle(const T0 &t0, const T1 &t1, const T2 &t2, const T3 &t3,
                     const T4 &t4);

    template<class T0, class T1, class T2, class T3, class T4, class T5>
    ImplementsHandle(const T0 &t0, const T1 &t1, const T2 &t2, const T3 &t3,
                     const T4 &t4, const T5 &t5);

    template<class T0, class T1, class T2, class T3, class T4, 
             class T5, class T6>
    ImplementsHandle(const T0 &t0, const T1 &t1, const T2 &t2, const T3 &t3,
                     const T4 &t4, const T5 &t5, const T6 &t6);

    template<class T0, class T1, class T2, class T3, class T4, 
             class T5, class T6, class T7>
    ImplementsHandle(const T0 &t0, const T1 &t1, const T2 &t2, const T3 &t3,
                     const T4 &t4, const T5 &t5, const T6 &t6, const T7 &t7);

    virtual ~ImplementsHandle();

    ImplementsHandle<Derived,Base>& operator=(const Handle &other);

    bool operator==(const Handle &other) const;

    bool operator!=(const Handle &other) const;

    static QString typeName();

    QString what() const;

    HanRef copy() const;

    const Class& getClass() const;

    void copy(const Handle &other);
    bool equals(const Handle &other) const;

protected:
    static const Class& createTypeInfo();

    ImplementsHandle<Derived,Base>* clone() const;

    ImplementsHandle<Derived,Base>&
    operator=(const ImplementsHandle<Derived,Base> &other);

    bool operator==(const ImplementsHandle<Derived,Base> &other) const;
    bool operator!=(const ImplementsHandle<Derived,Base> &other) const;

    Base& super();
    const Base& super() const;

private:
    static const Class* class_typeinfo;
};

#ifndef SIREN_SKIP_INLINE_FUNCTIONS

///////
/////// Implementation of Handles
///////

/** Null constructor */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
Handles<T>::Handles() : Handle()
{}

/** Construct a handle to the resource 'resource' - this will
    take over ownership of this resource */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
Handles<T>::Handles(T *resource) : Handle(), resource_ptr(resource)
{}

/** Copy constructor */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
Handles<T>::Handles(const Handles<T> &other) 
           : Handle(other), resource_ptr(other.resource_ptr)
{}

/** Destructor - if this is the last handle to the data
    then the data will be deleted (and released) now */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
Handles<T>::~Handles()
{}

/** Internal function called by WeakHandle to neutralise this handle */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
void Handles<T>::neuter()
{
    this->resource_ptr.reset();
    Handle::neuter();
}

/** Internal function called by WeakHandle to create a weak_ptr 
    to the resource managed by this handle */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
void* Handles<T>::createWeakHandle() const
{
    return new boost::weak_ptr<T>( this->resource_ptr );
}

/** Internal function called by WeakHandle to copy the 
    passed boost::weak_ptr */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
void* Handles<T>::copyWeakHandle(void *weak_ptr)
{
    if (weak_ptr)
        return new boost::weak_ptr<T>( 
                    static_cast<const boost::weak_ptr<T>&>(weak_ptr) );
    else
        return 0;
}

/** Internal function called by WeakHandle to delete
    the passed boost::weak_ptr */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
void Handles<T>::deleteWeakHandle(void *weak_ptr)
{
    if (weak_ptr)
    {
        boost::weak_ptr<T> *weak_ptr_t = static_cast<boost::weak_ptr<T>*>(weak_ptr);
        delete weak_ptr_t;
    }
}

/** Internal function callde by WeakHandle to restore this
    handle from the passed weak_ptr - this returns whether or
    not the weak_ptr is still valid (and this has been restored) */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
bool Handles<T>::restoreFromWeakHandle(void *weak_ptr)
{
    if (not weak_ptr)
        return false;
        
    boost::weak_ptr<T> *weak_ptr_t = static_cast<boost::weak_ptr<T>*>(weak_ptr);
    
    this->resource_ptr = weak_ptr_t->lock();
    
    return this->resource_ptr.get() != 0;
}

/** Return a reference to the shared resource - you should
    only access this if you hold this Handle's lock
    
    \throw Siren::nullptr_error
*/
template<class T>
SIREN_OUTOFLINE_TEMPLATE
T& Handles<T>::resource()
{
    Handle::assertNotNull();
    return *(resource_ptr.get());
}

/** Return a reference to the shared resource - you should
    only access this if you hold this Handle's lock
    
    \throw Siren::nullptr_error
*/
template<class T>
SIREN_OUTOFLINE_TEMPLATE
const T& Handles<T>::resource() const
{
    Handle::assertNotNull();
    return *(resource_ptr.get());
}

/** Return a reference to the shared resource - you should
    only access this if you hold this Handle's lock
    
    \throw Siren::nullptr_error
*/
template<class T>
SIREN_OUTOFLINE_TEMPLATE
const T& Handles<T>::constResource() const
{
    Handle::assertNotNull();
    return *(resource_ptr.get());
}

//////
////// Implementation of 'ExtendsHandle'
//////

template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
ExtendsHandle<Derived,Base>::ExtendsHandle() : Base()
{}

template<class Derived, class Base>
template<class T0>
SIREN_OUTOFLINE_TEMPLATE
ExtendsHandle<Derived,Base>::ExtendsHandle(const T0 &t0) : Base(t0)
{}

template<class Derived, class Base>
template<class T0, class T1>
SIREN_OUTOFLINE_TEMPLATE
ExtendsHandle<Derived,Base>::ExtendsHandle(const T0 &t0, const T1 &t1) : Base(t0,t1)
{}

template<class Derived, class Base>
template<class T0, class T1, class T2>
SIREN_OUTOFLINE_TEMPLATE
ExtendsHandle<Derived,Base>::ExtendsHandle(const T0 &t0, const T1 &t1, const T2 &t2)
                            : Base(t0, t1, t2)
{}

template<class Derived, class Base>
template<class T0, class T1, class T2, class T3>
SIREN_OUTOFLINE_TEMPLATE
ExtendsHandle<Derived,Base>::ExtendsHandle(const T0 &t0, const T1 &t1, const T2 &t2, 
                                           const T3 &t3)
                            : Base(t0, t1, t2, t3)
{}

template<class Derived, class Base>
template<class T0, class T1, class T2, class T3, class T4>
SIREN_OUTOFLINE_TEMPLATE
ExtendsHandle<Derived,Base>::ExtendsHandle(const T0 &t0, const T1 &t1, const T2 &t2, 
                                           const T3 &t3, const T4 &t4)
                            : Base(t0, t1, t2, t3, t4)
{}

template<class Derived, class Base>
template<class T0, class T1, class T2, class T3, class T4, class T5>
SIREN_OUTOFLINE_TEMPLATE
ExtendsHandle<Derived,Base>::ExtendsHandle(const T0 &t0, const T1 &t1, const T2 &t2, 
                                           const T3 &t3, const T4 &t4, const T5 &t5)
                            : Base(t0, t1, t2, t3, t4, t5)
{}

template<class Derived, class Base>
template<class T0, class T1, class T2, class T3, class T4, 
         class T5, class T6>
SIREN_OUTOFLINE_TEMPLATE
ExtendsHandle<Derived,Base>::ExtendsHandle(const T0 &t0, const T1 &t1, const T2 &t2, 
                                           const T3 &t3, const T4 &t4, const T5 &t5, 
                                           const T6 &t6)
                            : Base(t0, t1, t2, t3, t4, t5, t6)
{}

template<class Derived, class Base>
template<class T0, class T1, class T2, class T3, class T4, 
         class T5, class T6, class T7>
SIREN_OUTOFLINE_TEMPLATE
ExtendsHandle<Derived,Base>::ExtendsHandle(const T0 &t0, const T1 &t1, const T2 &t2, 
                                           const T3 &t3, const T4 &t4, const T5 &t5, 
                                           const T6 &t6, const T7 &t7)
                            : Base(t0, t1, t2, t3, t4, t5, t6, t7)
{}

template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
ExtendsHandle<Derived,Base>::~ExtendsHandle()
{}

template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
Base& ExtendsHandle<Derived,Base>::super()
{
    return *this;
}

template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
const Base& ExtendsHandle<Derived,Base>::super() const
{
    return *this;
}

/** Return the class typeinfo object for 'Derived' */
template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
const Class& ExtendsHandle<Derived,Base>::createTypeInfo()
{
    if ( ExtendsHandle<Derived,Base>::class_typeinfo == 0 )
    {
        QMutexLocker lkr( &(Handle::globalLock()) );
     
        if ( ExtendsHandle<Derived,Base>::class_typeinfo == 0 )
        {
            const Class &base_class = Base::createTypeInfo();
            QStringList interfaces = Derived::listInterfaces();

            const detail::RegisterMetaTypeBase *r
                    = RegisterMetaType<Derived>::getRegistration();
                    
            if (not r)
                Handle::throwUnregisteredMetaTypeError( Derived::typeName() );
        
            ExtendsHandle<Derived,Base>::class_typeinfo = 
                    Handle::registerVirtualClass(r, base_class, interfaces);
        }
    }

    return *( ExtendsHandle<Derived,Base>::class_typeinfo );
}

//////
////// Implementation of 'ImplementsHandle'
//////

template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
ImplementsHandle<Derived,Base>::ImplementsHandle() : Base()
{}

template<class Derived, class Base>
template<class T0>
SIREN_OUTOFLINE_TEMPLATE
ImplementsHandle<Derived,Base>::ImplementsHandle(const T0 &t0) : Base(t0)
{}

template<class Derived, class Base>
template<class T0, class T1>
SIREN_OUTOFLINE_TEMPLATE
ImplementsHandle<Derived,Base>::ImplementsHandle(const T0 &t0, const T1 &t1)
                               : Base(t0, t1)
{}

template<class Derived, class Base>
template<class T0, class T1, class T2>
SIREN_OUTOFLINE_TEMPLATE
ImplementsHandle<Derived,Base>::ImplementsHandle(const T0 &t0, const T1 &t1, const T2 &t2)
                               : Base(t0, t1, t2)
{}

template<class Derived, class Base>
template<class T0, class T1, class T2, class T3>
SIREN_OUTOFLINE_TEMPLATE
ImplementsHandle<Derived,Base>::ImplementsHandle(const T0 &t0, const T1 &t1, const T2 &t2, 
                                                 const T3 &t3)
                               : Base(t0, t1, t2, t3)
{}

template<class Derived, class Base>
template<class T0, class T1, class T2, class T3, class T4>
SIREN_OUTOFLINE_TEMPLATE
ImplementsHandle<Derived,Base>::ImplementsHandle(const T0 &t0, const T1 &t1, const T2 &t2, 
                                                 const T3 &t3, const T4 &t4)
                               : Base(t0, t1, t2, t3, t4)
{}

template<class Derived, class Base>
template<class T0, class T1, class T2, class T3, class T4, class T5>
SIREN_OUTOFLINE_TEMPLATE
ImplementsHandle<Derived,Base>::ImplementsHandle(const T0 &t0, const T1 &t1, const T2 &t2, 
                                                 const T3 &t3, const T4 &t4, const T5 &t5)
                               : Base(t0, t1, t2, t3, t4, t5)
{}

template<class Derived, class Base>
template<class T0, class T1, class T2, class T3, class T4, 
         class T5, class T6>
SIREN_OUTOFLINE_TEMPLATE
ImplementsHandle<Derived,Base>::ImplementsHandle(const T0 &t0, const T1 &t1, const T2 &t2, 
                                                 const T3 &t3, const T4 &t4, const T5 &t5, 
                                                 const T6 &t6)
                               : Base(t0, t1, t2, t3, t4, t5, t6)
{}

template<class Derived, class Base>
template<class T0, class T1, class T2, class T3, class T4, 
         class T5, class T6, class T7>
SIREN_OUTOFLINE_TEMPLATE
ImplementsHandle<Derived,Base>::ImplementsHandle(const T0 &t0, const T1 &t1, const T2 &t2, 
                                                 const T3 &t3, const T4 &t4, const T5 &t5, 
                                                 const T6 &t6, const T7 &t7)
                               : Base(t0, t1, t2, t3, t4, t5, t6, t7)
{}

template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
ImplementsHandle<Derived,Base>::~ImplementsHandle()
{}

template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
QString ImplementsHandle<Derived,Base>::typeName()
{
    return QMetaType::typeName( qMetaTypeId<Derived>() );
}

template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
QString ImplementsHandle<Derived,Base>::what() const
{
    return ImplementsHandle<Derived,Base>::typeName();
}

template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
ImplementsHandle<Derived,Base>* ImplementsHandle<Derived,Base>::clone() const
{
    const Derived* derived_this = dynamic_cast<const Derived*>(this);

    BOOST_ASSERT( derived_this );
    
    return new Derived( *derived_this );
}

template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
void ImplementsHandle<Derived,Base>::copy(const Handle &other)
{
    ImplementsHandle<Derived,Base>::operator=( other.asA<Derived>() );
}

template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
bool ImplementsHandle<Derived,Base>::equals(const Handle &other) const
{
    return ImplementsHandle<Derived,Base>::operator==( other.asA<Derived>() );
}

/** Return the class typeinfo object for 'Derived' */
template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
const Class& ImplementsHandle<Derived,Base>::createTypeInfo()
{
    if ( ImplementsHandle<Derived,Base>::class_typeinfo == 0 )
    {
        QMutexLocker lkr( &(Handle::globalLock()) );
        
        if ( ImplementsHandle<Derived,Base>::class_typeinfo == 0 )
        {
            const Class &base_class = Base::createTypeInfo();
            QStringList interfaces = Derived::listInterfaces();

            const detail::RegisterMetaTypeBase *r
                    = RegisterMetaType<Derived>::getRegistration();
                    
            if (not r)
                Handle::throwUnregisteredMetaTypeError( Derived::typeName() );
        
            ImplementsHandle<Derived,Base>::class_typeinfo = 
                    Handle::registerConcreteClass(r, base_class, interfaces);
        }
    }

    return *(ImplementsHandle<Derived,Base>::class_typeinfo);
}

template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
const Class& ImplementsHandle<Derived,Base>::getClass() const
{
    return ImplementsHandle<Derived,Base>::createTypeInfo();
}

template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
ImplementsHandle<Derived,Base>&
ImplementsHandle<Derived,Base>::operator=(const ImplementsHandle<Derived,Base> &other)
{
    const Derived* other_t = dynamic_cast<const Derived*>(&other);

    BOOST_ASSERT( other_t );

    return static_cast<Derived*>(this)->operator=(*other_t);
}

template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
bool ImplementsHandle<Derived,Base>::operator==(
                        const ImplementsHandle<Derived,Base> &other) const
{
    const Derived* other_t = dynamic_cast<const Derived*>(&other);

    BOOST_ASSERT( other_t );

    return static_cast<const Derived*>(this)->operator==(*other_t);
}

template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
bool ImplementsHandle<Derived,Base>::operator!=(
                        const ImplementsHandle<Derived,Base> &other) const
{
    const Derived* other_t = dynamic_cast<const Derived*>(&other);

    BOOST_ASSERT( other_t );

    return static_cast<const Derived*>(this)->operator!=(*other_t);
}

/** Return the superclass of this type */
template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
Base& ImplementsHandle<Derived,Base>::super()
{
    return *this;
}

/** Return the superclass of this type */
template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
const Base& ImplementsHandle<Derived,Base>::super() const
{
    return *this;
}

///////
/////// Implementation of Handle
///////

/** Return whether or not this HandleRef refers to a Handle of type 'T' */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
bool Handle::isA() const
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

/** Return this Handle cast as type 'T'
    
    \throw Siren::invalid_cast
*/
template<class T>
SIREN_OUTOFLINE_TEMPLATE
T& Handle::asA()
{
    T *obj = dynamic_cast<T*>(this);
    
    if (obj)
    {
        return *obj;
    }
    else
    {
        this->private_assertCanCast( T::typeName() );

        return *(static_cast<T*>(this));
    }
}

/** Return this Handle cast as type 'T'
    
    \throw Siren::invalid_cast
*/
template<class T>
SIREN_OUTOFLINE_TEMPLATE
const T& Handle::asA() const
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

}

#ifndef SIREN_SKIP_INLINE_FUNCTIONS

/** Accessor function so that Siren::Handle objects can be placed
    into a QSet or used as keys in a QHash */
inline uint qHash(const Siren::Handle &handle)
{
    return handle.hashCode();
}

#endif // SIREN_SKIP_INLINE_FUNCTIONS

SIREN_EXPOSE_CLASS( Siren::Handle )
SIREN_EXPOSE_CLASS( Siren::HandleLocker )
SIREN_EXPOSE_CLASS( Siren::WeakHandle )

SIREN_END_HEADER

#endif
