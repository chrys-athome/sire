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

/** This is the implicitly shared pointer class that 
    is used to hold any of the ArrayMemory allocated objects
    
    @author Christopher Woods
*/
template<class T>
class SharedArrayPtr
{

public:
    SharedArrayPtr() : ptr(0)
    {}

    SharedArrayPtr(const T *p)
    {
        ptr = const_cast<T*>(p);
    
        if (ptr)
            ptr->incref();
    }
    
    SharedArrayPtr(const SharedArrayPtr &other) : ptr(other.ptr)
    {
        if (ptr)
            ptr->incref();
    }
    
    ~SharedArrayPtr()
    {
        if (ptr)
            ptr->decref();
    }
    
    SharedArrayPtr<T>& operator=(const SharedArrayPtr &other)
    {
        if (ptr != other.ptr)
        {
            T *new_ptr = other.ptr;
            
            //increment the other reference count
            if (new_ptr)
                new_ptr->incref();
                
            //decrement our reference count
            if (ptr)
                ptr->decref();
                
            //set the new pointer
            ptr = new_ptr;
        }
        
        return *this;
    }
    
    const T& operator*() const
    {
        return *ptr;
    }
    
    const T* operator->() const
    {
        return ptr;
    }
    
    T& operator*()
    {
        if (ptr)
            ptr = ptr->detach();
            
        return *ptr;
    }
    
    T* operator->()
    {
        if (ptr)
            ptr = ptr->detach();
            
        return ptr;
    }
    
    const T* data() const
    {
        return ptr;
    }
    
    const T* constData() const
    {
        return ptr;
    }
    
    T* data()
    {
        if (ptr)
            ptr = ptr->detach();
        
        return ptr;
    }
    
    /** Assign this pointer to point at 'weakptr' 
        but *without* changing the reference count.
        You ABSOLUTELY MUST ensure that you call 
        SharedArrayPtr::weakRelease() before this 
        pointer is deleted or reassigned, so 
        as to not decrement the reference count incorrectly! */
    void weakAssign(T *weakptr)
    {
        if (ptr)
            ptr->decref();
            
        ptr = weakptr;
    }
    
    /** Release the pointer *without* decrementing the
        reference count. You should only call this
        function if the pointer was assigned using 
        the 'weakAssign()' function */
    void weakRelease()
    {
        ptr = 0;
    }
    
private:
    /** Actual pointer */
    T *ptr;
};

/** This class is used to hold all of the metadata about the 
    packed array of arrays
*/
template<class T>
class PackedArrayData : public PackedArrayDataBase
{
friend class ArrayMemory<T>;

public:
    PackedArrayData();
    PackedArrayData(quint32 narrays, quint32 nvalues);
    
    PackedArrayData(const PackedArrayData &other);
    
    ~PackedArrayData();

    void decref();
    
    PackedArrayData* detach();
    
    const PackedArray_ArrayData* nullArray() const;
    
    const PackedArray_ArrayData* arrayDataData() const;
    const PackedArray<T>::Array* arrayData() const;

    const T* valueData() const;

    PackedArray_ArrayData* arrayDataData();
    PackedArray<T>::Array* cgArrayData();

    T* valueData();
    
    void setNValuesInArray(quint32 i, quint32 nvalues);
    
    void close();
};

template<class T>
class PackedArray_Array
{
};

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

namespace SireBase
{

namespace detail
{

////////
//////// Implementation of ArrayMemory
////////

/** Create space for the arrays. The layout in memory is;

    --------------------------------------------------
    | A |   B   |    C    |            D             |
    --------------------------------------------------

      A = PackedArrayData<T> object          1 * sizeof(PackedArrayData<T>)
      B = PackedArray<T>::Array objects      narrays * sizeof(PackedArray<T>::Array)
      C = PackedArray_ArrayData<T> objects   narrays * sizeof( " _ArrayData<T>)
      D = Array of T objects                 nvalues * sizeof(T)
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
char* ArrayMemory<T>::create(quint32 narrays, quint32 nvalues)
{
    //create space for the data
    char *storage = ArrayMemoryBase::create(narrays, nvalues, 
                                            sizeof(PackedArrayData<T>),
                                            sizeof(PackedArray<T>::Array),
                                            sizeof(PackedArray_ArrayData<T>),
                                            sizeof(T));
    
    try
    {
        //now we need to construct each object in turn at its correct
        //location in this space
        PackedArrayData<T> *arraydata 
                    = new (storage) PackedArrayData<T>(narrays, nvalues);
                    
        //add a reference count for this object
        arraydata->ref.ref();

        //advance the index into the storage array so that we are now
        //pointing just after the PackedArrayData
        quint32 idx = sizeof(PackedArrayData<T>);
                    
        //this is the location of the first PackedArray<T>::Array :-)
        arraydata->array0 = idx;
        
        //the first PackedArray_ArrayData lies at this 
        //index + narrays*sizeof(PackedArray<T>::Array)
        arraydata->arraydata0 = idx + (narrays * sizeof(PackedArray<T>::Array));

        if (narrays > 0)
        {
            quint32 dataidx = arraydata->arraydata0;
        
            //loop over each array and create it in its place
            for (quint32 i=0; i<narrays; ++i)
            {   
                //assert that there is sufficient space in the array
                BOOST_ASSERT(idx + sizeof(PackedArray<T>::Array) < sz);
                BOOST_ASSERT(dataidx + sizeof(PackedArray_ArrayData<T>) < sz);
            
                //create the ArrayData, letting it know where it
                //is relative to the beginning of the storage array
                PackedArray_ArrayData<T> *data 
                        = new (storage + dataidx) PackedArray_ArrayData<T>(dataidx);
                
                //now create the PackedArray<T>::Array that uses this data
                new (storage + idx) PackedArray<T>::Array(data);
        
                //advance the index into the storage array to point
                //just after the just-created PackedArray_ArrayData
                idx += sizeof(PackedArray<T>::Array);
                dataidx += sizeof(PackedArray_ArrayData<T>);
            }
            
            //make idx point just after all of the PackedArray_ArrayDatas...
            BOOST_ASSERT( idx == arraydata->array0 
                                + narrays*sizeof(PackedArray<T>::Array) );
                                
            idx = dataidx;
        }
        else
        {
            //we need to create space for the null PackedArray<T>::Array
            quint32 data_idx = idx + sizeof(PackedArray<T>::Array);
            PackedArray_ArrayData<T> *array_arraydata 
                        = new (storage + data_idx) PackedArray_ArrayData<T>(data_idx);
            
            new (storage + idx) PackedArray<T>::Array(arraydata);
            
            array_arraydata->nvalues = 0;
            array_arraydata->value0 = 0;
            
            idx += sizeof(PackedArray<T>::Array) 
                    + sizeof(PackedArray_ArrayData);
        }
        
        //we are now at the location of the first item in the array
        arraydata->value0 = idx;
        
        //loop over each object and create it in place
        for (quint32 i=0; i<nvalues; ++i)
        {
            BOOST_ASSERT(idx + sizeof(T) <= sz);

            new (storage + idx) T();
                
            idx += sizeof(T);
        }
        
        //we should now be at the end of the storage
        BOOST_ASSERT( idx == sz );
                
        return storage;
    }
    catch(...)
    {
        delete[] storage;
        throw;
    }
}

/** Increase the reference count */
template<class T>
SIRE_INLINE_TEMPLATE
void ArrayMemory<T>::incref(char *this_ptr, quint32 this_idx)
{
    ( (PackedArrayData*)(ArrayMemoryBase::getRoot(this_ptr,this_idx)) )->incref();
}

/** Decrease the reference count */
template<class T>
SIRE_INLINE_TEMPLATE
void ArrayMemory<T>::decref(char *this_ptr, quint32 this_idx)
{
    ( (PackedArrayData*)(ArrayMemoryBase::getRoot(this_ptr,this_idx)) )->decref();
}

/** Destroy the object 'array' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void ArrayMemory<T>::destroy(PackedArrayData<T> *array)
{
    //we need to delete it in the opposite order to creation - first
    //lets delete all of the objects
    quint32 nvalues = array->nValues();

    char *storage = (char*)array;
        
    if (nvalues > 0)
    {
        T *values = array->valueData();
        
        for (qint32 i=nvalues-1; i>=0; --i)
        {
            values[i].~T();
        }
    }
        
    //now delete all of the arrays
    quint32 narrays = array->nArrays();
        
    if (narrays > 0)
    {
        PackedArray<T>::Array *arrays = array->arrayData();
            
        for (qint32 i=narrays-1; i>=0; --i)
        {
            PackedArray_ArrayData *arraydata 
                   = const_cast<PackedArray_ArrayData*>( arrays[i].d.constData() );
            
            //delete the PackedArray_ArrayData
            data->~PackedArray_ArrayData();
            
            //remove the shared pointer
            arrays[i].d.weakRelease();
            
            //delete the PackedArray<T>::Array
            arrays[i].~PackedArray<T>::Array();
        }
    }
        
    //delete the PackedArrayData object itself
    array->~PackedArrayData();
        
    //finally release the memory
    delete[] storage;
}

/** Detach the data pointed to by 'this_ptr' from shared storage */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
char* ArrayMemory<T>::detach(char *this_ptr, quint32 this_idx)
{
    //get a pointer to the start of the storage for this container
    char *storage = getRoot(this_ptr, this_idx);
    
    //The PackedArrayData object is at the beginning of this storage array
    PackedArrayData<T> *arraydata = (PackedArrayData<T>*) storage;
    
    if (arraydata->ref != 1)
    {
        //there is more than one reference to this data - it will have to 
        //be cloned - get the size of memory to be cloned
        int sz = getSize(arraydata->nArrays(), arraydata->nValues(),
                         sizeof(PackedArrayData<T>), 
                         sizeof(PackedArray<T>::Array),
                         sizeof(PackedArray_ArrayData<T>),
                         sizeof(T));
                         
        //create space for the copy
        char *new_storage = new char[sz];
        
        //copy the data
        void *output = qMemCopy( new_storage, storage, sz*sizeof(char) );
        BOOST_ASSERT(output == new_storage);

        //the first part of the data is the PackedArrayData object
        PackedArrayData *new_arraydata = (PackedArrayData*) new_storage;
        
        //set the reference count of this copy to 1
        new_arraydata->ref = 1;

        //now loose a reference to the original
        ArrayMemory<T>::decref(this_ptr, this_idx);
        
        //the final step is to update all of the PackedArray<T>::Array
        //pointers that exist in this array (otherwise they will all
        //point to the original!)
        
        if (new_arraydata->nArrays() > 0)
        {
            PackedArray<T>::Array *arrays = new_arraydata->arrayData();
            PackedArray_ArrayData *arrays_data = new_array->arrayDataData();
        
            for (quint32 i=0; i < new_arraydata->nArrays(); ++i)
            {
                arrays[i].d.weakRelease();
                arrays[i].d.weakAssign( &(arrays_data[i]) );
            }
        }
        else
        {
            //update the null PackedArray<T>::Array
            PackedArray<T>::Array *arrays = new_arraydata->arrayData();
            PackedArray_ArrayData *arrays_data = new_array->arrayDataData();

            arrays[0].d.weakRelease();
            arrays[0].d.weakAssign( &(arrays_data[0]) );
        }

        //return a pointer to the clone
        return new_storage + this_idx;
    }
    else
    {
        //only one reference, so no need to clone
        return this_ptr;
    }
}

///////
/////// Implementation of PackedArrayData<T>
///////

/** Null constructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
PackedArrayData<T>::PackedArrayData() : PackedArrayDataBase()
{}

/** Construct to hold the specified number of arrays and values */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
PackedArrayData<T>::PackedArrayData(quint32 narrays, quint32 nvalues)
                   : PackedArrayDataBase(narrays, nvalues)
{}

/** Copy constructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
PackedArrayData<T>::PackedArrayData(const PackedArrayData &other)
                   : PackedArrayDataBase(other)
{}

/** Destructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
PackedArrayData<T>::~PackedArrayData()
{}

/** Decrease the reference count of this object */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void PackedArrayData<T>::decref()
{
    if (not this->ref.deref())
        ArrayMemory<T>::destroy(this);
}

/** Return a pointer to a copy of this array that has been 
    detached from shared storage */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
PackedArrayData<T>* PackedArrayData<T>::detach()
{
    return (PackedArrayData<T>*)( ArrayMemory<T>::detach( (char*)(this), 0 ) );
}

/** Return a pointer to the null array */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const PackedArray_ArrayData* PackedArrayData<T>::nullArray() const
{
    BOOST_ASSERT( this->narrays == 0 );
    BOOST_ASSERT( this->arraydata0 != 0 );
    
    return (const PackedArray_ArrayData*)( this->memory()[this->arraydata0] );
}

/** Return a raw pointer to the array of PackedArray_ArrayData objects */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const PackedArray_ArrayData<T>* PackedArrayData<T>::arrayDataData() const
{
    if (this->arraydata0 == 0)
        return 0;
    else
        return (const PackedArray_ArrayData<T>*)( this->memory() + this->arraydata0 );
}

/** Return a raw pointer to the array of PackedArray<T>::Array objects */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const PackedArray<T>::Array* PackedArrayData<T>::arrayData() const
{
    if (this->array0 == 0)
        return 0;
    else
        return (const PackedArray<T>::Array*)( this->memory() + this->array0 );
}

/** Return a raw pointer to the array of objects in this array */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const T* PackedArrayData<T>::valueData() const
{
    if (this->value0 == 0)
        return 0;
    else
        return (const T*)( this->memory() + this->array0 );
}

/** Return a raw pointer to the array of PackedArray_ArrayData objects */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
PackedArray_ArrayData<T>* PackedArrayData<T>::arrayDataData()
{
    if (this->arraydata0 == 0)
        return 0;
    else
        return (PackedArray_ArrayData<T>*)( this->memory() + this->arraydata0 );
}

/** Return a raw pointer to the array of PackedArray<T>::Array objects */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
PackedArray<T>::Array* PackedArrayData<T>::arrayData()
{
    if (this->array0 == 0)
        return 0;
    else
        return (PackedArray<T>::Array*)( this->memory() + this->array0 );
}

/** Return a raw pointer to the array of objects in this array */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
T* PackedArrayData<T>::valueData()
{
    if (this->value0 == 0)
        return 0;
    else
        return (T*)( this->memory() + this->array0 );
}

template<class T>
SIRE_OUTOFLINE_TEMPLATE
void PackedArrayData<T>::setNValuesInArray(quint32 i, quint32 nvalues);

template<class T>
SIRE_OUTOFLINE_TEMPLATE
void PackedArrayData<T>::close();

}


SIRE_END_HEADER

#endif
