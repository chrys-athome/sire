#ifndef SIREBASE_DYNAMICSHAREDPTR_HPP
#define SIREBASE_DYNAMICSHAREDPTR_HPP

#include <QSharedDataPointer>
#include <QSharedData>

#include "sireglobal.h"

#include "SireStream/datastream.h"
#include "SireError/errors.h"

SIRE_BEGIN_HEADER

namespace SireBase
{

namespace detail
{
template<class T>
class DynamicSharedData;
}

template<class T>
class DynamicSharedPtr;
}

template<class T>
QDataStream& operator<<(QDataStream&, const SireBase::detail::DynamicSharedData<T>&);
template<class T>
QDataStream& operator>>(QDataStream&, SireBase::detail::DynamicSharedData<T>&);

template<class T>
QDataStream& operator<<(QDataStream&, const SireBase::DynamicSharedPtr<T>&);
template<class T>
QDataStream& operator>>(QDataStream&, SireBase::DynamicSharedPtr<T>&);

namespace SireBase
{

/** Default implementation of 'clone()' - this assumes that
    the type implements the 'clone()' member function. */
template<class T>
T* clone(const T &obj)
{
    return obj.clone();
}

/** Default implementation of 'what()' - his assumes that
    the type implements the 'what()' member function. */
template<class T>
const char* what(const T &obj)
{
    return obj.what();
}

namespace detail
{

/** This is an internal class used by DynamicSharedPtr

    @author Christopher Woods
*/
template<class T>
class DynamicSharedData : public QSharedData
{

friend QDataStream& ::operator<<<>(QDataStream&, const DynamicSharedData<T>&);
friend QDataStream& ::operator>><>(QDataStream&, DynamicSharedData<T>&);

public:
    DynamicSharedData( T *shared_data = 0 );

    DynamicSharedData( const DynamicSharedData<T> &other );

    ~DynamicSharedData();

    DynamicSharedData<T>& operator=(const DynamicSharedData<T> &other);

    const T* constData() const;

    const T* data() const;
    T* data();

private:
    /** Pointer to the actual object */
    T *objptr;
};

/** Construct a holder for the pointer to 'shared_data' */
template<class T>
DynamicSharedData<T>::DynamicSharedData( T *shared_data )
                     : QSharedData(), objptr(shared_data)
{}

/** Copy constructor - this clones any object that is held by 'other' */
template<class T>
DynamicSharedData<T>::DynamicSharedData( const DynamicSharedData<T> &other )
                     : QSharedData(), objptr(0)
{
    if (other.objptr)
        objptr = clone<T>( *(other.objptr) );
}

/** Destructor - this deletes the object pointed to by 'objptr' */
template<class T>
DynamicSharedData<T>::~DynamicSharedData()
{
    delete objptr;
}

/** Assignment operator - this clones any object that is held by 'other' */
template<class T>
DynamicSharedData<T>& DynamicSharedData<T>::operator=(const DynamicSharedData<T> &other)
{
    //prevent self-assignment
    if (this != &other)
    {
        if (objptr)
        {
            //delete our copy
            delete objptr;
            objptr = 0;
        }

        //clone the object
        if (other.objptr)
            objptr = clone<T>( *(other.objptr) );
    }

    return *this;
}

/** Return a pointer to the object */
template<class T>
const T* DynamicSharedData<T>::constData() const
{
    return objptr;
}

/** Return a pointer to the object */
template<class T>
const T* DynamicSharedData<T>::data() const
{
    return objptr;
}

/** Return a pointer to the object */
template<class T>
T* DynamicSharedData<T>::data()
{
    return objptr;
}

}

/** This class provides a pointer to polymorphic implicitly shared data.

    Note that the template function 'T* clone(const T&obj)' must have
    been defined for type 'T', or instead it must supply a
    'clone()' function.

    For data streaming to work there must also be a 'const char* what(const T&obj)'
    function declared, else instead supply a 'what()' function, and the derived
    object must have been registered with QMetaType.

    The interface to this pointer is copied from QSharedDataPointer,
    from Qt 4.1.3, (C) Trolltech

    @author Christopher Woods
*/
template<class T>
class DynamicSharedPtr
{

friend QDataStream& ::operator<<<>(QDataStream&, const DynamicSharedPtr<T>&);
friend QDataStream& ::operator>><>(QDataStream&, DynamicSharedPtr<T>&);

public:
    DynamicSharedPtr();

    DynamicSharedPtr( const T &obj );
    DynamicSharedPtr( T *shared_data );

    DynamicSharedPtr( const DynamicSharedPtr<T> &other );

    ~DynamicSharedPtr();

    const T* constData() const;
    T* data();
    const T* data() const;

    void detach();

    operator T*();
    operator const T*() const;

    bool operator!() const;

    bool operator==( const DynamicSharedPtr<T> & other ) const;
    bool operator!=(const DynamicSharedPtr<T> &other) const;

    T& operator*();
    const T& operator*() const;

    T* operator->();
    const T* operator->() const;

    DynamicSharedPtr<T>& operator=( const DynamicSharedPtr<T> &other );
    DynamicSharedPtr<T>& operator=( T *shared_data );
    DynamicSharedPtr<T>& operator=( const T &obj );

    template<class S>
    bool isA() const;

    template<class S>
    const S& asA() const;

    template<class S>
    S& asA();

private:
    /** Shared null pointer */
    static QSharedDataPointer< detail::DynamicSharedData<T> > shared_null;

    /** Pointer to the implicitly shared data */
    QSharedDataPointer< detail::DynamicSharedData<T> > d;
};

/** Shared null pointer */
template<class T>
QSharedDataPointer< detail::DynamicSharedData<T> >
DynamicSharedPtr<T>::shared_null( new detail::DynamicSharedData<T>(0) );

/** Construct a null pointer */
template<class T>
DynamicSharedPtr<T>::DynamicSharedPtr()
                    : d( DynamicSharedPtr<T>::shared_null )
{}

/** Construct a pointer that points to a clone of 'obj' */
template<class T>
DynamicSharedPtr<T>::DynamicSharedPtr( const T &obj )
                    : d( new detail::DynamicSharedData<T>( clone<T>(obj) ) )
{}

/** Construct a pointer to the object pointed to by 'shared_data'. This
    pointer will take over ownership of this pointer and may delete it
    at any time. */
template<class T>
DynamicSharedPtr<T>::DynamicSharedPtr( T *shared_data )
                    : d( new detail::DynamicSharedData<T>(shared_data) )
{}

/** Copy constructor - this is fast as an implicit copy is taken */
template<class T>
DynamicSharedPtr<T>::DynamicSharedPtr( const DynamicSharedPtr<T> &other )
                    : d( other.d )
{}

/** Destructor - this will only delete the object if this is the last
    pointer referring to it. */
template<class T>
DynamicSharedPtr<T>::~DynamicSharedPtr()
{}

/** Return a const pointer to the shared data - this does not call 'detach()' */
template<class T>
const T* DynamicSharedPtr<T>::constData() const
{
    return d->constData();
}

/** Return a pointer to the shared data - this calls 'detach()'. */
template<class T>
T* DynamicSharedPtr<T>::data()
{
    return d->data();
}

/** Return a const pointer to the shared data - this does not call 'detach()' */
template<class T>
const T* DynamicSharedPtr<T>::data() const
{
    return d->data();
}

/** If the reference count is more than one, then create a clone of the object */
template<class T>
void DynamicSharedPtr<T>::detach()
{
    d.detach();
}

/** Return a pointer to the object - this calls 'detach()' */
template<class T>
DynamicSharedPtr<T>::operator T*()
{
    return d->data();
}

/** Return a const pointer to the shared data - this does not call 'detach()' */
template<class T>
DynamicSharedPtr<T>::operator const T*() const
{
    return d->data();
}

/** Return true if the pointer is null, otherwise false */
template<class T>
bool DynamicSharedPtr<T>::operator!() const
{
    return d->data() == 0;
}

/** Returns true if the pointer to the data in this pointer is the same as the
    pointer to the data held by 'other'. This does not call 'detach()' */
template<class T>
bool DynamicSharedPtr<T>::operator==( const DynamicSharedPtr<T> & other ) const
{
    return d->data() == other.data();
}

/** Returns true if the pointer to the data in this pointer is not the same as the
    pointer to the data held by 'other'. This does not call 'detach()' */
template<class T>
bool DynamicSharedPtr<T>::operator!=(const DynamicSharedPtr<T> &other) const
{
    return d->data() != other.data();
}

/** Dereference the pointer - does call 'detach()' */
template<class T>
T& DynamicSharedPtr<T>::operator*()
{
    return *(d->data());
}

/** Dereference the pointer - does not call 'detach()' */
template<class T>
const T& DynamicSharedPtr<T>::operator*() const
{
    return *(d->data());
}

/** Pointer-operator - calls 'detach()' */
template<class T>
T* DynamicSharedPtr<T>::operator->()
{
    return d->data();
}

/** Pointer-operator - does not call 'detach()' */
template<class T>
const T* DynamicSharedPtr<T>::operator->() const
{
    return d->data();
}

/** Assignment operator - makes an implicit copy of the object pointed to
    by 'other' */
template<class T>
DynamicSharedPtr<T>& DynamicSharedPtr<T>::operator=( const DynamicSharedPtr<T> &other )
{
    d = other.d;
    return *this;
}

/** Assignment operator - set this pointer pointing to the object pointed to
    by 'shared_data'. This takes over ownership of this object and may delete
    it at any time. */
template<class T>
DynamicSharedPtr<T>& DynamicSharedPtr<T>::operator=( T *shared_data )
{
    d = new detail::DynamicSharedData<T>(shared_data);
    return *this;
}

/** Assignment operator - set this pointer pointing to a clone of 'obj' */
template<class T>
DynamicSharedPtr<T>& DynamicSharedPtr<T>::operator=( const T &obj )
{
    d = new detail::DynamicSharedData<T>( clone<T>(obj) );
    return *this;
}

/** Return whether or not the object pointed to by this pointer is of
    type 'S'. This does not call 'detach()' */
template<class T>
template<class S>
bool DynamicSharedPtr<T>::isA() const
{
    return dynamic_cast<const S*>( d->data() ) != 0;
}

/** Return the object pointed to by this pointer cast as type 'S'. Note
    that this will return undefined results unless isA<S> returns true.
    This will not call 'detach()' */
template<class T>
template<class S>
const S& DynamicSharedPtr<T>::asA() const
{
    return dynamic_cast<const S&>( *(d->data()) );
}

/** Return the object pointed to by this pointer cast as type 'S'. Note
    that this will return undefined results unless isA<S> returns true.
    This calls 'detach()' */
template<class T>
template<class S>
S& DynamicSharedPtr<T>::asA()
{
    return dynamic_cast<S&>( *(d->data()) );
}

const SireStream::MagicID dynamicsharedptr_magic = SireStream::getMagic(
                                                          "SireBase::DynamicSharedPtr");

/** Default implementation of the helper class used to
    clone and query a polymorphic class.

    @author Christopher Woods
*/
template<class T>
struct SharedPolymorphicPointerHelper
{
    /** Return a copy of the object (a clone). Returns
        a null pointer if 'obj' is null. */
    static T* clone(const T *obj)
    {
        if (obj)
            return obj->clone();
        else
            return 0;
    }

    /** Return the type of the object - returns a
        null pointer if 'obj' is null */
    static const char* what(const T *obj)
    {
        if (obj)
            return obj->what();
        else
            return 0;
    }
};

/** This is a version of QSharedDataPointer that has been modified to
    work with polymorphic objects (i.e. will 'clone()' the original
    rather than using the copy constructor).

    This class is copied from QSharedDataPointer in qshareddata.h
    from the Trolltech Qt 4.2.1 distribution (C) Trolltech.com

    Modifications from the original QSharedDataPointer are highlighted,
    and are (C) Christopher Woods. (obviously all QSharedDataPointer are
    changed to SharedPolymorphicPointer)

*/
template <class T>
class SharedPolymorphicPointer
{
public:
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

    inline bool operator==(const SharedPolymorphicPointer<T> &other) const
    {
        return d == other.d;
    }

    inline bool operator!=(const SharedPolymorphicPointer<T> &other) const
    {
        return d != other.d;
    }

    inline SharedPolymorphicPointer() { d = 0; }
    inline ~SharedPolymorphicPointer() { if (d && !d->ref.deref()) delete d; }

    explicit SharedPolymorphicPointer(T *data);

    inline SharedPolymorphicPointer(const SharedPolymorphicPointer<T> &o) : d(o.d)
    {
        if (d) d->ref.ref();
    }

    inline SharedPolymorphicPointer<T> & operator=(const SharedPolymorphicPointer<T> &o)
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

    inline SharedPolymorphicPointer &operator=(T *o)
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

    inline bool operator!() const { return !d; }

    /** CW Mod - additional member function used to return the
        type of the object */
    inline const char* what() const
    {
        return SharedPolymorphicPointerHelper<T>::what(d);
    }

    /** CW Mod - used to see whether or not the object is of
        type 'D' */
    template<class D>
    bool isA() const
    {
        return dynamic_cast<const D*>(d);
    }

    /** CW Mod - used to return the function cast as type 'D' -
        Must be non-null and 'isA<D>()' must return true */
    template<class D>
    inline D& asA() { detach(); return dynamic_cast<D&>(*d); }

    /** CW Mod - used to return the function cast as type 'D' -
        Must be non-null and 'isA<D>()' must return true */
    template<class D>
    inline const D& asA() const { return dynamic_cast<const D&>(*d); }

    template<class D>
    inline const D& constAsA() const { return dynamic_cast<const D&>(*d); }

private:
    void detach_helper();

    T *d;
};

template <class T>
Q_INLINE_TEMPLATE
SharedPolymorphicPointer<T>::SharedPolymorphicPointer(T *adata) : d(adata)
{
    if (d) d->ref.ref();
}

template <class T>
Q_OUTOFLINE_TEMPLATE
void SharedPolymorphicPointer<T>::detach_helper()
{
    /** Modification by C.Woods - use helper function
        to clone the object */
    T *x = SharedPolymorphicPointerHelper<T>::clone(d);

    x->ref.ref();
    x = qAtomicSetPtr(&d, x);
    if (!x->ref.deref())
        delete x;
}


}


/** Serialise to a binary data stream */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
QDataStream& operator<<(QDataStream &ds,
                        const SireBase::detail::DynamicSharedData<T> &data)
{
    SireStream::writeHeader(ds, SireBase::dynamicsharedptr_magic, 1);

    if (data.objptr == 0)
        ds << qint32(0);
    else
    {
        ds << qint32(1);

        //get the object type name
        QString objname = SireBase::what<T>( *(data.objptr) );

        //save the object type name
        ds << objname;

        //get the ID number of this type
        int id = QMetaType::type( objname.toLatin1().constData() );

        if ( id == 0 or not QMetaType::isRegistered(id) )
            throw SireError::unknown_type(QObject::tr(
                "The object with type \"%1\" does not appear to have been "
                "registered with QMetaType. It cannot be streamed! (%2, %3)")
                    .arg(objname).arg(id).arg(QMetaType::isRegistered(id)), CODELOC);

        //use the QMetaType streaming function to save this table
        if (not QMetaType::save(ds, id, data.objptr))
            throw SireError::program_bug(QObject::tr(
                "There was an error saving the table of type \"%1\"")
                    .arg(objname), CODELOC);
    }

    return ds;
}

/** Deserialise from a binary data stream */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
QDataStream& operator>>(QDataStream &ds,
                        SireBase::detail::DynamicSharedData<T> &data)
{
    SireStream::VersionID v = SireStream::readHeader(ds, SireBase::dynamicsharedptr_magic,
                                                     "SireMM::DynamicSharedPtr");

    if (v == 1)
    {
        //is this a null pointer
        qint32 hasdata;
        ds >> hasdata;

        if (hasdata)
        {
            //there is some data! read in the type name
            QString type_name;
            ds >> type_name;

            //get the type that represents this name
            int id = QMetaType::type( type_name.toLatin1().constData() );

            if ( id == 0 or not QMetaType::isRegistered(id) )
                throw SireStream::version_error( QObject::tr(
                      "Cannot deserialise an object of type \"%1\". "
                      "Ensure that the library or module containing "
                      "this type has been loaded and that it has been registered "
                      "with QMetaType.").arg(type_name), CODELOC );

            //if the pointer is not pointing to an object of the right type
            //then delete it
            if (data.objptr != 0 and
                      type_name != QLatin1String( SireBase::what<T>(*(data.objptr)) ))
            {
                delete data.objptr;
                data.objptr = 0;
            }

            //construct a new object if one doesn't already exist
            if (data.objptr == 0)
            {
                //create a default-constructed object of this type
                data.objptr = static_cast<T*>(QMetaType::construct(id,0));

                if (not data.objptr)
                    throw SireError::program_bug( QObject::tr(
                            "Could not create an object of type \"%1\" despite "
                            "this type having been registered with QMetaType. This is "
                            "a program bug!!!").arg(type_name), CODELOC );
            }

            //load the object from the datastream
            if ( not QMetaType::load(ds, id, data.objptr) )
                throw SireError::program_bug(QObject::tr(
                    "There was an error loading the object of type \"%1\"")
                        .arg(type_name), CODELOC);

        }
        else
        {
            //this is a null pointer
            delete data.objptr;
            data.objptr = 0;
        }
    }
    else
        throw SireStream::version_error(v, "1", "SireBase::DynamicSharedPtr", CODELOC);

    return ds;
}

/** Serialise to a binary data stream */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
QDataStream& operator<<(QDataStream &ds,
                        const SireBase::DynamicSharedPtr<T> &dptr)
{
    ds << *(dptr.d);

    return ds;
}

/** Deserialise from a binary data stream */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
QDataStream& operator>>(QDataStream &ds,
                        SireBase::DynamicSharedPtr<T> &dptr)
{
    ds >> *(dptr.d);

    return ds;
}

SIRE_END_HEADER

#endif
