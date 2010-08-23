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

#ifndef SIREN_OBJPTR_HPP
#define SIREN_OBJPTR_HPP

#include "object.h"
#include "class.h"
#include "datastream.h"
#include "stream.h"
#include "objref.h"

#include "detail/sharedpolypointer.hpp"
#include "detail/globalsharedpointer.hpp"

SIREN_BEGIN_HEADER

namespace Siren
{

namespace detail
{
    class ObjPtrBase;
    class GlobalObjPtrBase;
}

template<class T> class ObjPtr;
template<class T> class GlobalObjPtr;

}

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
public:
    ~ObjPtrBase();

    bool operator==(const Object &object) const;
    bool operator!=(const Object &object) const;

    bool operator==(const ObjPtrBase &other) const;
    bool operator!=(const ObjPtrBase &other) const;

    void detach();

    bool equals(const ObjPtrBase &other) const;
    bool equals(const Object &object) const;

    bool unique() const;

    bool isNull() const;
    
    void assertNotNull() const;
    
    operator const Object&() const;

protected:
    ObjPtrBase(const Object &object);
    ObjPtrBase(Object *object);
    
    ObjPtrBase(const ObjPtrBase &other);

    ObjPtrBase& operator=(const ObjPtrBase &other);

    const Object& read() const;

    const Object* data() const;
    const Object* constData() const;
    
    Object* data();
    
    Object& write();
    Object& edit();

    static void throwCastingError(QString got_type, QString want_type);

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
public:
    ~GlobalObjPtrBase();

    bool operator==(const Object &object) const;
    bool operator!=(const Object &object) const;

    bool operator==(const GlobalObjPtrBase &other) const;
    bool operator!=(const GlobalObjPtrBase &other) const;

    bool unique() const;

    bool isNull() const;

    void assertNotNull() const;

    operator const Object&() const;

protected:
    GlobalObjPtrBase(const Object &object);
    GlobalObjPtrBase(Object *object);
    
    GlobalObjPtrBase(const GlobalObjPtrBase &other);

    GlobalObjPtrBase& operator=(const GlobalObjPtrBase &other);

    const Object& read() const;

    const Object* data() const;
    const Object* constData() const;

    static void throwCastingError(QString got_type, QString want_type);

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
    typedef T value_type;

    ObjPtr();

    ObjPtr(const T &obj);
    ObjPtr(T *obj);

    ObjPtr(const ObjPtrBase &other);

    ObjPtr(const ObjPtr<T> &other);
    
    ~ObjPtr();
    
    ObjPtr<T>& operator=(const T &obj);
    ObjPtr<T>& operator=(T *obj);

    ObjPtr<T>& operator=(const ObjPtr<T> &other);
    ObjPtr<T>& operator=(const ObjPtrBase &object);
    ObjPtr<T>& operator=(const ObjRef &objref);

    const T* operator->() const;
    const T& operator*() const;

    static QString typeName();
    
    const T& read() const;
    T& edit();

    const T* data() const;
    const T* constData() const;

    T* data();
    
    operator const T&() const;

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
    typedef T value_type;

    GlobalObjPtr();

    GlobalObjPtr(const T &obj);
    GlobalObjPtr(T *obj);

    GlobalObjPtr(const GlobalObjPtrBase &other);

    GlobalObjPtr(const GlobalObjPtr<T> &other);
    
    ~GlobalObjPtr();
    
    GlobalObjPtr<T>& operator=(const T &obj);
    GlobalObjPtr<T>& operator=(T *obj);

    GlobalObjPtr<T>& operator=(const GlobalObjPtr<T> &other);
    GlobalObjPtr<T>& operator=(const GlobalObjPtrBase &object);

    const T* operator->() const;
    const T& operator*() const;
    
    const T& read() const;

    const T* data() const;
    const T* constData() const;
    
    static QString typeName();
    
    operator const T&() const;

protected:
    void assertSane() const;
};

#ifndef SIREN_SKIP_INLINE_FUNCTIONS

//////
////// Implementation of 'ObjPtr'
//////

/** Assert that this is sane - this is to make sure that the 
    Object really is derived from 'T' */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
void ObjPtr<T>::assertSane() const
{
    if (not this->isNull())
    {
        const Object &p = ObjPtrBase::read();

        if (not p.isA<T>())
        {
            ObjPtrBase::throwCastingError( p.what(), T::typeName() );
        }
    }
}

/** Null constructor */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
ObjPtr<T>::ObjPtr() : ObjPtrBase(0)
{}

/** Construct from the object 'obj' */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
ObjPtr<T>::ObjPtr(const T &obj) : ObjPtrBase(obj)
{}

/** Construct from a pointer to the passed object 'obj' - this
    takes over ownership of the pointer and can delete it at any time */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
ObjPtr<T>::ObjPtr(T *obj) : ObjPtrBase(obj)
{}

/** Construct from a pointer of another type */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
ObjPtr<T>::ObjPtr(const ObjPtrBase &other) : ObjPtrBase(other)
{
    this->assertSane();
}

/** Copy constructor */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
ObjPtr<T>::ObjPtr(const ObjPtr<T> &other) : ObjPtrBase(other)
{}

/** Destructor */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
ObjPtr<T>::~ObjPtr()
{}

/** Copy assignment operator */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
ObjPtr<T>& ObjPtr<T>::operator=(const ObjPtr<T> &other)
{
    ObjPtrBase::operator=(other);
    return *this;
}

/** Create a pointer that points to the object 'obj' (or a copy, if this
    object is on the stack) */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
ObjPtr<T>& ObjPtr<T>::operator=(const T &obj)
{
    return this->operator=( ObjPtr<T>(obj) );
}

/** Create a pointer that points to the object 'obj' - this takes
    over ownership of 'obj' and can delete it at any time */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
ObjPtr<T>& ObjPtr<T>::operator=(T *obj)
{
    return this->operator=( ObjPtr<T>(obj) );
}

/** Construct from the passed pointer to a object */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
ObjPtr<T>& ObjPtr<T>::operator=(const ObjPtrBase &object)
{
    return this->operator=( ObjPtr<T>(object) );
}

template<class T>
SIREN_OUTOFLINE_TEMPLATE
ObjPtr<T>& ObjPtr<T>::operator=(const ObjRef &object)
{
    return this->operator=(object.asA<T>());
}

template<class T>
SIREN_OUTOFLINE_TEMPLATE
QString ObjPtr<T>::typeName()
{
    return QMetaType::typeName( qMetaTypeId< ObjPtr<T> >() );
}

/** Return a reference to the object */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
const T& ObjPtr<T>::read() const
{
    //This is only safe as everything in this class ensures that 
    //the base pointer really is a pointer to the derived type 'T' */
    return static_cast<const T&>( ObjPtrBase::read() );
}

/** Return an editable reference to the object */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
T& ObjPtr<T>::edit()
{
    //This is only safe as everything in this class ensures that 
    //the base pointer really is a pointer to the derived type 'T' */
    return static_cast<T&>( ObjPtrBase::edit() );
}

/** Return the raw pointer to the object */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
const T* ObjPtr<T>::operator->() const
{
    return &(this->read());
}

/** Return a reference to the object */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
const T& ObjPtr<T>::operator*() const
{
    return this->read();
}

/** Return the raw pointer to the object */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
const T* ObjPtr<T>::data() const
{
    return static_cast<const T*>( ObjPtrBase::data() );
}

/** Return the raw pointer to the object */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
const T* ObjPtr<T>::constData() const
{
    return static_cast<const T*>( ObjPtrBase::data() );
}

/** Return a modifiable pointer to the data */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
T* ObjPtr<T>::data()
{
    return static_cast<T*>( ObjPtrBase::data() );
}

/** Allow automatic casting to a T() */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
ObjPtr<T>::operator const T&() const
{
    return this->read();
}

template<class T>
Siren::Stream& Siren::Stream::operator&(Siren::ObjPtr<T> &object)
{
    this->assertVersion("Siren::ObjPtr", 1);

    if (this->isLoading())
    {
        Siren::ObjRef objref;
        objref.stream(*this);
        object = objref;
    }
    else
    {
        Siren::ObjRef objref = object;
        objref.stream(*this);
    }

    return *this;
}

//////
////// Implementation of 'GlobalObjPtr'
//////

/** Assert that this is sane - this is to make sure that the 
    Object really is derived from 'T' */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
void GlobalObjPtr<T>::assertSane() const
{
    if (not this->isNull())
    {
        const Object &p = GlobalObjPtrBase::read();

        if (not p.isA<T>())
        {
            GlobalObjPtrBase::throwCastingError( p.what(), T::typeName() );
        }
    }
}

/** Null constructor */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
GlobalObjPtr<T>::GlobalObjPtr() : GlobalObjPtrBase(0)
{}

/** Construct from the object 'obj' */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
GlobalObjPtr<T>::GlobalObjPtr(const T &obj) : GlobalObjPtrBase(obj)
{}

/** Construct from a pointer to the passed object 'obj' - this
    takes over ownership of the pointer and can delete it at any time */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
GlobalObjPtr<T>::GlobalObjPtr(T *obj) : GlobalObjPtrBase(obj)
{}

/** Construct from a pointer of another type */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
GlobalObjPtr<T>::GlobalObjPtr(const GlobalObjPtrBase &other) : GlobalObjPtrBase(other)
{
    this->assertSane();
}

/** Copy constructor */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
GlobalObjPtr<T>::GlobalObjPtr(const GlobalObjPtr<T> &other) : GlobalObjPtrBase(other)
{}

/** Destructor */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
GlobalObjPtr<T>::~GlobalObjPtr()
{}

/** Copy assignment operator */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
GlobalObjPtr<T>& GlobalObjPtr<T>::operator=(const GlobalObjPtr<T> &other)
{
    GlobalObjPtrBase::operator=(other);
    return *this;
}

/** Create a pointer that points to the object 'obj' (or a copy, if this
    object is on the stack) */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
GlobalObjPtr<T>& GlobalObjPtr<T>::operator=(const T &obj)
{
    return this->operator=( GlobalObjPtr<T>(obj) );
}

/** Create a pointer that points to the object 'obj' - this takes
    over ownership of 'obj' and can delete it at any time */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
GlobalObjPtr<T>& GlobalObjPtr<T>::operator=(T *obj)
{
    return this->operator=( GlobalObjPtr<T>(obj) );
}

/** Construct from the passed pointer to a object */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
GlobalObjPtr<T>& GlobalObjPtr<T>::operator=(const GlobalObjPtrBase &object)
{
    return this->operator=( GlobalObjPtr<T>(object) );
}

/** Return a reference to the object */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
const T& GlobalObjPtr<T>::read() const
{
    //This is only safe as everything in this class ensures that 
    //the base pointer really is a pointer to the derived type 'T' */
    return static_cast<const T&>( GlobalObjPtrBase::read() );
}

/** Return the raw pointer to the object */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
const T* GlobalObjPtr<T>::operator->() const
{
    return &(this->read());
}

/** Return a reference to the object */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
const T& GlobalObjPtr<T>::operator*() const
{
    return this->read();
}

/** Return the raw pointer to the object */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
const T* GlobalObjPtr<T>::data() const
{
    return static_cast<const T*>( GlobalObjPtr::data() );
}

/** Return the raw pointer to the object */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
const T* GlobalObjPtr<T>::constData() const
{
    return static_cast<const T*>( GlobalObjPtr::constData() );
}

/** Allow automatic casting to a T() */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
GlobalObjPtr<T>::operator const T&() const
{
    return this->read();
}

template<class T>
SIREN_OUTOFLINE_TEMPLATE
QString GlobalObjPtr<T>::typeName()
{
    return QMetaType::typeName( qMetaTypeId< GlobalObjPtr<T> >() );
}

template<class T>
Stream& Stream::operator&(GlobalObjPtr<T> &object)
{
    this->assertVersion("Siren::GlobalObjPtr", 1);
    
    if (this->isLoading())
    {
        Siren::ObjRef objref;
        *this >> objref;
        
        object = objref;
    }
    else
    {
        Siren::ObjRef objref = object;
        *this << objref;
    }
    
    return *this;
}

#endif // SIREN_SKIP_INLINE_FUNCTIONS

typedef ObjPtr<Object> ObjectPtr;
typedef GlobalObjPtr<Object> GlobalObjectPtr;

}

Q_DECLARE_METATYPE( Siren::ObjectPtr )
Q_DECLARE_METATYPE( Siren::GlobalObjectPtr )

SIREN_EXPOSE_OBJECT_PTR( Siren::ObjectPtr, Siren::Object )

SIREN_END_HEADER

#endif
