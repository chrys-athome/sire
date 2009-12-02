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

#ifndef SIREN_SHAREDPOLYPOINTER_HPP
#define SIREN_SHAREDPOLYPOINTER_HPP

#include "Siren/sirenglobal.h"

SIREN_BEGIN_HEADER

namespace Siren
{

namespace detail
{
template<class T>
class SharedPolyPointer;
}

}

namespace Siren
{

namespace detail
{

/** Default implementation of the helper class used to
    clone and query a polymorphic class.

    @author Christopher Woods
*/
template<class T>
struct SharedPolyPointerHelper
{
    /** Return a copy of the object (a clone). */
    static T* clone(const T &obj)
    {
        return obj.ptr_clone();
    }

    /** Return the type of the object */
    static QString what(const T &obj)
    {
        return obj.what();
    }

    /** Return the typename directly */
    static QString typeName()
    {
        return T::typeName();
    }
    
    /** Return whether or not the two objects are equal */
    static bool equal(const T &obj0, const T &obj1)
    {
        return obj0.equals(obj1);
    }
    
    /** Copy obj1 into obj1 */
    static void copy(T &obj0, const T &obj1)
    {
        obj0.copy(obj1);
    }
    
    /** Return whether or not 'obj' is an object of type 'S' */
    template<class S>
    static bool isA(const T &obj)
    {
        return obj.isA<S>();
    }
    
    /** Return 'obj' cast to an object of type 'S' */
    template<class S>
    static const S& asA(const T &obj)
    {
        return obj.asA<S>();
    }
    
    /** Return 'obj' cast to an object of type 'S' */
    template<class S>
    static S& asA(const T &obj)
    {
        return obj.asA<S>();
    }
};

/** Small base class used to abstract template-independent
    functions away from SharedPolyPointer */
class SIREN_EXPORT SharedPolyPointerBase
{
public:
    SharedPolyPointerBase()
    {}

    SharedPolyPointerBase(const SharedPolyPointerBase&)
    {}

    ~SharedPolyPointerBase();

    static void save(QDataStream &ds, const QString &objname,
                     const void *data);

    static void* read(QDataStream &ds);

protected:
    static void throwInvalidCast(const QString &newtype,
                                 const QString &oldtype);
};

/** This is a version of QSharedDataPointer that has been modified to
    work with polymorphic objects (i.e. will 'clone()' the original
    rather than using the copy constructor).

    This class is copied from QSharedDataPointer in qshareddata.h
    from the Trolltech Qt 4.2.1 distribution (C) Trolltech.com

    Modifications from the original QSharedDataPointer are highlighted,
    and are (C) Christopher Woods. (obviously all QSharedDataPointer are
    changed to SharedPolyPointer)

*/
template <class T>
class SIREN_EXPORT SharedPolyPointer : private SharedPolyPointerBase
{

public:

    typedef T element_type;
    typedef T value_type;
    typedef T* pointer;

    SharedPolyPointer();
    ~SharedPolyPointer();

    explicit SharedPolyPointer(T *data);
    SharedPolyPointer(const T &obj);

    SharedPolyPointer(const SharedPolyPointer<T> &o);

    template<class S>
    SharedPolyPointer(const SharedPolyPointer<S> &o);

    template<class S>
    explicit SharedPolyPointer(S *data);

    template<class S>
    SharedPolyPointer(const S &obj);

    SharedPolyPointer<T>& operator=(T *o);
    SharedPolyPointer<T>& operator=(const T &obj);

    SharedPolyPointer<T>& operator=(const SharedPolyPointer<T> &o);

    template<class S>
    SharedPolyPointer<T>& operator=(const SharedPolyPointer<S> &o);

    SharedPolyPointer<T>& operator=(int);

    template<class S>
    SharedPolyPointer<T>& operator=(const S &obj);

    template<class S>
    SharedPolyPointer<T>& operator=(S *obj);
    
    void detach();
    
    bool unique() const;
    
    T& operator*();
    const T& operator*() const;
    T* operator->();
    const T* operator->() const;
    
    operator T*();
    operator const T*() const;
    
    T *data();
    const T* data() const;
    const T* constData() const;

    bool operator!() const;

    bool operator==(const SharedPolyPointer<T> &other) const;
    bool operator!=(const SharedPolyPointer<T> &other) const;

    bool operator==(const T *other_ptr) const;
    bool operator!=(const T *other_ptr) const;

    QString what() const
    {
        return SharedPolyPointerHelper<T>::what(d);
    }

    template<class S>
    bool isA() const
    {
        if (not d)
            return false;
            
        else
            return SharedPolyPointerHelper<T>::template isA<S>( *d );
    }

    template<class S>
    const S& asA() const
    {
        QString typname = SharedPolyPointerHelper<T>::typeName();
    
        if (not d)
            SharedPolyPointerBase::throwInvalidCast("NULL", typname);

        return SharedPolyPointerHelper<T>::template asA<S>( *d );
    }
    
    template<class S>
    S& asA()
    {
        QString typname = SharedPolyPointerHelper<T>::typeName();
    
        if (not d)
            SharedPolyPointerBase::throwInvalidCast("NULL", typname);

        return SharedPolyPointerHelper<T>::template asA<S>( *d );
    }

private:
    T *d;
};

#ifndef SIREN_SKIP_INLINE_FUNCTIONS

/** Null constructor */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
SharedPolyPointer<T>::SharedPolyPointer() 
                     : SharedPolyPointerBase(),
                       d(0)
{}

/** Construct from a pointer to data - this will take over ownership
    of the data, and may delete it at any time. */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
SharedPolyPointer<T>::SharedPolyPointer(T *adata) 
                     : SharedPolyPointerBase(), 
                       d(adata)
{
    if (d) 
        d->ref.ref();
}

/** Construct from a reference to an object - if this object
    is already pointed to by a SharedPolyPointer then this
    will take another reference to it, otherwise this will
    clone the object (as we will assume that it is not safe
    to delete this object!) */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
SharedPolyPointer<T>::SharedPolyPointer(const T &obj)
                     : SharedPolyPointerBase()
{
    //increment the reference count of this object - this 
    //stops if from being deleted
    T *obj_ptr = const_cast<T*>(&obj);
    obj_ptr->ref.ref();
    
    //if this object is already pointed to by a SharedPolyPointer
    //then the reference count of the QSharedData part will now be
    //greater than one
    if ( int(obj_ptr->ref) > 1 )
    {
        //this is held by another SharedPolyPointer
        d = obj_ptr;
    }
    else
    {
        //the reference count was zero - this implies that
        //this object is not held by another SharedPolyPointer,
        //(it is probably on the stack) so it is not
        //safe to use this object directly - point to a clone
        //of this object.
        d = SharedPolyPointerHelper<T>::clone(obj);
        d->ref.ref();
    
        //reduce the reference count of the original object
        obj_ptr->ref.deref();
    }
}

/** Copy constructor */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
SharedPolyPointer<T>::SharedPolyPointer(const SharedPolyPointer<T> &other)
                     : SharedPolyPointerBase(), d(other.d)
{
    if (d)
        d->ref.ref();
}

/** Copy constructor from a pointer to type 'S'

    \throw SireError::invalid_cast
*/
template<class T>
template<class S>
SIREN_OUTOFLINE_TEMPLATE
SharedPolyPointer<T>::SharedPolyPointer(const SharedPolyPointer<S> &other)
                     : d(0)
{
    S *other_ptr = const_cast<S*>(other.constData());

    if (other_ptr)
    {
        d = dynamic_cast<T*>(other_ptr);
    
        if (d)
            d->ref.ref();
        else
            throwInvalidCast( SharedPolyPointerHelper<S>::what(*other),
                              SharedPolyPointerHelper<T>::typeName() );
    }
}

/** Construct from a raw pointer to an object of type 'S'

    This will take over ownership of the object, and may
    delete it at any time

    \throw SireError::invalid_cast
*/
template<class T>
template<class S>
SIREN_OUTOFLINE_TEMPLATE
SharedPolyPointer<T>::SharedPolyPointer(S *data)
                     : d( dynamic_cast<T*>(data) )
{
    if (data)
    {
        if (d)
            d->ref.ref();
        else
            throwInvalidCast( SharedPolyPointerHelper<S>::what(*data),
                              SharedPolyPointerHelper<T>::typeName() );
    }
}

/** Construct from a reference to the object 'obj' - if this 
    object is already pointed to by a SharedPolyPointer then 
    this will take another reference to it, otherwise this will
    clone the object (as we will assume that it is not safe
    to delete this object!) 
    
    \throw SireError::invalid_cast
*/
template<class T>
template<class S>
SIREN_OUTOFLINE_TEMPLATE
SharedPolyPointer<T>::SharedPolyPointer(const S &obj)
{
    T *obj_ptr = dynamic_cast<T*>( const_cast<S*>(&obj) );
    
    if (!obj_ptr)
        throwInvalidCast( SharedPolyPointerHelper<S>::what(obj),
                          SharedPolyPointerHelper<T>::typeName() );
    
    obj_ptr->ref.ref();
    
    //if this object is already pointed to by a SharedPolyPointer
    //then the reference count of the QSharedData part will now be
    //greater than one
    if ( int(obj_ptr->ref) > 1 )
    {
        //this is held by another SharedPolyPointer
        d = obj_ptr;
    }
    else
    {
        //the reference count was zero - this implies that
        //this object is not held by another SharedPolyPointer,
        //(it is probably on the stack) so it is not
        //safe to use this object directly - point to a clone
        //of this object.
        d = SharedPolyPointerHelper<T>::clone(*obj_ptr);
        d->ref.ref();
    
        //reduce the reference count of the original object
        obj_ptr->ref.deref();
    }
}

/** Destructor */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
SharedPolyPointer<T>::~SharedPolyPointer()
{ 
    if (d && !d->ref.deref()) 
        delete d; 
}

/** Null assignment operator - allows you to write ptr = 0 to 
    reset the pointer to null */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
SharedPolyPointer<T>& SharedPolyPointer<T>::operator=(int)
{
    if (d && !d->ref.deref())
        delete d;
        
    d = 0;
    
    return *this;
}

/** Copy assigment operator - this takes over the ownership
    of 'ptr' and can delete the object at any time. */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
SharedPolyPointer<T>& SharedPolyPointer<T>::operator=(T *ptr)
{
    if (d != ptr)
    {
        if (d)
            qAtomicAssign(d, ptr);
        else
        {
            d = ptr;
            d->ref.ref();
        }
    }
    
    return *this;
}

/** Copy assignment from a reference to the object 'obj' - this
    will take a reference to this object if it is already held
    by another SharedPolyPointer, or it will take a reference to
    a clone of this object */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
SharedPolyPointer<T>& SharedPolyPointer<T>::operator=(const T &obj)
{
    if (d != &obj)
    {
        //increment the reference count of this object - this 
        //stops if from being deleted
        T *obj_ptr = const_cast<T*>(&obj);
        obj_ptr->ref.ref();
    
        //if this object is already pointed to by a SharedPolyPointer
        //then the reference count of the QSharedData part will now be
        //greater than one
        if ( int(obj_ptr->ref) > 1 )
        {
            //this is held by another SharedPolyPointer
            if (d)
            {
                qAtomicAssign(d, obj_ptr);
            
                //remove the extra reference count
                d->ref.deref();
            }
            else
                d = obj_ptr;
        }
        else
        {
            //the reference count was zero - this implies that
            //this object is not held by another SharedPolyPointer,
            //(it is probably on the stack) so it is not
            //safe to use this object directly - point to a clone
            //of this object.
            obj_ptr->ref.deref();
            obj_ptr = SharedPolyPointerHelper<T>::clone(obj);
            
            if (d)
                qAtomicAssign(d, obj_ptr);
            else
            {
                d = obj_ptr;
                d->ref.ref();
            }
        }
    }
    
    return *this;
}

/** Copy assignment from another SharedPolyPointer<T> */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
SharedPolyPointer<T>& SharedPolyPointer<T>::operator=(const SharedPolyPointer<T> &other)
{
    if (other.d != d)
    {
        if (d)
            qAtomicAssign(d, other.d);
        else
        {
            d = other.d;
            d->ref.ref();
        }
    }

    return *this;
}

/** Copy assignment from a SharedPolyPointer<S> 

    \throw Siren::invalid_cast
*/
template<class T>
template<class S>
SIREN_OUTOFLINE_TEMPLATE
SharedPolyPointer<T>& SharedPolyPointer<T>::operator=(const SharedPolyPointer<S> &other)
{
    S *other_ptr = const_cast<S*>(other.constData());

    if (other_ptr and (void*)d != (void*)other_ptr)
    {
        T *obj_ptr = dynamic_cast<T*>(other_ptr);
    
        if (obj_ptr)
        {
            if (d)
                qAtomicAssign(d, obj_ptr);
            else
            {
                d = obj_ptr;
                d->ref.ref();
            }
        }
        else
            throwInvalidCast( SharedPolyPointerHelper<S>::what(*other),
                              SharedPolyPointerHelper<T>::typeName() );
        
    }
        
    return *this;
}

/** Assign from a pointer to an object of type 'S' - this will take
    over ownership of this object and may delete it at any time.
    
    \throw Siren::invalid_cast
*/
template<class T>
template<class S>
SIREN_OUTOFLINE_TEMPLATE
SharedPolyPointer<T>& SharedPolyPointer<T>::operator=(S *ptr)
{
    if (ptr and ptr != d)
    {
        T *obj_ptr = dynamic_cast<T*>(ptr);
        
        if (!obj_ptr)
            throwInvalidCast( SharedPolyPointerHelper<S>::what(*ptr),
                              SharedPolyPointerHelper<T>::typeName() );
        
        if (d)
            qAtomicAssign(d, obj_ptr);
        else
        {
            d = obj_ptr;
            d->ref.ref();
        }
    }
    
    return *this;
}

/** Assign from a reference of type 'S' - this will take a new
    reference to this object if it is already pointed to by
    another SharedPolyPointer, or will clone it.
    
    \throw Siren::invalid_cast
*/
template<class T>
template<class S>
SIREN_OUTOFLINE_TEMPLATE
SharedPolyPointer<T>& SharedPolyPointer<T>::operator=(const S &obj)
{
    if (d != &obj)
    {
        //increment the reference count of this object - this 
        //stops if from being deleted
        T *obj_ptr = dynamic_cast<T*>( const_cast<S*>(&obj) );
        
        if (!obj_ptr)
            throwInvalidCast( SharedPolyPointerHelper<S>::what(obj),
                              SharedPolyPointerHelper<T>::typeName() );
        
        obj_ptr->ref.ref();
    
        //if this object is already pointed to by a SharedPolyPointer
        //then the reference count of the QSharedData part will now be
        //greater than one
        if ( int(obj_ptr->ref) > 1 )
        {
            //this is held by another SharedPolyPointer
            if (d)
            {
                qAtomicAssign(d, obj_ptr);
            
                //remove the extra reference count
                d->ref.deref();
            }
            else
            {
                d = obj_ptr;
                d->ref.ref();
            }
        }
        else
        {
            //the reference count was zero - this implies that
            //this object is not held by another SharedPolyPointer,
            //(it is probably on the stack) so it is not
            //safe to use this object directly - point to a clone
            //of this object.
            obj_ptr->ref.deref();
            obj_ptr = SharedPolyPointerHelper<T>::clone(*obj_ptr);
            
            if (d)
            {
                qAtomicAssign(d, obj_ptr);
            }
            else
            {
                d = obj_ptr;
                d->ref.ref();
            }
        }
    }
    
    return *this;
}

/** Detach the object pointed to by this pointer from shared storage */
template <class T>
SIREN_OUTOFLINE_TEMPLATE
void SharedPolyPointer<T>::detach() 
{
    if (d && d->ref != 1)
    {
        T *x = SharedPolyPointerHelper<T>::clone(*d);
        qAtomicAssign(d, x);
    }
}

/** Return whether or not this pointer is unique (there are no copies) */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
bool SharedPolyPointer<T>::unique() const
{
    return (d and d->ref == 1);
}

/** Dereference this pointer */
template <class T>
SIREN_OUTOFLINE_TEMPLATE
T& SharedPolyPointer<T>::operator*() 
{ 
    detach(); 
    return *d; 
}

/** Dereference this pointer */
template <class T>
SIREN_OUTOFLINE_TEMPLATE
const T& SharedPolyPointer<T>::operator*() const 
{
    return *d; 
}

/** Pointer dereference */
template <class T>
SIREN_OUTOFLINE_TEMPLATE
T* SharedPolyPointer<T>::operator->() 
{
    detach(); 
    return d; 
}

/** Pointer dereference */
template <class T>
SIREN_OUTOFLINE_TEMPLATE
const T* SharedPolyPointer<T>::operator->() const 
{
    return d;
}

/** Cast back to a normal pointer */
template <class T>
SIREN_OUTOFLINE_TEMPLATE
SharedPolyPointer<T>::operator T*() 
{
    detach(); 
    return d; 
}

/** Cast back to a normal pointer */
template <class T>
SIREN_OUTOFLINE_TEMPLATE
SharedPolyPointer<T>::operator const T*() const 
{
    return d;
}

/** Return the pointer held by this shared pointer */
template <class T>
SIREN_OUTOFLINE_TEMPLATE
T* SharedPolyPointer<T>::data()
{
    detach(); 
    return d; 
}

/** Return the pointer held by this shared pointer */
template <class T>
SIREN_OUTOFLINE_TEMPLATE
const T* SharedPolyPointer<T>::data() const
{
    return d;
}

/** Return the pointer held by this shared pointer */
template <class T>
SIREN_OUTOFLINE_TEMPLATE
const T* SharedPolyPointer<T>::constData() const
{
    return d;
}

/** Used to implement if (!ptr){ ... } */
template <class T>
SIREN_OUTOFLINE_TEMPLATE
bool SharedPolyPointer<T>::operator!() const 
{
    return !d;
}

/** Comparison operator */
template <class T>
SIREN_OUTOFLINE_TEMPLATE
bool SharedPolyPointer<T>::operator==(const SharedPolyPointer<T> &other) const
{
    return d == other.d;
}

/** Comparison operator */
template <class T>
SIREN_OUTOFLINE_TEMPLATE
bool SharedPolyPointer<T>::operator!=(const SharedPolyPointer<T> &other) const
{
    return d != other.d;
}

/** Comparison operator */
template <class T>
SIREN_OUTOFLINE_TEMPLATE
bool SharedPolyPointer<T>::operator==(const T *other_ptr) const
{
    return d == other_ptr;
}

/** Comparison operator */
template <class T>
SIREN_OUTOFLINE_TEMPLATE
bool SharedPolyPointer<T>::operator!=(const T *other_ptr) const
{
    return d != other_ptr;
}

#endif //SIREN_SKIP_INLINE_FUNCTIONS

} // end of namespace detail
} // end of namespace Siren

SIREN_END_HEADER

#endif
