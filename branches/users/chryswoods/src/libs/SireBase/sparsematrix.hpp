/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007  Christopher Woods
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

#ifndef SIREBASE_SPARSEMATRIX_HPP
#define SIREBASE_SPARSEMATRIX_HPP

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireBase
{
template<class T>
class SparseMatrix;

namespace detail { class Index; }

}

uint qHash(const SireBase::detail::Index&);

#include <QHash>

template<class T>
QDataStream& operator<<(QDataStream&, const SireBase::SparseMatrix<T>&);
template<class T>
QDataStream& operator>>(QDataStream&, SireBase::SparseMatrix<T>&);

namespace SireBase
{

namespace detail
{

/** Small class used to index a matrix */
class Index
{
public:
    Index(quint32 ii=0, quint32 jj=0) : i(ii), j(jj)
    {}

    Index(const Index &other) : i(other.i), j(other.j)
    {}

    ~Index()
    {}

    Index& operator=(const Index &other)
    {
        i = other.i;
        j = other.j;
        return *this;
    }

    bool operator==(const Index &other)
    {
        return i == other.i and j == other.j;
    }

    bool operator!=(const Index &other)
    {
        return i != other.i or j != other.j;
    }

    quint32 i;
    quint32 j;
};

} // end of namespace detail

} // end of namespace SireBase

uint qHash(const SireBase::detail::Index &idx)
{
    return (idx.i << 16) & (idx.j | 0x0000FFFF);
}

namespace SireBase
{

/** This simple template class is used to provide a sparse matrix
    that can efficiently hold a matrix of objects. This class is
    *not* designed for numerical computation - it is merely to be
    used to efficiently hold a sparse matrix of objects.

    @author Christopher Woods
*/
template<class T>
class SIREBASE_EXPORT SparseMatrix
{
public:
    SparseMatrix(const T &default_value = T());

    SparseMatrix(const SparseMatrix<T> &other);

    ~SparseMatrix();

    SparseMatrix<T>& operator=(const SparseMatrix<T> &other);

    const T& operator()(quint32 i, quint32 j) const;

    void set(quint32 i, quint32 j, const T &value);
    const T& get(quint32 i, quint32 j) const;

    bool isEmpty() const;

    const T& defaultValue() const;

private:
    /** The default value of each element of the matrix */
    T def;

    /** Hash which is used to store all of the elements of the matrix */
    QHash<detail::Index,T> data;
};

/** Construct an empty sparse matrix */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
SparseMatrix<T>::SparseMatrix(const T &default_value) : def(default_value)
{}

/** Copy constructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
SparseMatrix<T>::SparseMatrix(const SparseMatrix<T> &other)
                : def(other.def), data(other.data)
{}

/** Destructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
SparseMatrix<T>::~SparseMatrix()
{}

/** Copy assignment operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
SparseMatrix<T>& SparseMatrix<T>::operator=(const SparseMatrix<T> &other)
{
    data = other.data;
    def = other.def;

    return *this;
}

/** Return the element at index (i,j) - this returns the default
    constructed value if there is no element at this index */
template<class T>
SIRE_INLINE_TEMPLATE
const T& SparseMatrix<T>::operator()(quint32 i, quint32 j) const
{
    detail::Index idx(i,j);

    typename QHash<detail::Index,T>::const_iterator it = data.constFind(idx);

    if (it != data.constEnd())
        return *it;
    else
        return def;
}

/** Set the element at (i,j) to equal 'value' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void SparseMatrix<T>::set(quint32 i, quint32 j, const T &value)
{
    if (value == def)
        data.remove( detail::Index(i,j) );
    else
        data.insert( detail::Index(i,j) ,value );
}

/** Return the element at index (i,j) - this returns the default
    constructed value if there is no element at this index */
template<class T>
SIRE_INLINE_TEMPLATE
const T& SparseMatrix<T>::get(quint32 i, quint32 j) const
{
    return this->operator()(i,j);
}

/** Return whether or not this matrix is empty (is filled with
    only the default value) */
template<class T>
SIRE_INLINE_TEMPLATE
bool SparseMatrix<T>::isEmpty() const
{
    return data.isEmpty();
}

}

SIRE_END_HEADER

#endif
