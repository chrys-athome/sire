/**
  * @file
  *
  * C++ Interface: SharedDataStream
  *
  * Description: Implementation of SharedDataStream
  *
  *
  * @author Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */
#ifndef SIRESTREAM_SHAREDDATASTREAM_H
#define SIRESTREAM_SHAREDDATASTREAM_H

#include "sireglobal.h"

#include <QDataStream>
#include <QSharedDataPointer>

#include <boost/shared_ptr.hpp>

SIRE_BEGIN_HEADER

namespace SireStream
{

/**
This is a streaming class that is used to help stream implicitly shared data. This class ensures that only a single copy of the implicitly shared data is actually written to the stream, thus reducing data bloat, and also preserving the shared data of the objects when they are read back into the program.

@author
*/
class SIRESTREAM_EXPORT SharedDataStream
{
public:
    SharedDataStream(QDataStream &ds);

    ~SharedDataStream();

    template<class T>
    SharedDataStream& operator<<(const T &obj);

    template<class T>
    SharedDataStream& operator<<(const QSharedDataPointer<T> &obj);

    template<class T>
    SharedDataStream& operator<<(const boost::shared_ptr<T> &obj);

    template<class T>
    SharedDataStream& operator>>(T &obj);

    template<class T>
    SharedDataStream& operator>>(QSharedDataPointer<T> &obj);

    template<class T>
    SharedDataStream& operator>>(boost::shared_ptr<T> &obj);

private:
    /** Reference to the actual QDataStream that is used to
        stream the data */
    QDataStream &ds;

};

/** Default serialisation function used as a wrapper around the standard
    QDataStream functions. This is called for any types that are
    not implicitly shared. */
template<class T>
SharedDataStream& SharedDataStream::operator<<(const T &obj)
{
    ds << obj;
    return *this;
}

/** Default deserialisation function used as a wrapper around the standard
    QDataStream functions. This is called for any types that are
    not implicitly shared. */
template<class T>
SharedDataStream& SharedDataStream::operator>>(T &obj)
{
    ds >> obj;
    return *this;
}

/** Specialisation of the serialisation function for implicitly shared
    objects handled by QSharedDataPointer. This will stream the object
    in a way that ensures that only a single copy of the data is passed
    to the stream, with multiple copies being merely references to the
    first copy. */
template<class T>
SharedDataStream& SharedDataStream::operator<<(const QSharedDataPointer<T> &objptr)
{
    ds << *objptr;
    return *this;
}

/** Specialisation of the deserialisation function for implicitly shared
    objects handled by QSharedDataPointer. This will destream the object
    in a way that ensures that the implicitly shared nature of the
    data is preserved. */
template<class T>
SharedDataStream& SharedDataStream::operator>>(QSharedDataPointer<T> &objptr)
{
    ds >> *objptr;
    return *this;
}

/** Specialisation of the serialisation function for explicitly shared
    objects handled by boost::shared_ptr. This will stream the object
    in a way that ensures that only a single copy of the data is passed
    to the stream, with multiple copies being merely references to the
    first copy. */
template<class T>
SharedDataStream& SharedDataStream::operator<<(const boost::shared_ptr<T> &objptr)
{
    ds << *objptr;
    return *this;
}

/** Specialisation of the deserialisation function for explicitly shared
    objects handled by boost::shared_ptr. This will destream the object
    in a way that ensures that the explicitly shared nature of the
    data is preserved. */
template<class T>
SharedDataStream& SharedDataStream::operator>>(boost::shared_ptr<T> &objptr)
{
    ds >> *objptr;
    return *this;
}

}

SIRE_END_HEADER

#endif
