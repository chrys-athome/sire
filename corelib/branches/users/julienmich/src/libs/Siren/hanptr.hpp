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

#ifndef SIREN_HANPTR_HPP
#define SIREN_HANPTR_HPP

#include "handle.h"
#include "class.h"

SIREN_BEGIN_HEADER

namespace Siren
{

namespace detail
{
    class HanPtrBase;
}

template<class T> class HanPtr;

}

namespace Siren
{

namespace detail
{

/** This is base class of the polymorphic pointer holder for the entire
    Handle class hierarchy. This can hold explicitly shared pointers to 
    any Handle class.

    @author Christopher Woods
*/
class SIREN_EXPORT HanPtrBase
{
public:
    ~HanPtrBase();

    bool operator==(const Handle &handle) const;
    bool operator!=(const Handle &handle) const;

    bool operator==(const HanPtrBase &other) const;
    bool operator!=(const HanPtrBase &other) const;

    void detach();

    bool unique() const;

    bool isNull() const;
    
    void assertNotNull() const;
    
    operator const Handle&() const;

protected:
    HanPtrBase(const Handle &handle);
    HanPtrBase(Handle *handle);
    
    HanPtrBase(const HanPtrBase &other);

    HanPtrBase& operator=(const HanPtrBase &other);

    Handle* get();
    const Handle* get() const;

    static void throwCastingError(QString got_type, QString want_type);

private:
    /** Shared pointer to the actual Handle */
    boost::shared_ptr<Handle> ptr;
};

} // end of namespace detail

/** This template class provides a typed pointer to a 
    Handle derived from Siren::Handle. 
    
    @author Christopher Woods
*/
template<class T>
class SIREN_EXPORT HanPtr : public detail::HanPtrBase
{
public:
    HanPtr();

    HanPtr(const T &handle);
    HanPtr(T *handle);

    HanPtr(const HanPtrBase &other);

    HanPtr(const HanPtr<T> &other);
    
    ~HanPtr();
    
    HanPtr<T>& operator=(const T &handle);
    HanPtr<T>& operator=(T *handle);

    HanPtr<T>& operator=(const HanPtr<T> &other);
    HanPtr<T>& operator=(const HanPtrBase &handle);

    T* operator->();
    T& operator*();

    const T* operator->() const;
    const T& operator*() const;
    
    T* get();
    const T* get() const;
    
    operator const T&() const;

protected:
    void assertSane() const;
};

#ifndef SIREN_SKIP_INLINE_FUNCTIONS

//////
////// Implementation of 'HanPtr'
//////

/** Assert that this is sane - this is to make sure that the 
    Object really is derived from 'T' */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
void HanPtr<T>::assertSane() const
{
    const Handle *h = HanPtrBase::get();

    if (h)
    {
        if (not h->isA<T>())
        {
            HanPtrBase::throwCastingError( h->what(), T::typeName() );
        }
    }
}

/** Null constructor */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
HanPtr<T>::HanPtr() : HanPtrBase(0)
{}

/** Construct from the handle 'handle' */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
HanPtr<T>::HanPtr(const T &handle) : HanPtrBase(handle)
{}

/** Construct from a pointer to the passed handle 'handle' - this
    takes over ownership of the pointer and can delete it at any time */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
HanPtr<T>::HanPtr(T *handle) : HanPtrBase(handle)
{}

/** Construct from a pointer of another type */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
HanPtr<T>::HanPtr(const HanPtrBase &other) : HanPtrBase(other)
{
    this->assertSane();
}

/** Copy constructor */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
HanPtr<T>::HanPtr(const HanPtr<T> &other) : HanPtrBase(other)
{}

/** Destructor */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
HanPtr<T>::~HanPtr()
{}

/** Copy assignment operator */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
HanPtr<T>& HanPtr<T>::operator=(const HanPtr<T> &other)
{
    HanPtrBase::operator=(other);
    return *this;
}

/** Create a pointer that points to the handle 'handle' (or a copy, if this
    handle is on the stack) */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
HanPtr<T>& HanPtr<T>::operator=(const T &handle)
{
    return this->operator=( HanPtr<T>(handle) );
}

/** Create a pointer that points to the handle 'handle' - this takes
    over ownership of 'handle' and can delete it at any time */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
HanPtr<T>& HanPtr<T>::operator=(T *handle)
{
    return this->operator=( HanPtr<T>(handle) );
}

/** Construct from the passed pointer to a handle */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
HanPtr<T>& HanPtr<T>::operator=(const HanPtrBase &handle)
{
    return this->operator=( HanPtr<T>(handle) );
}

/** Return the raw pointer to the handle */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
const T* HanPtr<T>::get() const
{
    return static_cast<const T*>( HanPtrBase::get() );
}

/** Return a modifiable pointer to the data */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
T* HanPtr<T>::get()
{
    return static_cast<T*>( HanPtrBase::get() );
}

/** Return the raw pointer to the handle */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
const T* HanPtr<T>::operator->() const
{
    return &(this->get());
}

/** Return a reference to the handle */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
const T& HanPtr<T>::operator*() const
{
    HanPtrBase::assertNotNull();
    return *(this->get());
}

/** Allow automatic casting to a T() */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
HanPtr<T>::operator const T&() const
{
    HanPtrBase::assertNotNull();
    return *(this->read());
}

#endif // SIREN_SKIP_INLINE_FUNCTIONS

typedef HanPtr<Handle> HandlePtr;

}

SIREN_EXPOSE_HANDLE_PTR( Siren::ObjectPtr, Siren::Object )

SIREN_END_HEADER

#endif