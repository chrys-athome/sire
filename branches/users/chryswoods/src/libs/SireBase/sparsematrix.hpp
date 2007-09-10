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

    Note that this matrix is *always* a 2^32 by 2^32 square 
    matrix - it is just that the vast majority of it has
    the default value.
    
    The matrix can be set to be symmetric, meaning that 
    the value at (i,j) is constrained to be equal to the 
    value at (j,i) - this means that setting (i,j) equal to
    'x' also sets the value of (j,i) equal to 'x'.
    
    It is *very* fast to take the transpose of this matrix - the
    implementation is such that taking the transpose just 
    involves setting a flag.
    
    @author Christopher Woods
*/
template<class T>
class SIREBASE_EXPORT SparseMatrix
{
public:
    SparseMatrix(const T &default_value = T(), 
                 bool is_symmetric=false);

    SparseMatrix(const SparseMatrix<T> &other);

    ~SparseMatrix();

    SparseMatrix<T>& operator=(const SparseMatrix<T> &other);

    bool operator==(const SparseMatrix<T> &other) const;
    bool operator!=(const SparseMatrix<T> &other) const;
    
    const T& operator()(quint32 i, quint32 j) const;

    void set(quint32 i, quint32 j, const T &value);
    const T& get(quint32 i, quint32 j) const;

    bool isEmpty() const;
    bool isSymmetric() const;

    const T& defaultValue() const;

    SparseMatrix<T> transpose() const;
    
private:
    /** Possible state of the sparse matrix */
    enum MATRIX_STATE
    {
        NORMAL,     //normal matrix
        TRANSPOSE,  //the transpose of the matrix is stored
        SYMMETRIC   //this is a symmetrix matrix
    }
    
    /** The state of this matrix */
    MATRIX_STATE current_state;
    
    /** The default value of each element of the matrix */
    T def;

    /** Hash which is used to store all of the elements of the matrix */
    QHash<detail::Index,T> data;
};

/** Construct an empty sparse matrix */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
SparseMatrix<T>::SparseMatrix(const T &default_value,
                              bool is_symmetric) 
                : current_state(NORMAL), def(default_value)
{
    if (is_symmetric)
        current_state = SYMMETRIC;
}

/** Copy constructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
SparseMatrix<T>::SparseMatrix(const SparseMatrix<T> &other)
                : current_state(other.current_state), def(other.def), data(other.data)
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
    if (this != &other)
    {
        data = other.data;
        def = other.def;
        current_state = other.current_state;        
    }

    return *this;
}

/** Comparison operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool SparseMatrix<T>::operator==(const SparseMatrix<T> &other) const
{
    return current_state == other.current_state and
           def == other.def and
           data == other.data;
}

/** Comparison operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool SparseMatrix<T>::operator!=(const SparseMatrix<T> &other) const
{
    return current_state != other.current_state or
           def != other.def or
           data != other.data;
}

/** Return the element at index (i,j) - this returns the default
    constructed value if there is no element at this index */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const T& SparseMatrix<T>::operator()(quint32 i, quint32 j) const
{
    detail::Index idx;
    
    if ( current_state == TRANSPOSE or
         (current_state == SYMMETRIC and j < i) )
    {
        idx = detail::Index(j,i);
    }
    else
    {
        idx = detail::Index(i,j);
    }

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
    detail::Index idx;
    
    if ( current_state == TRANSPOSE or
         (current_state == SYMMETRIC and j < i) )
    {
        idx = detail::Index(j,i);
    }
    else
    {
        idx = detail::Index(i,j);
    }
    
    if (value == def)
        data.remove(idx);
    else
        data.insert( idx, value );
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

/** Return whether or not this matrix is symmetric */
template<class T>
SIRE_INLINE_TEMPLATE
bool SparseMatrix<T>::isSymmetric() const
{
    return current_state == SYMMETRIC;
}

/** Return the default value of this sparse matrix */
template<class T>
SIRE_INLINE_TEMPLATE
const T& SparseMatrix<T>::defaultValue() const
{
    return def;
}

/** Return the transpose of this matrix */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
SparseMatrix<T> SparseMatrix<T>::transpose() const
{
    SparseMatrix<T> ret(*this);
    
    switch( this->current_state )
    {
        case NORMAL:
            ret.current_state = TRANSPOSE;
            break;
        case TRANSPOSE:
            ret.current_state = NORMAL;
            break;
        case SYMMETRIC:
            break;
    }
    
    return ret;
}

}

SIRE_END_HEADER

#endif
