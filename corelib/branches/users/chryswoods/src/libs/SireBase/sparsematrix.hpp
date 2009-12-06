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

#include <QHash>

#include "sireglobal.h"

#include "Siren/primitive.h"

SIRE_BEGIN_HEADER

namespace SireBase
{
template<class T> class SparseMatrix;
}

template<class T>
Siren::Stream& operator&(Siren::Stream&, SireBase::SparseMatrix<T>&);

namespace SireBase
{

/** Small class used to index the SparseMatrix */
class SparseIndex : public Siren::Primitive<SparseIndex>
{
public:
    SparseIndex(quint32 ii=0, quint32 jj=0) 
            : Siren::Primitive<SparseIndex>(), i(ii), j(jj)
    {}

    SparseIndex(const SparseIndex &other) 
            : Siren::Primitive<SparseIndex>(), i(other.i), j(other.j)
    {}

    ~SparseIndex()
    {}

    SparseIndex& operator=(const SparseIndex &other)
    {
        i = other.i;
        j = other.j;
        return *this;
    }

    bool operator==(const SparseIndex &other) const
    {
        return i == other.i and j == other.j;
    }

    bool operator!=(const SparseIndex &other) const
    {
        return i != other.i or j != other.j;
    }
    
    uint hashCode() const
    {
        return (i << 16) & (j | 0x0000FFFF);
    }

    QString toString() const
    {
        return QString("(%1,%2)").arg(i).arg(j);
    }

    void stream(Siren::Stream &s);

private:
    quint32 i;
    quint32 j;
};

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

template<class U> friend class SparseMatrix;

friend Siren::Stream& operator&<>(Siren::Stream&, SparseMatrix<T>&);

public:
    SparseMatrix(const T &default_value = T(), 
                 bool is_symmetric=false);

    template<class U>
    SparseMatrix(const SparseMatrix<U> &other);

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
    bool isTranspose() const;

    const T& defaultValue() const;

    SparseMatrix<T> transpose() const;
    
    const void* shareKey() const;
    
private:
    /** The default value of each element of the matrix */
    T def;

    /** Hash which is used to store all of the elements of the matrix */
    QHash<SparseIndex,T> data;

    /** Is this is a symmetric matrix? */
    bool is_symmetric;
    
    /** Is the transpose stored? */
    bool is_transpose;
};    

#ifndef SIRE_SKIP_INLINE_FUNCTIONS

/** Construct an empty sparse matrix */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
SparseMatrix<T>::SparseMatrix(const T &default_value,
                              bool is_sym) 
                : is_symmetric(is_sym), is_transpose(false), def(default_value)
{}

/** Construct this SparseMatrix by casting the passed SparseMatrix<U> */
template<class T>
template<class U>
SIRE_OUTOFLINE_TEMPLATE
SparseMatrix<T>::SparseMatrix(const SparseMatrix<U> &other)
                : is_symmetric(other.is_symmetric),
                  is_transpose(other.is_transpose)
{
    def = T(other.def);

    for (typename QHash<SparseIndex,U>::const_iterator it = other.data.constBegin();
         it != other.data.constEnd();
         ++it)
    {
        data.insert( it.key(), T(it.value()) );
    }
}

/** Copy constructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
SparseMatrix<T>::SparseMatrix(const SparseMatrix<T> &other)
                : def(other.def), data(other.data),
                  is_symmetric(other.is_symmetric), is_transpose(other.is_transpose)
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
        is_symmetric = other.is_symmetric;
        is_transpose = other.is_transpose;
    }

    return *this;
}

/** Comparison operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool SparseMatrix<T>::operator==(const SparseMatrix<T> &other) const
{
    return is_symmetric == other.is_symmetric and
           is_transpose == other.is_transpose and
           def == other.def and
           data == other.data;
}

/** Comparison operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool SparseMatrix<T>::operator!=(const SparseMatrix<T> &other) const
{
    return not SparseMatrix<T>::operator==(other);
}

/** Return the element at index (i,j) - this returns the default
    constructed value if there is no element at this index */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const T& SparseMatrix<T>::operator()(quint32 i, quint32 j) const
{
    SparseIndex idx;
    
    if ( is_transpose or
         (is_symmetric and j < i) )
    {
        idx = SparseIndex(j,i);
    }
    else
    {
        idx = SparseIndex(i,j);
    }

    typename QHash<SparseIndex,T>::const_iterator it = data.constFind(idx);

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
    SparseIndex idx;
    
    if ( is_transpose or
         (is_symmetric and j < i) )
    {
        idx = SparseIndex(j,i);
    }
    else
    {
        idx = SparseIndex(i,j);
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
    return is_symmetric;
}

/** Return whether or not the transpose is stored */
template<class T>
SIRE_INLINE_TEMPLATE
bool SparseMatrix<T>::isTranspose() const
{
    return is_transpose;
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

    if (not ret.is_symmetric)
        ret.is_transpose = not is_transpose;
    
    return ret;
}

#endif //SIRE_SKIP_INLINE_FUNCTIONS

} // end of namespace SireBase

Q_DECLARE_METATYPE( SireBase::SparseIndex )

SIRE_END_HEADER

#endif
