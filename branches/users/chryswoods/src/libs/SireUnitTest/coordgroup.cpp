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

#include "coordgroup.h"

#include <boost/assert.hpp>

#include <QDebug>

#include "SireError/errors.h"

using namespace SireVol;
using namespace SireVol::detail;

//////////////
////////////// Implementation of CoordGroupData
//////////////

/** Null constructor */
CoordGroupData::CoordGroupData()
               : QSharedData(),
                 sz(0), needsupdate(false)
{}

/** Construct a group that holds 'size' coordinates */
CoordGroupData::CoordGroupData(quint32 size)
               : QSharedData(),
                 sz(size), needsupdate(true)
{}

/** Copy constructor */
CoordGroupData::CoordGroupData(const CoordGroupData &other)
               : QSharedData(),
                 aabox(other.aabox), sz(other.sz), 
                 needsupdate(other.needsupdate)
{}

/** Destructor */
CoordGroupData::~CoordGroupData()
{}

/** Copy assignment operator */
CoordGroupData& CoordGroupData::operator=(const CoordGroupData &other)
{
    if (this != &other)
    {
        aabox = other.aabox;
        sz = other.sz;
        needsupdate = other.needsupdate;
    }
    
    return *this;
}

/** Comparison operator */
bool CoordGroupData::operator==(const CoordGroupData &other) const
{
    return (this == &other) or
           (sz == other.sz and needsupdate == other.needsupdate and
            aabox == other.aabox);
}

/** Comparison operator */
bool CoordGroupData::operator!=(const CoordGroupData &other) const
{
    return (this != &other) and
           (sz != other.sz or needsupdate != other.needsupdate or
            aabox != other.aabox);
}

CoordGroupData* CoordGroupData::shared_null(0);

/** Return the global null object - this increments it's
    reference count, so make sure that you decrement
    the count if you stop using the returned pointer */
CoordGroupData* CoordGroupData::getSharedNull()
{
    if (!shared_null)
    {
        shared_null = new CoordGroupData(0);
        shared_null->ref.ref();
    }
        
    //increment the reference count
    shared_null->ref.ref();
        
    return shared_null;
}

//////////////
////////////// Implementation of CoordGroupBase
//////////////

/** Completely destroy the group at 'storage' */
void CoordGroupBase::destroy(CoordGroupData *group_ptr)
{
    char *storage = (char*)group_ptr;
    
    int sz = group_ptr->count();
    
    Vector *array = (Vector*)( storage + sizeof(CoordGroupData) );
    
    //destroy all elements in the array (in reverse order
    //to preserve the C++ canonical order)
    for (int i=sz-1; i>=0; --i)
    {
        array[i].~Vector();
    }
    
    //destroy the CoordGroupData
    group_ptr->~CoordGroupData();
    
    //finally, delete the memory
    delete[] storage;
}

/** Detach this object from shared storage */
void CoordGroupBase::detach()
{
    CoordGroupData *group_ptr = (CoordGroupData*)(storage);

    if (group_ptr->ref != 1)
    {
        //we need to clone the memory - get the size of the 
        //memory to be cloned
        int mem_size = sizeof(CoordGroupData) + 
                          group_ptr->count() * sizeof(Vector);
                          
        //create a new array of this size
        char *new_storage = new char[mem_size];
        
        //copy the data...
        void *output = qMemCopy( new_storage, storage, mem_size * sizeof(char) );
        
        BOOST_ASSERT( output == new_storage );
        
        //copy the new storage to the old pointer - the first
        //part of this memory is the CoordGroupData object...
        CoordGroupData *new_group_ptr = (CoordGroupData*)(new_storage);
        new_group_ptr->ref.ref();
        
        CoordGroupData *old_group_ptr = (CoordGroupData*)(storage);
        
        new_group_ptr = qAtomicSetPtr(&old_group_ptr, new_group_ptr);
        
        storage = (char*)(old_group_ptr);
        
        if (!new_group_ptr->ref.deref())
            //we need to delete the original...
            destroy(new_group_ptr);
    }
}

/** Return a reference to the CoordGroupData that contains the 
    metadata about this group */
const CoordGroupData& CoordGroupBase::_pvt_group() const
{
    return *( (CoordGroupData*)(memory()) );
}

/** Return a modifiable reference to the CoordGroupData that contains the 
    metadata about this group */
CoordGroupData& CoordGroupBase::_pvt_group()
{
    return *( (CoordGroupData*)(memory()) );
}

/** Return a modifiable reference to the CoordGroupData that contains the 
    metadata about this group */
const CoordGroupData& CoordGroupBase::_pvt_constGroup() const
{
    return _pvt_group();
}

/** Copy assignment operator - CoordGroupBase is implicitly
    shared so this will be fast */
CoordGroupBase& CoordGroupBase::operator=(const CoordGroupBase &other)
{
    if (storage != other.storage)
    {
        CoordGroupData *new_ptr = &( const_cast<CoordGroupData&>(other._pvt_group()) );
   
        new_ptr->ref.ref();
          
        CoordGroupData *old_group_ptr = (CoordGroupData*)(storage);
        
        new_ptr = qAtomicSetPtr(&old_group_ptr, new_ptr);
        
        storage = (char*)(old_group_ptr);
    
        if (!new_ptr->ref.deref())
            destroy(new_ptr);
    }
    
    return *this;
}

/** Return whether this group is empty (has no coordinates) */
bool CoordGroupBase::isEmpty() const
{
    return _pvt_group().isEmpty();
}

/** Return a raw pointer to the array of coordinates */
const Vector* CoordGroupBase::_pvt_data() const
{
    if (not isEmpty())
        return (Vector*)( memory() + sizeof(CoordGroupData) );
    else
        return 0;
}

/** Return a modifiable raw pointer to the array of 
    coordinates */
Vector* CoordGroupBase::_pvt_data()
{
    if (not isEmpty())
        return (Vector*)( memory() + sizeof(CoordGroupData) );
    else
        return 0;
}

/** Return a raw pointer to the array of coordinates */
const Vector* CoordGroupBase::_pvt_constData() const
{
    return _pvt_data();
}

/** Null constructor */
CoordGroupBase::CoordGroupBase() 
               : storage( (char*)(CoordGroupData::getSharedNull()) )
{}

/** Construct a CoordGroup that can hold up to 'size'
    coordinates */
CoordGroupBase::CoordGroupBase(quint32 size, const Vector &value)
{
    //construct the storage
    storage = new char[ sizeof(CoordGroupData) + size*sizeof(Vector) ];
    
    //construct the CoordGroupData in this piece of memory
    CoordGroupData *group_ptr = new (storage) CoordGroupData(size);
    
    //add a reference count for this object
    group_ptr->ref.ref();
    
    if (size > 0)
    {
        //initialise all of the coordinates to zero
        Vector *array = (Vector*)(storage + sizeof(CoordGroupData));
        
        for (quint32 i=0; i<size; ++i)
        {
            //construct the coordinate at this position in the array
            new ( &(array[i]) ) Vector(value);
        }
        
        //update the group with the correct bounding AABox
        group_ptr->update( AABox(value) );
    }
    else
        //update the group with the null bounding box
        group_ptr->update( AABox() );
}

/** Construct a CoordGroup from the specified coordinates */
CoordGroupBase::CoordGroupBase(const QVector<Vector> &coordinates)
{
    //how many coordinates are there?
    quint32 ncoords = coordinates.count();

    //construct the storage
    storage = new char[ sizeof(CoordGroupData) + ncoords*sizeof(Vector) ];
    
    //construct the CoordGroupData in this piece of memory
    CoordGroupData *group_ptr = new (storage) CoordGroupData(ncoords);
    
    //add a reference count for this object
    group_ptr->ref.ref();
    
    if (ncoords > 0)
    {
        //get the pointer to the first element in the array that
        //will house the coordinates
        Vector *array = (Vector*)(storage + sizeof(CoordGroupData));
        
        void *output = qMemCopy( array, coordinates.constData(), 
                                 ncoords*sizeof(Vector) );
                                 
        BOOST_ASSERT( output == array );
        
        //update the group with the bounding box that contains
        //these coordinates
        group_ptr->update( AABox(coordinates) );
    }
    else
        //update with the null AABox
        group_ptr->update( AABox() );
}

/** Copy constructor - this is fast as CoordGroupBase is implicitly
    shared */
CoordGroupBase::CoordGroupBase(const CoordGroupBase &other)
               : storage(other.storage)
{
    //increment the reference count of the data
    ((CoordGroupData*)storage)->ref.ref();
}

/** Destructor - this will only delete the data if this
    is the only reference to it */
CoordGroupBase::~CoordGroupBase()
{
    CoordGroupData *group_ptr = (CoordGroupData*)storage;
    
    if (!group_ptr->ref.deref())
        destroy(group_ptr);
}

/** Comparison operator */
bool CoordGroupBase::operator==(const CoordGroupBase &other) const
{
    if ( (this == &other) or
         (this->storage == other.storage) )
    {
        return true;
    }
    else if ( _pvt_group() == other._pvt_group())
    {
        //compare the coordinates
        const Vector *my_coords = constData();
        const Vector *other_coords = other.constData();
        
        int sz = size();
        for (int i=0; i<sz; ++i)
        {
            if (my_coords[i] != other_coords[i])
                return false;
        }
        
        return true;
    }
    else
        return false;
}

/** Comparison operator */
bool CoordGroupBase::operator!=(const CoordGroupBase &other) const
{
    return not this->operator==(other);
}

/** Return whether 'other' may be different to this group - this uses
    a simple comparison of the memory addresses of the storage of
    these two groups to see if they are definitely the same, or maybe
    different */
bool CoordGroupBase::maybeDifferent(const CoordGroupBase &other) const
{
    return storage != other.storage;
}
    
/** Assert that 'i' is a valid index in this group

    \throw SireError::invalid_index
*/
void CoordGroupBase::assertValidIndex(quint32 i) const
{
    if (i >= this->size())
        throw SireError::invalid_index( QObject::tr(
            "Invalid index: %1 - maximum index == %2.")
                .arg(i).arg(this->size()), CODELOC );
}

/** Return the 'ith' coordinate of this group

    \throw SireError::invalid_index
*/
const Vector& CoordGroupBase::at(quint32 i) const
{
    assertValidIndex(i);
    return _pvt_data()[i];
}

/** Return the 'ith' coordinate of this group

    \throw SireError::invalid_index
*/
const Vector& CoordGroupBase::operator[](quint32 i) const
{
    assertValidIndex(i);
    return _pvt_data()[i];
}

/** Return the enclosing AABox */
const AABox& CoordGroupBase::aaBox() const
{
    return _pvt_group().aaBox();
}

/** Return a raw pointer to the array containing all
    of the coordinates */
const Vector* CoordGroupBase::constData() const
{
    return _pvt_data();
}

/** Return a raw pointer to the array containing all
    of the coordinates */
const Vector* CoordGroupBase::data() const
{
    return _pvt_data();
}

/** Return the number of coordinates in this group */
quint32 CoordGroupBase::count() const
{
    return _pvt_group().count(); 
}

/** Return the number of coordinates in this group */
quint32 CoordGroupBase::size() const
{
    return _pvt_group().count();
}
