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

#ifndef SIREBASE_PACKEDARRAY2D_H
#define SIREBASE_PACKEDARRAY2D_H

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireBase
{

namespace detail
{

/** Template-independent parts of PackedArray2DMemory */
class PackedArray2DMemoryBase
{
    static char* getRoot(char *this_ptr, quint32 this_idx);
    static const char* getRoot(const char *this_ptr, quint32 this_idx);

protected:
    static quint32 getSize(quint32 narrays, quint32 nvalues, 
                           quint32 sizeof_PackedArray2DData,
                           quint32 sizeof_PackedArray2D_Array,
                           quint32 sizeof_PackedArray2D_ArrayData,
                           quint32 sizeof_T

    static char* create(quint32 narrays, quint32 nvalues, 
                        quint32 sizeof_PackedArray2DData,
                        quint32 sizeof_PackedArray2D_Array,
                        quint32 sizeof_PackedArray2D_ArrayData,
                        quint32 sizeof_T);
};

/** This converts the pointer to the object
    that starts at 'this_ptr' to a pointer to the first element
    of the storage array (given the location of the object at
    index 'this_idx' in the storage array) */
inline char* PackedArray2DMemoryBase::getRoot(char *this_ptr, quint32 this_idx)
{
    return this_ptr - this_idx;
}

/** This converts the pointer to the object
    that starts at 'this_ptr' to a pointer to the first element
    of the storage array (given the location of the object at
    index 'this_idx' in the storage array) */
inline const char* PackedArray2DMemoryBase::getRoot(const char *this_ptr, 
                                                    quint32 this_idx)
{
    return this_ptr - this_idx;
}

/** The template independent parts of the PackedArray2D metadata */
class PackedArray2DDataBase : public QSharedData
{
public:
    PackedArray2DDataBase();
    PackedArray2DDataBase(quint32 narrays, quint32 nvalues);
    
    PackedArray2DDataBase(const PackedArray2DDataBase &other);
    
    ~PackedArray2DDataBase();

    void incref();
    
    void setNValuesInArray(quint32 i, quint32 nvalues);
    
    void close();
    
    quint32 nCGArrays() const;
    quint32 nCGroups() const;
    quint32 nCoords() const;
    
    char *memory();
    const char* memory() const;
    
private:
    /** The index in the storage array of the first PackedArray2D<T>::Array
        in this array */
    quint32 array0;
    
    /** The index in the storage array of the first PackedArray2D_ArrayData */
    quint32 arraydata0;
    
    /** The number of arrays in this array */
    quint32 narrays;
    
    /** The index in the storage array of the first object in
        this array */
    quint32 value0;
    
    /** The number of objects in this array */
    quint32 nvalues;
    
};

/** The template independent parts of the PackedArray2D_ArrayData metadata */
class PackedArray2D_ArrayDataBase
{
public:
    PackedArray2D_ArrayDataBase();
    PackedArray2D_ArrayDataBase(quint32 this_idx);
    
    PackedArray2D_ArrayDataBase(const PackedArray2D_ArrayDataBase &other);
    
    ~PackedArray2D_ArrayDataBase();

    const char* memory() const;
    char* memory();
    
    quint32 nValues() const;
    
private:
    /** The index in the storage array of this array */
    quint32 this_array;
    
    /** The index in the storage array of the first object in this array */
    quint32 value0;
    
    /** The number of objects in this array */
    quint32 nvalues;
};

}

}

SIRE_END_HEADER

#endif
