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

template<class T>
class PackedArrayData;

template<class T>
class PackedArray_ArrayData;

template<class T>
class PackedArray_Array;

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

    static SharedArrayPtr< PackedArrayData<T> > shared_null;
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
    const PackedArray_Array<T>* arrayData() const;

    const T* valueData() const;

    PackedArray_ArrayData* arrayDataData();
    PackedArray_Array<T>* arrayData();

    T* valueData();
    
    void setNValuesInArray(quint32 i, quint32 nvalues);
    
    void close();
};

/** This class holds the metadata about an individual array
    in the packed collection of arrays */
template<class T>
class PackedArray_ArrayData : public PackedArray_ArrayDataBase
{

friend class PackedArrayData<T>;
friend class ArrayMemory<T>;

public:
    PackedArray_ArrayData();
    PackedArray_ArrayData(quint32 this_idx);
    
    PackedArray_ArrayData(const PackedArray_ArrayData &other);
    
    ~PackedArray_ArrayData();
    
    void incref();
    void decref();
    
    PackedArray_ArrayData* detach();
    
    PackedArrayData* extract() const;
    
    const T* valueData() const;
    T* valueData();
};

/** This is the implicitly shared array class that is packed
    together to make a PackedArray<T>. This class is mainly used
    via its alias - PackedArray<T>::Array. Note that the size
    of a PackedArray<T>::Array is fixed at construction. You
    cannot change the size as this array is packed contiguously
    in memory with other arrays.
    
    @author Christopher Woods
*/
template<class T>
class PackedArray_Array
{
public:
    PackedArray_Array();
    PackedArray_Array(quint32 sz);
    PackedArray_Array(quint32 sz, const T &value);
    PackedArray_Array(const QVector<T> &values);
    
    PackedArray_Array(const PackedArray_Array<T> &other);
    
    ~PackedArray_Array();
    
    PackedArray_Array<T>& operator=(const PackedArray_Array<T> &other);
    
    bool operator==(const PackedArray_Array<T> &other) const;
    bool operator!=(const PackedArray_Array<T> &other) const;

    const T& operator[](quint32 i) const;
    T& operator[](quint32 i);
    
    const T& at(quint32 i) const;

    int count() const;
    int size() const;
    
    int nValues() const;
    
    bool isEmpty() const;
    
    const T* data() const;
    T* data();
    
    const T* constData() const;
    
    void update(const PackedArray_Array<T> &other);
    
    void assertValidIndex(quint32 i) const;

protected:
    PackedArray_Array(detail::PackedArray_ArrayData<T> *data);
    
private:
    /** Implicitly shared pointer to the array */
    detail::SharedArrayPtr< detail::PackedArray_ArrayData<T> > d;
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
    PackedArray(const QVector<T> &values);
    PackedArray(const QVector< QVector<T> > &values);
    PackedArray(const QVector<Array> &arrays);
    
    PackedArray(const PackedArray<T> &other);
    
    ~PackedArray();
    
    PackedArray<T>& operator=(const PackedArray<T> &other);
    
    bool operator==(const PackedArray<T> &other) const;
    bool operator!=(const PackedArray<T> &other) const;
    
    const Array& operator[](quint32 i);  // only const to prevent accidental assignment
    
    const T& operator()(quint32 i, quint32 j) const;
    T& operator()(quint32 i, quint32 j);
    
    const Array& at(quint32 i) const;
    const T& at(quint32 i, quint32 j) const;
    
    int count() const;
    int size() const;
    
    int nArrays() const;
    int nValues() const;

    bool isEmpty() const;

    const Array* data() const;
    const Array* constData() const;
    
    const T* data(quint32 i) const;
    T* data(quint32 i);
    
    const T* constData(quint32 i) const;
    
    const T* valueData() const;
    T* valueData();
    
    const T* constValueData() const;

    void update(quint32 i, const Array &array);

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
      B = PackedArray_Array<T> objects      narrays * sizeof(PackedArray_Array<T>)
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
                                            sizeof(PackedArray_Array<T>),
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
                    
        //this is the location of the first PackedArray_Array<T> :-)
        arraydata->array0 = idx;
        
        //the first PackedArray_ArrayData lies at this 
        //index + narrays*sizeof(PackedArray_Array<T>)
        arraydata->arraydata0 = idx + (narrays * sizeof(PackedArray_Array<T>));

        if (narrays > 0)
        {
            quint32 dataidx = arraydata->arraydata0;
        
            //loop over each array and create it in its place
            for (quint32 i=0; i<narrays; ++i)
            {   
                //assert that there is sufficient space in the array
                BOOST_ASSERT(idx + sizeof(PackedArray_Array<T>) < sz);
                BOOST_ASSERT(dataidx + sizeof(PackedArray_ArrayData<T>) < sz);
            
                //create the ArrayData, letting it know where it
                //is relative to the beginning of the storage array
                PackedArray_ArrayData<T> *data 
                        = new (storage + dataidx) PackedArray_ArrayData<T>(dataidx);
                
                //now create the PackedArray_Array<T> that uses this data
                new (storage + idx) PackedArray_Array<T>(data);
        
                //advance the index into the storage array to point
                //just after the just-created PackedArray_ArrayData
                idx += sizeof(PackedArray_Array<T>);
                dataidx += sizeof(PackedArray_ArrayData<T>);
            }
            
            //make idx point just after all of the PackedArray_ArrayDatas...
            BOOST_ASSERT( idx == arraydata->array0 
                                + narrays*sizeof(PackedArray_Array<T>) );
                                
            idx = dataidx;
        }
        else
        {
            //we need to create space for the null PackedArray_Array<T>
            quint32 data_idx = idx + sizeof(PackedArray_Array<T>);
            PackedArray_ArrayData<T> *array_arraydata 
                        = new (storage + data_idx) PackedArray_ArrayData<T>(data_idx);
            
            new (storage + idx) PackedArray_Array<T>(arraydata);
            
            array_arraydata->nvalues = 0;
            array_arraydata->value0 = 0;
            
            idx += sizeof(PackedArray_Array<T>) 
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
        PackedArray_Array<T> *arrays = array->arrayData();
            
        for (qint32 i=narrays-1; i>=0; --i)
        {
            PackedArray_ArrayData *arraydata 
                   = const_cast<PackedArray_ArrayData*>( arrays[i].d.constData() );
            
            //delete the PackedArray_ArrayData
            data->~PackedArray_ArrayData();
            
            //remove the shared pointer
            arrays[i].d.weakRelease();
            
            //delete the PackedArray_Array<T>
            arrays[i].~PackedArray_Array<T>();
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
                         sizeof(PackedArray_Array<T>),
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
        
        //the final step is to update all of the PackedArray_Array<T>
        //pointers that exist in this array (otherwise they will all
        //point to the original!)
        
        if (new_arraydata->nArrays() > 0)
        {
            PackedArray_Array<T> *arrays = new_arraydata->arrayData();
            PackedArray_ArrayData *arrays_data = new_array->arrayDataData();
        
            for (quint32 i=0; i < new_arraydata->nArrays(); ++i)
            {
                arrays[i].d.weakRelease();
                arrays[i].d.weakAssign( &(arrays_data[i]) );
            }
        }
        else
        {
            //update the null PackedArray_Array<T>
            PackedArray_Array<T> *arrays = new_arraydata->arrayData();
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

/** Return a raw pointer to the array of PackedArray_Array<T> objects */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const PackedArray_Array<T>* PackedArrayData<T>::arrayData() const
{
    if (this->array0 == 0)
        return 0;
    else
        return (const PackedArray_Array<T>*)( this->memory() + this->array0 );
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

/** Return a raw pointer to the array of PackedArray_Array<T> objects */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
PackedArray_Array<T>* PackedArrayData<T>::arrayData()
{
    if (this->array0 == 0)
        return 0;
    else
        return (PackedArray_Array<T>*)( this->memory() + this->array0 );
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

/** Set the number of values in the ith array - can only be called
    before the close() (so don't call it after closing the arrays) */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void PackedArrayData<T>::setNValuesInArray(quint32 i, quint32 nvalues)
{
    BOOST_ASSERT( i < narrays );
    this->arrayDataData()[i].nvalues = nvalues;
}

/** Close out this array - this fixes the number of values
    in each array in this array of arrays */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void PackedArrayData<T>::close()
{
    PackedArray_ArrayData<T> *arraydata = this->arrayDataData();
    
    //ensure that everything adds up
    quint32 n_assigned_vals = 0;
    
    //close out each array
    for (quint32 i=0; i<this->narrays; ++i)
    {
        PackedArray_ArrayData<T> &array = arraydata[i];
        
        //tell the array where its first value is located
        if (array.nValues() > 0)
            array.value0 = this->value0 + n_assigned_vals * sizeof(T);
        else
            array.value0 = 0;

        n_assigned_vals += array.nValues();
    }

    BOOST_ASSERT( n_assigned_vals == this->nvalues );
}

////////
//////// Implementation of PackedArray_ArrayData
////////

/** Null constructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
PackedArray_ArrayData<T>::PackedArray_ArrayData()
                         : PackedArray_ArrayDataBase()
{}

/** Construct the data, telling it that it is at 
    index 'this_idx' in the memory storage array */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
PackedArray_ArrayData<T>::PackedArray_ArrayData(quint32 this_idx)
                         : PackedArray_ArrayDataBase(this_idx)
{}

/** Copy constructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
PackedArray_ArrayData<T>::PackedArray_ArrayData(const PackedArray_ArrayData<T> &other)
                         : PackedArray_ArrayDataBase(other)
{}

/** Destructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
PackedArray_ArrayData<T>::~PackedArray_ArrayData()
{}

/** Return a pointer to a PackedArray_ArrayData that has been detached from 
    shared storage (could be a pointer to this PackedArray_ArrayData) */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
PackedArray_ArrayData<T>* PackedArray_ArrayData<T>::detach()
{
    return (PackedArray_ArrayData<T>*)
           ( ArrayMemory<T>::detach( (char*)this, this_array ) );
}

/** Return a pointer to a PackedArrayData object that contains 
    just the data of this PackedArray_Array */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
PackedArrayData<T>* PackedArray_ArrayData<T>::extract() const
{
    const PackedArrayData<T> *array = (const PackedArrayData<T>*)(this->memory());
    
    if (array->nArrays() <= 1)
        //this is already extracted!
        return const_cast<PackedArrayData<T>*>(array);
    
    char *new_storage = ArrayMemory<T>::create(1, this->nValues());
    
    try
    {
                
    //ok, we need to extract!
    PackedArrayData<T> *new_array = (PackedArrayData<T>*)(new_storage);

    new_array->setNValuesInArray(1, this->nValues());
    new_array->close();
    
    //copy the objects
    void *output = qMemCopy( new_array->valueData(),
                             this->valueData(),
                             this->nValues() * sizeof(T) );
                       
    BOOST_ASSERT( output == new_array->valueData() );

    return new_array;

    }
    catch(...)
    {
        delete[] new_storage;
    }
    
    return 0;
}

/** Increment the reference count for this object */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void PackedArray_ArrayData<T>::incref()
{
    ArrayMemory<T>::incref( (char*)this, this_array );
}

/** Decrement the reference count for this object - this may delete it! */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void PackedArray_ArrayData<T>::decref()
{
    ArrayMemory<T>::decref( (char*)this, this_array );
}

/** Return a pointer to the first object in this array */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const T* PackedArray_ArrayData<T>::valueData() const
{
    if (value0 == 0)
        return 0;
    else
        return (const T*)( this->memory() + value0 );
}

/** Return a pointer to the first object in this array */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
T* PackedArray_ArrayData<T>::valueData()
{
    if (value0 == 0)
        return 0;
    else
        return (T*)( this->memory() + value0 );
}

/** Return the number of objects in this array */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
quint32 CGData::nValues() const
{
    return this->nvalues;
}

///////////
/////////// Implementation of PackedArray_Array
///////////

template<class T>
static const SharedArrayPtr< PackedArrayData<T> >& getSharedNull()
{
    if (detail::ArrayMemory<T>::shared_null.constData() == 0)
    {
        detail::ArrayMemory<T>::shared_null 
                = (PackedArrayData<T>*)( ArrayMemory<T>::create(0,0,0) );
                
        detail::ArrayMemory<T>::shared_null->close();
    }
    
    return detail::ArrayMemory<T>::shared_null;
}

template<class T>
static SharedArrayPtr< PackedArray_ArrayData<T> > createNullArray()
{
    const SharedArrayPtr< PackedArrayData<T> > &array 
                            = SireBase::detail::getSharedNull<T>();
    
    return SharedArrayPtr< PackedArray_ArrayData<T> >( array->nullArray() );
}

/** Null constructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
PackedArray_Array<T>::PackedArray_Array()
                     : d( SireBase::detail::createNullArray<T>() )
{}

template<class T>
static SharedArrayPtr< PackedArray_ArrayData<T> > createArray(quint32 sz)
{
    if (sz == 0)
        return SireBase::detail::createNullArray<T>();
        
    //construct space for 1 array of sz objects
    char *storage = ArrayMemory<T>::create(1, sz);
        
    PackedArrayData<T> *array = (PackedArrayData<T>*)storage;
        
    array->setNValuesInArray(0, sz);
    array->close();
    
    return SharedArrayPtr< PackedArray_ArrayData<T> >( &(array->arrayDataData()[0]) );
}

/** Construct an array of size 'sz'. All of the objects will be initialised  
    with their default values */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
PackedArray_Array<T>::PackedArray_Array(quint32 sz)
                     : d( SireBase::detail::createArray<T>(sz) )
{}

/** Construct an array of size 'sz' where all of the objects have value 'value'. */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
PackedArray_Array<T>::PackedArray_Array(quint32 sz, const T &value)
                     : d( SireBase::detail::createArray<T>(sz) )
{
    if (sz == 0)
        return;

    T *data = d.valueData();

    //initialise all of the values
    for (quint32 i=0; i<sz; ++i)
    {
        data[i] = value;
    }
}

/** Construct from the passed array of values */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
PackedArray_Array<T>::PackedArray_Array(const QVector<T> &values)
{
    PackedArray<T> array(values);
    this->operator=( array.constData()[0] );
}

/** Private constructor used internally */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
PackedArray_Array<T>::PackedArray_Array(detail::PackedArray_ArrayData<T> *data)
{
    d.weakAssign(data);
}

/** Copy constructor - fast as this class is implicitly shared */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
PackedArray_Array<T>::PackedArray_Array(const PackedArray_Array<T> &other)
                     : d(other.d)
{}

/** Destructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
PackedArray_Array<T>::~PackedArray_Array()
{}

/** Copy assignment operator - fast as this class is implicitly shared.
    However, as this copies a view of the whole packed array, you cannot
    use this to assign the contents of this array within its current 
    PackedArray - to do this, you must use the 'update()' function. */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
PackedArray_Array<T>& PackedArray_Array<T>::operator=(
                                        const PackedArray_Array<T> &other)
{
    this->d = other.d;
    return *this;
}

/** Comparison operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool PackedArray_Array<T>::operator==(const PackedArray_Array<T> &other) const
{
    if (this->d.constData() == other.d.constData())
        return true;

    if (this->nvalues != other.nvalues)
        return false;
        
    const T *this_data = this->constData();
    const T *other_data = other.constData();
    
    for (quint32 i=0; i<this->nvalues; ++i)
    {
        if (this_data[i] != other_data[i])
            return false;
    }
    
    return true;
}

/** Comparison operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool PackedArray_Array<T>::operator!=(const PackedArray_Array<T> &other) const
{
    return not this->operator==(other);
}

/** Assert that the index 'i' is valid for this array 

    \throw SireError::invalid_index
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void PackedArray_Array<T>::assertValidIndex(quint32 i) const
{
    if (i >= this->nvalues)
        detail::throwPackedArray_Array_invalidIndex(i, this->nvalues);
}

/** Return a raw pointer to the array */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const T* PackedArray_Array<T>::data() const
{
    return this->d->valueData();
}

/** Return a modifiable raw pointer to the array
     *DO NOT* delete this pointer! */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
T* PackedArray_Array<T>::data()
{
    return this->d->valueData();
}

/** Return a raw pointer to the array */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const T* PackedArray_Array<T>::constData() const
{
    return this->d->constValueData();
}

/** Return a reference to the ith object in this array */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const T& PackedArray_Array<T>::operator[](quint32 i) const
{
    this->assertValidIndex(i);
    return this->data()[i];
}

/** Return a reference to the ith object in this array */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
T& PackedArray_Array<T>::operator[](quint32 i)
{
    this->assertValidIndex(i);
    return this->data()[i];
}

/** Return a reference to the ith object in this array */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const T& PackedArray_Array<T>::at(quint32 i) const
{
    this->assertValidIndex(i);
    return this->data()[i];
}

/** Return the number of objects in this array */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
int PackedArray_Array<T>::count() const
{
    return d->nvalues;
}

/** Return the number of objects in this array */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
int PackedArray_Array<T>::size() const
{
    return this->d->nValues();
}

/** Return the number of objects in this array */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
int PackedArray_Array<T>::nValues() const
{
    return this->d->nValues();
}

/** Return whether or not this array is empty */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool PackedArray_Array<T>::isEmpty() const
{
    return this->count() == 0;
}

/** Update this array so that it has the same contents as 'other'

    \throw SireError::incompatible_error
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void PackedArray_Array<T>::update(const PackedArray_Array<T> &other)
{
    if (this->size() != other.size())
        detail::throwPackedArray_Array_incompatibleError(this->size(),other.size());
        
    if (this->size() == 0)
        return;
        
    void *output = qMemCopy(this->data(), other.data(), 
                            this->size() * sizeof(T));
                            
    BOOST_ASSERT( output == this->data() );
}

} // end of namespace detail

/////////
///////// Implementation of PackedArray
/////////

template<class T>
SIRE_OUTOFLINE_TEMPLATE
PackedArray<T>::PackedArray();

template<class T>
SIRE_OUTOFLINE_TEMPLATE
PackedArray<T>::PackedArray(const QVector<T> &values);

template<class T>
SIRE_OUTOFLINE_TEMPLATE
PackedArray<T>::PackedArray(const QVector< QVector<T> > &values);

template<class T>
SIRE_OUTOFLINE_TEMPLATE
PackedArray<T>::PackedArray(const QVector<Array> &arrays);

template<class T>
SIRE_OUTOFLINE_TEMPLATE
PackedArray<T>::PackedArray(const PackedArray<T> &other);

template<class T>
SIRE_OUTOFLINE_TEMPLATE
PackedArray<T>::~PackedArray();

template<class T>
SIRE_OUTOFLINE_TEMPLATE
PackedArray<T>& PackedArray<T>::operator=(const PackedArray<T> &other);

template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool PackedArray<T>::operator==(const PackedArray<T> &other) const;

template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool PackedArray<T>::operator!=(const PackedArray<T> &other) const;

template<class T>
SIRE_OUTOFLINE_TEMPLATE
const Array& PackedArray<T>::operator[](quint32 i);

template<class T>
SIRE_OUTOFLINE_TEMPLATE
const T& PackedArray<T>::operator()(quint32 i, quint32 j) const;

template<class T>
SIRE_OUTOFLINE_TEMPLATE
T& PackedArray<T>::operator()(quint32 i, quint32 j);

template<class T>
SIRE_OUTOFLINE_TEMPLATE
const Array& PackedArray<T>::at(quint32 i) const;

template<class T>
SIRE_OUTOFLINE_TEMPLATE
const T& PackedArray<T>::at(quint32 i, quint32 j) const;

template<class T>
SIRE_OUTOFLINE_TEMPLATE
int PackedArray<T>::count() const;

template<class T>
SIRE_OUTOFLINE_TEMPLATE
int PackedArray<T>::size() const;

template<class T>
SIRE_OUTOFLINE_TEMPLATE
int PackedArray<T>::nArrays() const;

template<class T>
SIRE_OUTOFLINE_TEMPLATE
int PackedArray<T>::nValues() const;

template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool PackedArray<T>::isEmpty() const;

template<class T>
SIRE_OUTOFLINE_TEMPLATE
const Array* PackedArray<T>::data() const;

template<class T>
SIRE_OUTOFLINE_TEMPLATE
const Array* PackedArray<T>::constData() const;

template<class T>
SIRE_OUTOFLINE_TEMPLATE
const T* PackedArray<T>::data(quint32 i) const;

template<class T>
SIRE_OUTOFLINE_TEMPLATE
T* PackedArray<T>::data(quint32 i);

template<class T>
SIRE_OUTOFLINE_TEMPLATE
const T* PackedArray<T>::constData(quint32 i) const;

template<class T>
SIRE_OUTOFLINE_TEMPLATE
const T* PackedArray<T>::valueData() const;

template<class T>
SIRE_OUTOFLINE_TEMPLATE
T* PackedArray<T>::valueData();

template<class T>
SIRE_OUTOFLINE_TEMPLATE
const T* PackedArray<T>::constValueData() const;

template<class T>
SIRE_OUTOFLINE_TEMPLATE
void PackedArray<T>::update(quint32 i, const Array &array);

} // end of namespace SireBase

SIRE_END_HEADER

#endif
