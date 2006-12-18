#ifndef SIREBASE_SHAREDPOLYPOINTER_HPP
#define SIREBASE_SHAREDPOLYPOINTER_HPP

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include "SireError/errors.h"

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
class SharedPolyPointer
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

    inline SharedPolyPointer() { d = 0; }
    inline ~SharedPolyPointer() { if (d && !d->ref.deref()) delete d; }

    explicit SharedPolyPointer(T *data);

    inline SharedPolyPointer(const SharedPolyPointer<T> &o) : d(o.d)
    {
        if (d) d->ref.ref();
    }

    template<class S>
    inline SharedPolyPointer(const SharedPolyPointer<S> &o)
             : d( const_cast<T*>( dynamic_cast<const T*>(o.constData()) ) )
    {
        if (o.constData())
        {
            if (d)
                d->ref.ref();
            else
                throw SireError::invalid_cast( QObject::tr(
                        "Cannot cast a SharedPolyPointer of type \"%1\".")
                            .arg( SharedPolyPointerHelper<S>::what(*o) ), CODELOC );
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
                    throw SireError::invalid_cast( QObject::tr(
                        "Cannot cast a SharedPolyPointer of type \"%1\".")
                            .arg( SharedPolyPointerHelper<S>::what(*o) ), CODELOC );

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

    /** CW Mod - additional member function used to return the
        type of the object */
    inline const char* what() const
    {
        return SharedPolyPointerHelper<T>::what(d);
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
SharedPolyPointer<T>::SharedPolyPointer(T *adata) : d(adata)
{
    if (d) d->ref.ref();
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

const SireStream::MagicID sharedpolypointer_magic = SireStream::getMagic(
                                                      "SireBase::SharedPolyPointer");
}

/** Serialise to a binary data stream */
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
        QString objname = SireBase::SharedPolyPointerHelper<T>::what( *(ptr.d) );

        //get the ID number of this type
        int id = QMetaType::type( objname.toLatin1().constData() );

        if ( id == 0 or not QMetaType::isRegistered(id) )
            throw SireError::unknown_type(QObject::tr(
                "The object with type \"%1\" does not appear to have been "
                "registered with QMetaType. It cannot be streamed! (%2, %3)")
                    .arg(objname).arg(id).arg(QMetaType::isRegistered(id)), CODELOC);

        //save the object type name
        ds << objname;

        //use the QMetaType streaming function to save this table
        if (not QMetaType::save(ds, id, ptr.d))
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
                        SireBase::SharedPolyPointer<T> &ptr)
{
    SireStream::VersionID v = SireStream::readHeader(ds, SireBase::sharedpolypointer_magic,
                                                     "SireBase::SharedPolyPointer");

    if (v == 1)
    {
        //read the type name
        QString type_name;
        ds >> type_name;

        if (type_name.isNull())
        {
            //this is a null pointer
            ptr = 0;
        }
        else
        {
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
            if ( ptr.d != 0 and
                 type_name !=
                    QLatin1String(
                        SireBase::SharedPolyPointerHelper<T>::what( *(ptr.d))
                                 )
               )
            {
                ptr = 0;
            }

            //construct a new object if one doesn't already exist
            if (ptr.d == 0)
            {
                //create a default-constructed object of this type
                ptr = static_cast<T*>(QMetaType::construct(id,0));

                if (not ptr.d)
                    throw SireError::program_bug( QObject::tr(
                            "Could not create an object of type \"%1\" despite "
                            "this type having been registered with QMetaType. This is "
                            "a program bug!!!").arg(type_name), CODELOC );
            }

            //load the object from the datastream
            if ( not QMetaType::load(ds, id, ptr.data()) )
                throw SireError::program_bug(QObject::tr(
                    "There was an error loading the object of type \"%1\"")
                        .arg(type_name), CODELOC);

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
