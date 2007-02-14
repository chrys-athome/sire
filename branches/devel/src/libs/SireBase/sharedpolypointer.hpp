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

#ifndef SIREBASE_SHAREDPOLYPOINTER_HPP
#define SIREBASE_SHAREDPOLYPOINTER_HPP

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

SIRE_BEGIN_HEADER

namespace SireBase
{
template<class T>
class SharedPolyPointer;
}

template<class T>
QDataStream& operator<<(QDataStream&, const SireBase::SharedPolyPointer<T>&);

template<class T>
QDataStream& operator>>(QDataStream&, SireBase::SharedPolyPointer<T>&);

namespace SireBase
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
        return obj.clone();
    }

    /** Return the type of the object */
    static const char* what(const T &obj)
    {
        return obj.what();
    }

    /** Return the typename directly */
    static const char* typeName()
    {
        return T::typeName();
    }
};

/** Small base class used to abstract template-independent
    functions away from SharedPolyPointer */
class SIREBASE_EXPORT SharedPolyPointerBase
{
public:
    SharedPolyPointerBase()
    {}

    SharedPolyPointerBase(const SharedPolyPointerBase&)
    {}

    ~SharedPolyPointerBase();

    static void save(QDataStream &ds, const char *objname,
                     const void *data);

    static void* read(QDataStream &ds, void *data, const char *objname);

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
class SharedPolyPointer : private SharedPolyPointerBase
{

friend QDataStream& ::operator<<<>(QDataStream&, const SharedPolyPointer<T>&);
friend QDataStream& ::operator>><>(QDataStream&, SharedPolyPointer<T>&);

public:

    typedef T element_type;
    typedef T value_type;
    typedef T * pointer;

    inline void detach() { if (d && d->ref != 1) detach_helper(); }
    inline T &operator*() { detach(); return *d; }
    inline const T &operator*() const { return *d; }
    inline T *operator->() { detach(); return d; }
    inline const T *operator->() const { return d; }
    inline operator T *() { detach(); return d; }
    inline operator const T *() const { return d; }
    inline T *data() { detach(); return d; }
    inline const T *data() const { return d; }
    inline const T *constData() const { return d; }

    inline bool operator!() const { return !d; }

    inline bool operator==(const SharedPolyPointer<T> &other) const
    {
        return d == other.d;
    }

    inline bool operator!=(const SharedPolyPointer<T> &other) const
    {
        return d != other.d;
    }

    inline SharedPolyPointer() : SharedPolyPointerBase() { d = 0; }
    inline ~SharedPolyPointer() { if (d && !d->ref.deref()) delete d; }

    explicit SharedPolyPointer(T *data);
    explicit SharedPolyPointer(const T &obj);

    inline SharedPolyPointer(const SharedPolyPointer<T> &o)
                : SharedPolyPointerBase(), d(o.d)
    {
        if (d) d->ref.ref();
    }


    template<class S>
    inline SharedPolyPointer(const SharedPolyPointer<S> &o)
             : SharedPolyPointerBase(),
               d( const_cast<T*>( dynamic_cast<const T*>(o.constData()) ) )
    {
        if (o.constData())
        {
            if (d)
                d->ref.ref();
            else
                throwInvalidCast( SharedPolyPointerHelper<S>::what(*o),
                                  SharedPolyPointerHelper<T>::typeName() );
        }
    }

    inline SharedPolyPointer<T> & operator=(const SharedPolyPointer<T> &o)
    {
        if (o.d != d)
        {
            T *x = o.d;
            if (x) x->ref.ref();
            x = qAtomicSetPtr(&d, x);
            if (x && !x->ref.deref())
                delete x;
        }

        return *this;
    }

    template<class S>
    inline SharedPolyPointer<T> & operator=(const SharedPolyPointer<S> &o)
    {
        T *x = const_cast<T*>( dynamic_cast<const T*>(o.constData()) );

        if (x != d)
        {

            if (o.constData())
            {
                if (x)
                    x->ref.ref();
                else
                    throwInvalidCast( SharedPolyPointerHelper<S>::what(*o),
                                      SharedPolyPointerHelper<T>::typeName() );

                x = qAtomicSetPtr(&d, x);

                if (x && !x->ref.deref())
                    delete x;
            }
        }

        return *this;
    }

    inline SharedPolyPointer &operator=(T *o)
    {
        if (o != d)
        {
            T *x = o;
            if (x) x->ref.ref();
            x = qAtomicSetPtr(&d, x);
            if (x && !x->ref.deref())
                delete x;
        }

        return *this;
    }

    inline SharedPolyPointer& operator=(const T &obj)
    {
        //look at the reference count - if the count is not zero
        //then we will assume that this object is held by another
        //SharedPolyPointer
        if (obj.ref)
        {
            T &obj_held_by_ptr = const_cast<T&>(obj);

            return this->operator=(&obj_held_by_ptr);
        }
        else
        {
            //this object is probably on the stack - it is
            //not safe to point to the original so lets point
            //to a clone
            return this->operator=( SharedPolyPointerHelper<T>::clone(obj) );
        }
    }

    inline const char* what() const
    {
        return SharedPolyPointerHelper<T>::what(d);
    }

    template<class S>
    bool isA() const
    {
        return dynamic_cast<const S*>(this->constData()) != 0;
    }

    template<class S>
    const S& asA() const
    {
        return dynamic_cast<const S&>(*(this->constData()));
    }

private:
    void detach_helper();

    T *d;
};

template <class T>
Q_INLINE_TEMPLATE
SharedPolyPointer<T>::SharedPolyPointer(T *adata) : d(adata)
{
    if (d) d->ref.ref();
}

template<class T>
Q_INLINE_TEMPLATE
SharedPolyPointer<T>::SharedPolyPointer(const T &obj)
{
    //if this object is already pointed to by a SharedPolyPointer
    //then the reference count of the QSharedData part will be
    //greater than zero
    if (obj.ref)
    {
        //this is held by another SharedPolyPointer - increase its
        //reference count and take a pointer to it
        //(as it is held by another SharedPolyPointer then it is
        //safe to const_cast the object, as the implicit sharing of
        //SharedPolyPointer will prevent incorrect assignment)

        T &obj_held_by_ptr = const_cast<T&>(obj);

        obj_held_by_ptr.ref.ref();
        d = &obj_held_by_ptr;
    }
    else
    {
        //the reference count is zero - this implies that
        //this object is not held by another SharedPolyPointer,
        //(it is probably on the stack) so it is not
        //safe to use this object directly - point to a clone
        //of this object.
        d = SharedPolyPointerHelper<T>::clone(obj);

        if (d)
            d->ref.ref();
    }
}

template <class T>
Q_OUTOFLINE_TEMPLATE
void SharedPolyPointer<T>::detach_helper()
{
    /** Modification by C.Woods - use helper function
        to clone the object */
    T *x = SharedPolyPointerHelper<T>::clone(*d);

    x->ref.ref();
    x = qAtomicSetPtr(&d, x);
    if (!x->ref.deref())
        delete x;
}

const MagicID sharedpolypointer_magic = getMagic("SireBase::SharedPolyPointer");
}

/** Serialise to a binary data stream

    \throw SireError::unknown_type
    \throw SireError::program_bug
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
QDataStream& operator<<(QDataStream &ds,
                        const SireBase::SharedPolyPointer<T> &ptr)
{
    SireStream::writeHeader(ds, SireBase::sharedpolypointer_magic, 1);

    if (ptr.d == 0)
        ds << QString::null;
    else
    {
        //get the object type name
        QLatin1String objname( SireBase::SharedPolyPointerHelper<T>::what( *(ptr.constData()) ) );

        SireBase::SharedPolyPointerBase::save(ds,
                      SireBase::SharedPolyPointerHelper<T>::what(*(ptr.constData())), ptr.d);
    }

    return ds;
}

/** Deserialise from a binary data stream */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
QDataStream& operator>>(QDataStream &ds,
                        SireBase::SharedPolyPointer<T> &ptr)
{
    SireStream::VersionID v = SireStream::readHeader(ds, SireBase::sharedpolypointer_magic,
                                                     "SireBase::SharedPolyPointer");

    if (v == 1)
    {
        if (ptr.d)
        {
            ptr = static_cast<T*>(
                      SireBase::SharedPolyPointerBase::read(ds, ptr.data(),
                                     SireBase::SharedPolyPointerHelper<T>::what(*(ptr.constData()))
                                                 )
                                 );
        }
        else
        {
            ptr = static_cast<T*>(
                      SireBase::SharedPolyPointerBase::read(ds, 0, 0) );
        }
    }
    else
        throw SireStream::version_error(v, "1", "SireBase::SharedPolyPointer", CODELOC);

    return ds;
}

template<class T>
inline const T* get_pointer(SireBase::SharedPolyPointer<T> const & p)
{
    return p.constData();
}

template<class T>
inline T* get_pointer(SireBase::SharedPolyPointer<T> &p)
{
    return p.data();
}

SIRE_END_HEADER

#endif
