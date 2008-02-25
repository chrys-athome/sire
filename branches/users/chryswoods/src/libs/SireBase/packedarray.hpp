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

#ifndef SIREBASE_PACKEDARRAY_HPP
#define SIREBASE_PACKEDARRAY_HPP

#include "packedarray.h"

SIRE_BEGIN_HEADER

namespace SireBase
{

namespace detail
{

/** This is a simple class that is used to manage the memory and 
    reference counting for the PackedArray */
template<class T>
class ArrayMemory : public ArrayMemoryBase
{
public:
    static char* create(quint32 narrays, quint32 nvalues);
    static char* detach(char *this_ptr, quint32 this_idx);
    static void destroy(PackedArrayData *arrays);
    
    static void incref(char *this_ptr, quint32 this_idx);
    static void decref(char *this_ptr, quint32 this_idx);
};

template<class T>
class SharedArrayPtr
{
};

template<class T>
class PackedArrayData
{
};

template<class T>
class PackedArray_Array
{
};

////////
//////// Implementation of ArrayMemory
////////

/** Create space for the arrays. The layout in memory is;

    --------------------------------------------------
    | A |   B   |    C    |            D             |
    --------------------------------------------------

      A = PackedArrayData<T> object          1 * sizeof(PackedArrayData<T>)
      B = PackedArray_Array<T> objects       narrays * sizeof(PackedArray_Array<T>)
      C = PackedArray_ArrayData<T> objects   narrays * sizeof( " _ArrayData<T>)
      D = Array of T objects                 nvalues * sizeof(T)
*/
template<class T>
char* ArrayMemory<T>::create(quint32 narrays, quint32 nvalues)
{
    //create space for the data
    char *storage = ArrayMemoryBase::create(narrays, nvalues, sizeof(T));
    
    ...
}

template<class T>
char* ArrayMemory<T>::detach(char *this_ptr, quint32 this_idx);

template<class T>
void ArrayMemory<T>::destroy(PackedArrayData *arrays);
    
template<class T>
void ArrayMemory<T>::incref(char *this_ptr, quint32 this_idx);

template<class T>
void ArrayMemory<T>::decref(char *this_ptr, quint32 this_idx);

}

/** This class provides an array of arrays of type T, where
    each array is packed in memory so that each object of type
    T is contiguous. This allows rapid indexing over each
    object in each array, or over all objects in all arrays.
    
    @author Christopher Woods
*/
template<class T>
class PackedArray
{
public:
    typedef typename detail::PackedArray_Array<T> Array;

    PackedArray();
    PackedArray( const QVector< QVector<T> > &values );
    
    PackedArray(const PackedArray<T> &other);
    
    ~PackedArray();
    
    const Array& at(quint32 i) const;
    
    int count() const;

    const Array* constData() const;

private:
    /** Implicitly shared pointer to the array data */
    detail::SharedPtr<detail::PackedArrayData> d;
};

}

SIRE_END_HEADER

#endif
