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

#include "packedarray.h"

using namespace SireBase;
using namespace SireBase::detail;

////////
//////// Implementation of ArrayMemoryBase
////////

/** Return the number of bytes necessary to store the array of arrays */
quint32 ArrayMemoryBase::getSize(quint32 narrays, quint32 nvalues, 
                                 quint32 sizeof_PackedArrayData,
                                 quint32 sizeof_PackedArray_Array,
                                 quint32 sizeof_PackedArray_ArrayData,
                                 quint32 sizeof_T)
{
    //need to add space for null arrays
    if (narrays == 0)
        narrays = 1;

    return sizeof_PackedArrayData +
           narrays * sizeof_PackedArray_Array +
           narrays * sizeof_PackedArray_ArrayData +
           nvalues * sizeof_T;
}

/** Allocate the memory necessary to hold the array of arrays */
char* ArrayMemoryBase::create(quint32 narrays, quint32 nvalues, 
                              quint32 sizeof_PackedArrayData,
                              quint32 sizeof_PackedArray_Array,
                              quint32 sizeof_PackedArray_ArrayData,
                              quint32 sizeof_T)
{
    quint32 sz = ArrayMemoryBase::getSize(narrays, nvalues,
                                          sizeof_PackedArrayData,
                                          sizeof_PackedArray_Array,
                                          sizeof_PackedArray_ArrayData,
                                          sizeof_T);
                                          
    return new char[sz];
}

/////////
///////// Implementation of PackedArrayDataBase
/////////

/** Null constructor */
PackedArrayDataBase::PackedArrayDataBase()
                    : QSharedData(), array0(0), arraydata0(0),
                      narrays(0), value0(0), nvalues(0)
{}

/** Construct to hold the specified number of arrays and values */
PackedArrayDataBase::PackedArrayDataBase(quint32 na, quint32 nv)
                    : QSharedData(), array0(0), arraydata0(0),
                      narrays(na), value0(0), nvalues(nv)
{}

/** Copy constructor */
PackedArrayDataBase::PackedArrayDataBase(const PackedArrayDataBase &other)
                    : QSharedData(), array0(other.array0),
                      arraydata0(other.arraydata0), narrays(other.narrays),
                      value0(other.value0), nvalues(other.nvalues)
{}

/** Destructor */
PackedArrayDataBase::~PackedArrayDataBase()
{}

/** Increse the reference count of this object */
void PackedArrayDataBase::incref()
{
    this->ref.ref();
}

/** Return the number of arrays in this container */
quint32 PackedArrayDataBase::nArrays() const
{
    return narrays;
}

/** Return the total number of objects in this container */
quint32 PackedArrayDataBase::nValues() const
{
    return nvalues;
}

/** Return a raw pointer to the memory used to store this container */
char* PackedArrayDataBase::memory()
{
    return ArrayMemoryBase::getRoot( (char*)this, 0 );
}

/** Return a raw pointer to the memory used to store this container */
const char* PackedArrayDataBase::memory() const
{
    return ArrayMemoryBase::getRoot( (const char*)this, 0 );
}
