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

#ifndef SIREBASE_ARRAY2D_HPP
#define SIREBASE_ARRAY2D_HPP

#include "array2d.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

SIRE_BEGIN_HEADER

namespace SireBase
{
template<class T>
class Array2D;
}

template<class T>
QDataStream& operator<<(QDataStream&, const SireBase::Array2D<T>&);
template<class T>
QDataStream& operator>>(QDataStream&, SireBase::Array2D<T>&);

namespace SireBase
{

/** This provides a 2D matrix of objects of type T. All objects
    are packed together in memory in row-major order, e.g.
    
        j0   j1   j2
    i0   0    1    2
    i1   3    4    5
    i2   6    7    8
    
    Arranged in memory as;
    
    [ (i0,j0), (i0,j1), (i0,j2), (i1,j0),  ... etc. ]
    [    0   ,    1   ,    2   ,   3    ,  ... etc. ]
    
    @author Christopher Woods
*/
template<class T>
class Array2D
{

friend QDataStream& ::operator<<<>(QDataStream&, const Array2D<T>&);
friend QDataStream& ::operator>><>(QDataStream&, Array2D<T>&);

public:
    Array2D();

    Array2D(quint32 nrows, quint32 ncolumns);

    Array2D(const Array2D<T> &other);

    ~Array2D();

    Array2D<T>& operator=(const Array2D<T> &other);

    const T& operator()(quint32 i, quint32 j) const;
    T& operator()(quint32 i, quint32 j);
    const T& at(quint32 i, quint32 j) const;

    void redimension(quint32 nrows, quint32 ncolumns);

    const T* data() const;
    T* data();
    const T* constData() const;

    Array2D<T> transpose() const;

private:
    /** The 1D array of entries in this Array2D */
    QVector<T> array;
};

/** Construct a null Array2D */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Array2D<T>::Array2D() : Array2DBase()
{}

/** Construct a Array2D that holds nrow rows of ncolumn columns. */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Array2D<T>::Array2D(quint32 nrows, quint32 ncolumns)
           : Array2DBase(nrows,ncolumns)
{
    if (nrows*ncolumns > 0)
    {
        array = QVector<T>(nrows*ncolumns);
        array.squeeze();
    }
}

/** Copy constructor. Fast as this class is implicitly shared */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Array2D<T>::Array2D(const Array2D<T> &other)
           : Array2DBase(other), array(other.array)
{}

/** Destructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Array2D<T>::~Array2D()
{}

/** Copy assignment operator - fast as this class is implicitly 
    shared */
template<class T>
SIRE_INLINE_TEMPLATE
Array2D<T>& Array2D<T>::operator=(const Array2D<T> &other)
{
    Array2DBase::operator=(other);
    array = other.array;
    return *this;
}

/** Return a reference to the object in the ith row and
    the jth column 
    
    \throw SireError::invalid_index
*/
template<class T>
SIRE_INLINE_TEMPLATE
const T& Array2D<T>::operator()(quint32 i, quint32 j) const
{
    Array2DBase::assertValidIndex(i,j);
    return array.constData()[ Array2DBase::map(i,j) ];
}

/** Return a reference to the object in the ith row and
    the jth column 
    
    \throw SireError::invalid_index
*/
template<class T>
SIRE_INLINE_TEMPLATE
T& Array2D<T>::operator()(unsigned int i, unsigned int j)
{
    Array2DBase::assertValidIndex(i,j);
    return array.data()[ Array2DBase::map(i,j) ];
}

/** Return a reference to the object in the ith row and
    the jth column 
    
    \throw SireError::invalid_index
*/
template<class T>
SIRE_INLINE_TEMPLATE
const T& Array2D<T>::at(quint32 i, quint32 j) const
{
    return Array2D<T>::operator()(i,j);
}

/** Redimension this Array2D to nrows by ncolumns objects.
    This will keep any existing data in the top left of the
    matrix if the matrix gets bigger, or it will crop any
    extra data to the bottom right if the matrix gets smaller 
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void Array2D<T>::redimension(quint32 nrows, quint32 ncolumns)
{
    if (nrows == this->nRows() and ncolumns == this->nColumns())
        return;

    Array2D<T> new_array(nrows, ncolumns);
    
    //copy the data...
    nrows = qMin(nrows, this->nRows());
    ncolumns = qMin(ncolumns, this->nColumns());
    
    T *data = new_array.data();
    T *old_data = array.constData();
    
    for (quint32 i=0; i<nrows; ++i)
    {
        for (quint32 j=0; j<ncolumns; ++j)
        {
            data[this->map(i,j)] = old_data[this->map(i,j)];
        }
    }
    
    this->operator=(new_array);
}

/** Return a raw pointer into the data of the array. Remember
    that the data is in row-major order */
template<class T>
SIRE_INLINE_TEMPLATE
const T* Array2D<T>::data() const
{
    return array.data();
}

/** Return a raw pointer into the data of the array. Remember
    that the data is in row-major order */
template<class T>
SIRE_INLINE_TEMPLATE
T* Array2D<T>::data()
{
    return array.data();
}

/** Return a raw pointer into the data of the array. Remember
    that the data is in row-major order */
template<class T>
SIRE_INLINE_TEMPLATE
const T* Array2D<T>::constData() const
{
    return array.constData();
}

/** Return the transpose of this matrix */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Array2D<T> Array2D<T>::transpose() const
{
    if (array.isEmpty())
        return Array2D<T>();
    
    Array2D trans( this->nColumns(), this->nRows() );
    
    T *new_array = trans.data();
    const T *old_array = array.constData();
    
    for (quint32 i=0; i < this->nRows(); ++i)
    {
        for (quint32 j=0; j < this->nColumns(); ++j)
        {
            new_array[trans.map(j,i)] = old_array[this->map(i,j)];
        }
    }
}

}

/** Serialise to a binary datastream */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
QDataStream& operator<<(QDataStream &ds, const SireBase::Array2D<T> &array)
{
    SireStream::SharedDataStream sds(ds);
    
    sds << static_cast<const SireBase::Array2DBase&>(array)
        << array.array;
        
    return ds;
}

/** Extract from a binary datastream */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
QDataStream& operator>>(QDataStream &ds, SireBase::Array2D<T> &array)
{
    SireStream::SharedDataStream sds(ds);
    
    sds >> static_cast<SireBase::Array2DBase&>(array)
        >> array.array;
        
    return ds;
}

SIRE_END_HEADER

#endif
