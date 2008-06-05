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

#ifndef SIRESTREAM_SHAREDDATASTREAM_H
#define SIRESTREAM_SHAREDDATASTREAM_H

#include "sireglobal.h"

#include <QDataStream>
#include <QSharedDataPointer>

#include <boost/shared_ptr.hpp>

#include "SireBase/shareddatapointer.hpp"
#include "SireBase/sharedpolypointer.hpp"

SIRE_BEGIN_HEADER

namespace SireStream
{

using SireBase::SharedDataPointer;
using SireBase::SharedPolyPointer;

/**
This is a streaming class that is used to help stream implicitly shared data. 
This class ensures that only a single copy of the implicitly shared data is actually 
written to the stream, thus reducing data bloat, and also preserving the shared data
of the objects when they are read back into the program.

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
    SharedDataStream& operator<<(const SharedDataPointer<T> &obj);
    
    template<class T>
    SharedDataStream& operator<<(const SharedPolyPointer<T> &obj);

    template<class T>
    SharedDataStream& operator<<(const boost::shared_ptr<T> &obj);

    template<class T>
    SharedDataStream& operator>>(T &obj);

    template<class T>
    SharedDataStream& operator>>(QSharedDataPointer<T> &obj);

    template<class T>
    SharedDataStream& operator>>(SharedDataPointer<T> &obj);
    
    template<class T>
    SharedDataStream& operator>>(SharedPolyPointer<T> &obj);

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

/** Specialisation of the serialisation function for implicitly shared
    objects handled by SharedDataPointer. This will stream the object
    in a way that ensures that only a single copy of the data is passed
    to the stream, with multiple copies being merely references to the
    first copy. */
template<class T>
SharedDataStream& SharedDataStream::operator<<(const SharedDataPointer<T> &objptr)
{
    ds << *objptr;
    return *this;
}

/** Specialisation of the deserialisation function for implicitly shared
    objects handled by SharedDataPointer. This will destream the object
    in a way that ensures that the implicitly shared nature of the
    data is preserved. */
template<class T>
SharedDataStream& SharedDataStream::operator>>(SharedDataPointer<T> &objptr)
{
    ds >> *objptr;
    return *this;
}

/** Specialisation of the serialisation function for implicitly shared
    objects handled by SharedPolyPointer. This will stream the object
    in a way that ensures that only a single copy of the data is passed
    to the stream, with multiple copies being merely references to the
    first copy. */
template<class T>
SharedDataStream& SharedDataStream::operator<<(const SharedPolyPointer<T> &objptr)
{
    ds << objptr;
    return *this;
}

/** Specialisation of the deserialisation function for implicitly shared
    objects handled by SharedPolyPointer. This will destream the object
    in a way that ensures that the implicitly shared nature of the
    data is preserved. */
template<class T>
SharedDataStream& SharedDataStream::operator>>(SharedPolyPointer<T> &objptr)
{
    ds >> objptr;
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
