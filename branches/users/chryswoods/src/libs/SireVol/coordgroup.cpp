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

#include "SireMaths/rotate.h"
#include "SireMaths/quaternion.h"
#include "SireMaths/matrix.h"

#include "SireError/errors.h"

#include "SireStream/datastream.h"

using namespace SireVol;
using namespace SireVol::detail;

using namespace SireMaths;
using namespace SireStream;

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
    qDebug() << CODELOC;
    char *storage = (char*)group_ptr;
    
    qDebug() << CODELOC;
    int sz = group_ptr->count();
    
    qDebug() << CODELOC;
    Vector *array = (Vector*)( storage + sizeof(CoordGroupData) );
    
    //destroy all elements in the array (in reverse order
    //to preserve the C++ canonical order)
    qDebug() << CODELOC;
    for (int i=sz-1; i>=0; --i)
    {
        array[i].~Vector();
    }
    
    //destroy the CoordGroupData
    qDebug() << CODELOC;
    group_ptr->~CoordGroupData();
    
    //finally, delete the memory
    qDebug() << CODELOC;
    delete[] storage;
}

/** Detach this object from shared storage */
void CoordGroupBase::detach()
{
    qDebug() << CODELOC;
    CoordGroupData *group_ptr = (CoordGroupData*)(storage);

    if (group_ptr->ref != 1)
    {
    qDebug() << CODELOC;
        //we need to clone the memory - get the size of the 
        //memory to be cloned
        int mem_size = sizeof(CoordGroupData) + 
                          group_ptr->count() * sizeof(Vector);
                          
    qDebug() << CODELOC;
        //create a new array of this size
        char *new_storage = new char[mem_size];
        
    qDebug() << CODELOC;
        //copy the data...
        void *output = qMemCopy( new_storage, storage, mem_size * sizeof(char) );
        
    qDebug() << CODELOC;
        BOOST_ASSERT( output == new_storage );
        
    qDebug() << CODELOC;
        //copy the new storage to the old pointer - the first
        //part of this memory is the CoordGroupData object...
        CoordGroupData *new_group_ptr = (CoordGroupData*)(new_storage);
        new_group_ptr->ref.ref();
        
    qDebug() << CODELOC;
        CoordGroupData *old_group_ptr = (CoordGroupData*)(storage);
        
    qDebug() << CODELOC;
        new_group_ptr = qAtomicSetPtr(&old_group_ptr, new_group_ptr);
        
    qDebug() << CODELOC;
        storage = (char*)(old_group_ptr);
        
    qDebug() << CODELOC;
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
    qDebug() << CODELOC;
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
    qDebug() << CODELOC;
    
    if (not isEmpty())
        return (Vector*)( constMemory() + sizeof(CoordGroupData) );
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
    qDebug() << CODELOC;
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
    
static inline void assertSame(quint32 sz, quint32 other)
{
    if (sz != other)
        throw SireError::incompatible_error( QObject::tr(
            "The number of coordinates in this group is %1, while "
            "the number of coordinates in the other is %2!")
                .arg(sz).arg(other), CODELOC );
}
    
/** Assert that the array of coordinates contains the same number of
    coordinates as are in this group
    
    \throw SireError::incompatible_error
*/
void CoordGroupBase::assertSameSize(const QVector<Vector> &coordinates) const
{
    assertSame(this->count(), coordinates.count());
}
    
/** Assert that the CoordGroup 'other' contains the same number of
    coordinates as are in this group
    
    \throw SireError::incompatible_error
*/
void CoordGroupBase::assertSameSize(const CoordGroupBase &other) const
{
    assertSame(this->count(), other.count());
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
    
/** Return whether or not this group needs to be updated */
bool CoordGroupBase::needsUpdate() const
{
    return _pvt_group().needsUpdate();
}
    
/** Set that the AABox needs to be updated */
void CoordGroupBase::setNeedsUpdate()
{
    _pvt_group().setNeedsUpdate();
}
    
/** Update the AABox with the current coordinates */
void CoordGroupBase::update()
{
    qDebug() << CODELOC;
    _pvt_group().update( AABox(*this) );
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

//////////////
////////////// Implementation of CoordGroup
//////////////

static const RegisterMetaType<CoordGroup> r_cgroup;

/** Serialise to a binary data stream */
QDataStream SIREVOL_EXPORT &operator<<(QDataStream &ds, 
                                       const CoordGroup &cgroup)
{
    writeHeader(ds, r_cgroup, 1);
    
    quint32 sz = cgroup.count();
    const Vector *coords = cgroup.constData();
    
    ds << cgroup.aaBox() << sz;
    
    for (quint32 i=0; i<sz; ++i)
    {
        const Vector &coord = coords[i];
        ds << coord.x() << coord.y() << coord.z();
    }
    
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREVOL_EXPORT &operator>>(QDataStream &ds, 
                                       CoordGroup &cgroup)
{
    VersionID v = readHeader(ds, r_cgroup);
    
    if (v == 1)
    {
        quint32 sz;
        AABox box;
        
        ds >> box >> sz;
        
        CoordGroup newgroup(sz);
        Vector *coords = newgroup._pvt_data();
        
        for (quint32 i=0; i<sz; ++i)
        {
            double x, y, z;
            ds >> x >> y >> z;
            
            coords[i] = Vector(x,y,z);
        }
        
        newgroup._pvt_group().update(box);
        
        cgroup = newgroup;
    }
    else
        throw version_error(v, "1", r_cgroup, CODELOC);
    
    return ds;
}

/** Null constructor */
CoordGroup::CoordGroup() : CoordGroupBase()
{}

/** Construct a CoordGroup that holds 'size' coordinates */
CoordGroup::CoordGroup(quint32 size)
           : CoordGroupBase(size)
{}

/** Construct a CoordGroup that holds 'size' coordinates,
    all of which have the value 'value' */
CoordGroup::CoordGroup(quint32 size, const Vector &value)
           : CoordGroupBase(size,value)
{}

/** Construct a CoordGroup from the array of passed coordinates */
CoordGroup::CoordGroup(const QVector<Vector> &points)
           : CoordGroupBase(points)
{}

/** Near-copy constructor */
CoordGroup::CoordGroup(const CoordGroupBase &other)
           : CoordGroupBase(other)
{
    if (CoordGroupBase::needsUpdate())
        CoordGroupBase::update();
}

/** Copy constructor */
CoordGroup::CoordGroup(const CoordGroup &other)
           : CoordGroupBase(other)
{}

/** Destructor */
CoordGroup::~CoordGroup()
{}

/** Copy assignment operator */
CoordGroup& CoordGroup::operator=(const CoordGroup &other)
{
    CoordGroupBase::operator=(other);
    return *this;
}

/** Copy assignment from a CoordGroupBase */
CoordGroup& CoordGroup::operator=(const CoordGroupBase &other)
{
    CoordGroupBase::operator=(other);
    
    if (CoordGroupBase::needsUpdate())
        CoordGroupBase::update();

    return *this;
}

/** Return an editor that can be used to edit the 
    coordinates in this group */
CoordGroupEditor CoordGroup::edit() const
{
    return CoordGroupEditor(*this);
}

//////////////
////////////// Implementation of CoordGroupEditor
//////////////

/** Null constructor */
CoordGroupEditor::CoordGroupEditor() : CoordGroupBase()
{}

/** Copy constructor */
CoordGroupEditor::CoordGroupEditor(const CoordGroupBase &other)
                 : CoordGroupBase(other)
{}

/** Destructor */
CoordGroupEditor::~CoordGroupEditor()
{}

/** Assignment operator */
CoordGroupEditor& CoordGroupEditor::operator=(const CoordGroupBase &other)
{
    CoordGroupBase::operator=(other);
    return *this;
}

/** Return a modifiable reference to the 'ith' coordinate in the group
    - this will throw an exception if 'i' refers to an invalid index

    \throw SireError::invalid_index
*/
Vector& CoordGroupEditor::operator[](quint32 i)
{
    CoordGroupBase::assertValidIndex(i);
    CoordGroupBase::setNeedsUpdate();
    
    return _pvt_data()[i];
}

/** Return a modifiable pointer to the array of coordinates -
    *do not delete this pointer* */
Vector* CoordGroupEditor::data()
{
    CoordGroupBase::setNeedsUpdate();
    return _pvt_data();
}

/** Translate this CoordGroup by 'delta' */
void CoordGroupEditor::translate(const Vector &delta)
{
    if (not delta.isZero())
    {
        //translate all of the coordinates
        quint32 ncoords = this->count();
        Vector *coords = _pvt_data();
        
        for (quint32 i=0; i<ncoords; ++i)
        {
            coords[i] += delta;
        }
        
        //now translate the AABox
        if (not this->needsUpdate())
        {
            AABox new_box = this->aaBox();
            new_box.translate(delta);
            this->_pvt_group().update(new_box);
        }
    }
}

/** Translate the 'ith' point in the group by 'delta'

    \throw SireError::invalid_index
*/
void CoordGroupEditor::translate(quint32 i, const Vector &delta)
{
    if (not delta.isZero())
        this->operator[](i) += delta;
}

/** Rotate (and perhaps scale and shear) this group by the matrix 'rotmat'
    about the point 'point' */
void CoordGroupEditor::rotate(const Matrix &rotmat, const Vector &point)
{
    quint32 sz = this->count();
    Vector *coords = this->_pvt_data();

    for (quint32 i=0; i<sz; ++i)
    {
        coords[i] = SireMaths::rotate( coords[i], rotmat, point );
    }
    
    CoordGroupBase::setNeedsUpdate();
}

/** Rotate the 'ith' point in the group using the matrix 'rotmat' about the
    point 'point'

    \throw SireError::index
*/
void CoordGroupEditor::rotate(quint32 i, const Matrix &rotmat, const Vector &point)
{
    Vector &coord = this->operator[](i);
    coord = SireMaths::rotate(coord, rotmat, point);
}

/** Rotate this group by the Quaternion 'quat' about the point 'point' */
void CoordGroupEditor::rotate(const Quaternion &quat, const Vector &point)
{
    this->rotate(quat.toMatrix(), point);
}

/** Rotate the 'ith' point in the group using the quaternion 'quat' about the
    point 'point'

    \throw SireError::index
*/
void CoordGroupEditor::rotate(quint32 i, const Quaternion &quat, const Vector &point)
{
    this->rotate(i, quat.toMatrix(), point);
}

/** Set the coordinates of the CoordGroup to 'newcoords' - this must
    have the same number of points as this CoordGroup or an exception
    will be thrown

    \throw SireError::incompatible_error
*/
void CoordGroupEditor::setCoordinates(const QVector<Vector> &newcoords)
{
    CoordGroupBase::assertSameSize(newcoords);
    
    int sz = this->count();
    Vector *coords = this->_pvt_data();
    const Vector *newcoords_array  = newcoords.constData();
    
    void *output = qMemCopy(coords, newcoords_array, sz*sizeof(Vector));
    CoordGroupBase::setNeedsUpdate();
    
    BOOST_ASSERT(output == coords);    
}

/** Set the coordinates of the CoordGroup to 'newcoords' - this
    must have the same number of points as this CoordGroup or an
    exception will be thrown

    \throw SireError::incompatible_error
*/
void CoordGroupEditor::setCoordinates(const CoordGroupBase &newcoords)
{
    CoordGroupBase::assertSameSize(newcoords);
    CoordGroupBase::operator=(newcoords);
}

/** Return a CoordGroup which is a copy of this group. This will update the
    AABox before making the copy, thus ensuring that the AABox of the returned
    CoordGroup is consistent with its coordinates. */
CoordGroup CoordGroupEditor::commit()
{
    qDebug() << CODELOC;

    if (this->needsUpdate())
        //update the AABox
        this->update();

    qDebug() << CODELOC;

    //return a copy of this CoordGroup
    return CoordGroup(*this);
}
