/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2008  Christopher Woods
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

#ifndef SIREBASE_ARRAY2D_H
#define SIREBASE_ARRAY2D_H

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireBase
{
class Array2DBase;
}

QDataStream& operator<<(QDataStream&, const SireBase::Array2DBase&);
QDataStream& operator>>(QDataStream&, SireBase::Array2DBase&);

namespace SireBase
{

/** Base class of the Array2D<T> class

    @author Christopher Woods
*/
class SIREMOL_EXPORT Array2DBase
{

friend QDataStream& ::operator<<(QDataStream&, const Array2DBase&);
friend QDataStream& ::operator>>(QDataStream&, Array2DBase&);

public:
    ~Array2DBase();
    
    quint32 nRows() const;
    quint32 nColumns() const;
    
    quint32 map(quint32 i, quint32 j) const;
    
    void assertValidIndex(quint32 i, quint32 j) const;

protected:
    Array2DBase();

    Array2DBase(quint32 nrows, quint32 ncolumns);

    Array2DBase(const Array2DBase &other);

    Array2DBase& operator=(const Array2DBase &other);

private:
    void throwInvalidIndex(quint32 i, quint32 j) const;
    
    /** The number of rows and columns in the matrix */
    quint32 nrows, ncolumns;
};

/** Return the number of rows in the matrix */
inline quint32 Array2DBase::nRows() const
{
    return nrows;
}

/** Return the number of columns in the matrix */
inline quint32 Array2DBase::nColumns() const
{
    return ncolumns;
}

/** Map the 2D index (i,j) into the 1D index into memory */
inline quint32 Array2DBase::map(quint32 i, quint32 j) const
{
    return (i*nrows) + j;
}

/** Assert that the index (i,j) is valid for this matrix

    \throw SireError::invalid_index
*/
inline void Array2DBase::assertValidIndex(quint32 i, quint32 j) const
{
    if (i >= nrows or j >= ncolumns)
        throwInvalidIndex(i,j);
}

}

SIRE_END_HEADER

#endif
