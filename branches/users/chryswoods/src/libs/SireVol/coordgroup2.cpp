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

#include "coordgroup2.h"

#include "SireMaths/matrix.h"
#include "SireMaths/quaternion.h"
#include "SireMaths/axisset.h"
#include "SireMaths/rotate.h"

#include "SireError/errors.h"

#include <QDebug>

using namespace SireVol;
using namespace SireMaths;

namespace SireVol
{
namespace detail
{

class CGMemory;

class CGData;
class CGArrayData;
class CGArrayArrayData;

/** This class provides the main metadata that describes the array
    of array of CoordGroups */
class CGArrayArrayData : public QSharedData
{
friend class CGMemory;

public:
    CGArrayArrayData();
    CGArrayArrayData(quint32 narray, quint32 ncgroups, quint32 ncoords);
    
    CGArrayArrayData(const CGArrayArrayData &other);
    
    ~CGArrayArrayData();
    
    void incref();
    void decref();
    
    CGArrayArrayData* detach();
    
    const CGArrayData* nullArray() const;
    const CGData* nullCGroup() const;
    
    const CGData* cgDataData() const;
    const CGArrayData* cgArrayDataData() const;
    
    const CoordGroupArray* cgArrayData() const;
    const CoordGroup2* cGroupData() const;
    const AABox* aaBoxData() const;
    const Vector* coordsData() const;

    CGData* cgDataData();
    CGArrayData* cgArrayDataData();
    
    CoordGroupArray* cgArrayData();
    CoordGroup2* cGroupData();
    AABox* aaBoxData();
    Vector* coordsData();
    
    void setNCGroupsInArray(quint32 i, quint32 ncgroups);
    void setNPointsInCGroup(quint32 i, quint32 npoints);
    
    void close();
    
    quint32 nCGArrays() const;
    quint32 nCGroups() const;
    quint32 nCoords() const;
    
    char *memory();
    const char* memory() const;
    
private:
    /** The index in the storage array of the first CoordGroupArray  
        in this array */
    quint32 cgarray0;
    
    /** The index in the storage array of the first CGArrayData */
    quint32 cgarraydata0;
    
    /** The number of CoordGroupArrays in this array */
    quint32 ncgarrays;
    
    /** The index in the storage array of the first CoordGroup in 
        this array */
    quint32 cgroup0;
    
    /** The index in the storage array for the first CGData in this array */
    quint32 cgdata0;
    
    /** The index in the storage array of the first AABox in this
        array (there is one for each CoordGroup) */
    quint32 aabox0;
    
    /** The number of CoordGroups in this array */
    quint32 ncgroups;
    
    /** The index in the storage array of the first vector in
        this array */
    quint32 coords0;
    
    /** The number of vectors in this array */
    quint32 ncoords;
};

/** This class provides the metainformation that describes an
    array of CoordGroups */
class CGArrayData
{

friend class CGMemory;
friend class CGArrayArrayData;

public:
    CGArrayData();
    CGArrayData(quint32 this_idx);
    
    CGArrayData(const CGArrayData &other);
    
    ~CGArrayData();

    void incref();
    void decref();
    
    CGArrayData* detach();
    
    const CoordGroup2* cGroupData() const;
    const AABox* aaBoxData() const;
    const Vector* coordsData() const;
    
    const CoordGroup2* constCGroupData() const;
    const AABox* constAABoxData() const;
    const Vector* constCoordsData() const;
    
    CoordGroup2* cGroupData();
    AABox* aaBoxData();
    Vector* coordsData();
    
    quint32 nCGroups() const;
    quint32 nCoords() const;
    
    char* memory();
    const char* memory() const;
    
private:
    /** The index in the storage array of this CoordGroupArray */
    quint32 this_cgarray;
    
    /** The index into the storage array of the first CoordGroup
        of this array */
    quint32 cgroup0;
    
    /** The index into the storage array of the first AABox of
        this array (same number as number of CoordGroups) */
    quint32 aabox0;
    
    /** The number of CoordGroups in this array */
    quint32 ncgroups;
    
    /** The index into the storage array of the first vector in 
        this array */
    quint32 coords0;
    
    /** The number of vectors in this array */
    quint32 ncoords;
};

/** This class provides the metainformation that describes
    a single CoordGroup */
class CGData
{

friend class CGMemory;
friend class CGArrayArrayData;

public:
    CGData();
    CGData(quint32 this_idx);
    
    CGData(const CGData &other);
    
    ~CGData();
    
    void incref();
    void decref();
    
    CGData* detach();
    
    const char* memory() const;
    const Vector* coordsData() const;
    const AABox* aaBox() const;

    char* memory();
    Vector* coordsData();
    AABox* aaBox();
    
    quint32 nCoords() const;
    
private:
    /** The index in the storage array of this CoordGroup */
    quint32 this_cgroup;
    
    /** The index in the storage array of the first vector in this CoordGroup */
    quint32 coords0;
    
    /** The number of vectors in this CoordGroup */
    quint32 ncoords;
    
    /** The index in the storage array of the AABox for this CoordGroup */
    quint32 aabox;
};

/** This class is used to create, copy and manage the memory of 
    the CoordGroup-type classes
    
    @author Christopher Woods
*/
class CGMemory
{
public:
    static char* create(quint32 narray, quint32 ncgroups, quint32 ncoords);

    static char* detach(char *this_ptr, quint32 this_idx);

    static void destroy(CGArrayArrayData *array);
    
    static void incref(char *this_ptr, quint32 this_idx);
    static void decref(char *this_ptr, quint32 this_idx);

    static char* getRoot(char *this_ptr, quint32 this_idx);
    static const char* getRoot(const char *this_ptr, quint32 this_idx);

private:
    static quint32 getSize(quint32 narrays, quint32 ngroups, quint32 ncoords);
};

} // end of namespace detail;
} // end of namespace SireVol;

using namespace SireVol::detail;

////////
//////// Implementation of CGMemory
////////

/** Private function used to calculate the size of the memory required
    for a container with narrays CoordGroupArrays, containing in total
    ngroups CoordGroups and ncoords Vector coordinates */
quint32 CGMemory::getSize(quint32 narrays, quint32 ngroups, quint32 ncoords)
{
    //need to add space for null arrays and groups
    if (narrays == 0)
        narrays = 1;
        
    if (ngroups == 0)
        ngroups = 1;

    return sizeof(CGArrayArrayData) +
                  narrays * sizeof(CoordGroupArray) + 
                  narrays * sizeof(CGArrayData) + 
                  ngroups * sizeof(CoordGroup2) +
                  ngroups * sizeof(CGData) + 
                  ngroups * sizeof(AABox) + 
                  ncoords * sizeof(Vector);
}

/** Create space for a container that holds narrays CoordGroupArrays,
    ncgroups CoordGroups (in total) and ncoords Vectors (in total) 
    
    // The whole point of this set of classes is so that all of the 
    // relavant coordinate data of a set of molecules is all packed
    // together in memory. This should help keep the cache full,
    // as we won't have to keep bouncing randomly around main memory
    // to find the coordinates of the next CoordGroup to test.
    //
    // The layout in memory for this data is as follows;
    //
    // ---------------------------------------------------------------------
    // |A|  B  |    C      |    D     |                E                   |
    // ---------------------------------------------------------------------
    //
    //  A = CGArrayArrayData object       ( 1 * sizeof(CGArrayArrayData) )
    //  B = Array of CoordGroupArrays +   ( narrays * sizeof(CoordGroupArray) )
    //      Array of CGArrayData objects  ( narrays * sizeof(CGArrayData) )
    //  C = Array of CoordGroups +        ( ncgroups * sizeof(CoordGroup) )
    //      Array of CGData objects       ( ncgroups * sizeof(CGData) )
    //  D = Array of AABoxes 
    //       (one for each CoordGroup)    ( ncgroups * sizeof(AABox) )
    //  E = Array of Vectors              ( ncoords * sizeof(Vector) )
    //
    // This layout packs all of the coordinates together, all of the
    // AABoxes together (used for sphere cutting tests), and then
    // all of the CoordGroup, then CGArray metadata together. This layout
    // also then allows easy iteration over sub-parts, e.g. iterating
    // over all coordinates in all CoordGroups in all arrays, or
    // over all coordinates in all CoordGroups in a specific array,
    // or over all coordinates in a specific CoordGroup.
*/
char* CGMemory::create(quint32 narrays, quint32 ncgroups, quint32 ncoords)
{
    //calculate how big the memory requirements are
    quint32 sz = getSize(narrays, ncgroups, ncoords);
    
    //allocate this space
    char *storage = new char[sz];
    
    try
    {
        //now we need to construct each object in turn at its correct
        //location in this space
        CGArrayArrayData *cgarrayarray 
                    = new (storage) CGArrayArrayData(narrays, ncgroups, ncoords);
                    
        //add a reference count for this object
        cgarrayarray->ref.ref();

        //advance the index into the storage array so that we are now
        //pointing just after the CGArrayArrayData
        quint32 idx = sizeof(CGArrayArrayData);
                    
        //this is the location of the first CoordGroupArray :-)
        cgarrayarray->cgarray0 = idx;
        
        //the first CGArrayData lies at this index + narrays*sizeof(CoordGroupArray)
        cgarrayarray->cgarraydata0 = idx + (narrays * sizeof(CoordGroupArray));

        if (narrays > 0)
        {
            quint32 dataidx = cgarrayarray->cgarraydata0;
        
            //loop over each CGArray and create it in its place
            for (quint32 i=0; i<narrays; ++i)
            {   
                //assert that there is sufficient space in the array
                BOOST_ASSERT(idx + sizeof(CoordGroupArray) < sz);
                BOOST_ASSERT(dataidx + sizeof(CGArrayData) < sz);
            
                //create the CGArrayData, letting it know where it
                //is relative to the beginning of the storage array
                CGArrayData *data = new (storage + dataidx) CGArrayData(dataidx);
                
                //now create the CoordGroupArray that uses this data
                new (storage + idx) CoordGroupArray(data);
        
                //advance the index into the storage array to point
                //just after the just-created CGArrayData
                idx += sizeof(CoordGroupArray);
                dataidx += sizeof(CGArrayData);
            }
            
            //make idx point just after all of the CGArrayDatas...
            BOOST_ASSERT( idx == cgarrayarray->cgarray0 
                                + narrays*sizeof(CoordGroupArray) );
                                
            idx = dataidx;
        }
        else
        {
            //we need to create space for the null CoordGroupArray
            CGArrayData *cgarray = new (storage + idx) CGArrayData(idx);
            cgarray->ncgroups = 0;
            cgarray->cgroup0 = 0;
            cgarray->ncoords = 0;
            cgarray->coords0 = 0;
            cgarray->aabox0 = 0;

            cgarrayarray->ncgarrays = 0;
            
            idx += sizeof(CGArrayData);
        }
        
        //we are now at the location of the first CGData
        cgarrayarray->cgroup0 = idx;
        cgarrayarray->cgdata0 = idx + (ncgroups * sizeof(CoordGroup2));
        
        if (ncgroups > 0)
        {
            quint32 dataidx = cgarrayarray->cgdata0;
        
            //loop over each CoordGroup and create it in its place
            for (quint32 i=0; i<ncgroups; ++i)
            {
                //assert that there is sufficient space in the array
                BOOST_ASSERT(idx + sizeof(CoordGroup2) < sz);
                BOOST_ASSERT(dataidx + sizeof(CGData) < sz);
            
                //create the CGData, letting it know where it is
                //relative to the beginning of the storage array
                CGData *data = new (storage + dataidx) CGData(dataidx);
                
                //create the CoordGroup
                new (storage + idx) CoordGroup2(data);
                
                //advance the index into the storage array to point
                //just after the just-created CGData
                idx += sizeof(CoordGroup2);
                dataidx += sizeof(CGData); 
            }
        
            BOOST_ASSERT( idx == cgarrayarray->cgroup0 + 
                                    ncgroups * sizeof(CoordGroup2) );
        
            idx = dataidx;
        
            //we are now at the location of the first AABox
            cgarrayarray->aabox0 = idx;
        
            //loop over each AABox and create it in place
            for (quint32 i=0; i<ncgroups; ++i)
            {
                BOOST_ASSERT(idx + sizeof(AABox) < sz);
            
                new (storage + idx) AABox();
            
                idx += sizeof(AABox);
            }
        
            //we are now at the location of the first Vector
            cgarrayarray->coords0 = idx;
        
            //loop over each vector and create it in place
            for (quint32 i=0; i<ncoords; ++i)
            {
                BOOST_ASSERT(idx + sizeof(Vector) <= sz);

                new (storage + idx) Vector(0.0);
                
                idx += sizeof(Vector);
            }
        }
        else
        {
            //create space for the null CoordGroup
            CGData *cgroup = new (storage + idx) CGData(idx);
            cgroup->ncoords = 0;
            cgroup->coords0 = 0;
            cgroup->aabox = 0;
            
            cgarrayarray->aabox0 = 0;
            cgarrayarray->coords0 = 0;
            cgarrayarray->ncgroups = 0;
            
            idx += sizeof(CGData);
        }
        
        //we should now be at the end of the storage
        BOOST_ASSERT( idx == sz );
                
        return storage;
    } 
    catch(...)
    {
        //release the memory so we don't leak
        delete[] storage;
        throw;
    }
    
    return 0;
}

/** This converts the pointer to the object
    that starts at 'this_ptr' to a pointer to the first element
    of the storage array (given the location of the object at
    index 'this_idx' in the storage array) */
char* CGMemory::getRoot(char *this_ptr, quint32 this_idx)
{
    return this_ptr - this_idx;
}

/** This converts the pointer to the object
    that starts at 'this_ptr' to a pointer to the first element
    of the storage array (given the location of the object at
    index 'this_idx' in the storage array) */
const char* CGMemory::getRoot(const char *this_ptr, quint32 this_idx)
{
    return this_ptr - this_idx;
}

/** Destroy the object 'array' */
void CGMemory::destroy(CGArrayArrayData *array)
{
    //we need to delete it in the opposite order to creation - first
    //lets delete all of the vectors
    quint32 ncoords = array->nCoords();

    char *storage = (char*)array;
        
    if (ncoords > 0)
    {
        Vector *coords = array->coordsData();
        
        for (qint32 i=ncoords-1; i>=0; --i)
        {
            coords[i].~Vector();
        }
    }
        
    //now delete all of the AABoxes
    quint32 ncgroups = array->nCGroups();
        
    if (ncgroups > 0)
    {
        AABox *aaboxes = array->aaBoxData();
            
        for (qint32 i=ncgroups-1; i>=0; --i)
        {
            aaboxes[i].~AABox();
        }
        
        //now delete all of the CGData objects
        CoordGroup2 *cgroups = array->cGroupData();
            
        for (qint32 i=ncgroups-1; i>=0; --i)
        {
            CGData *data = const_cast<CGData*>( cgroups[i].d.constData() );
            
            //delete the CGData
            data->~CGData();
            
            //remove the shared pointer
            cgroups[i].d.weakRelease();
            
            //delete the CoordGroup
            cgroups[i].~CoordGroup2();
        }
    }
        
    //now delete all of the CGArrayData objects
    quint32 narrays = array->nCGArrays();
        
    if (narrays > 0)
    {
        CoordGroupArray *arrays = array->cgArrayData();
            
        for (qint32 i=narrays-1; i>=0; --i)
        {
            CGArrayData *data = const_cast<CGArrayData*>(arrays[i].d.constData());
            
            //delete the data
            data->~CGArrayData();
            
            //release the shared pointer
            arrays[i].d.weakRelease();
            
            //delete the array
            arrays[i].~CoordGroupArray();
        }
    }
        
    //delete the CGArrayArrayData object itself
    array->~CGArrayArrayData();
        
    //finally delete the memory itself
    delete[] storage;
}

/** Detach this object from shared storage and return a new pointer to it. */
char* CGMemory::detach(char *this_ptr, quint32 this_idx)
{
    //get a pointer to the start of the storage for this container
    char *storage = getRoot(this_ptr, this_idx);
    
    //The CGArrayArrayData object is at the beginning of this storage array
    CGArrayArrayData *cgarrayarray = (CGArrayArrayData*) storage;
    
    if (cgarrayarray->ref != 1)
    {
        //there is more than one reference to this data - it will have to 
        //be cloned - get the size of memory to be cloned
        int sz = getSize(cgarrayarray->nCGArrays(), 
                         cgarrayarray->nCGroups(),
                         cgarrayarray->nCoords());
                         
        //create space for the copy
        char *new_storage = new char[sz];
        
        //copy the data
        void *output = qMemCopy( new_storage, storage, sz*sizeof(char) );
        BOOST_ASSERT(output == new_storage);

        //the first part of the data is the CGArrayArrayData object
        CGArrayArrayData *new_cgarrayarray = (CGArrayArrayData*) new_storage;
        
        //set the reference count of this copy to 1
        new_cgarrayarray->ref = 1;

        //now loose a reference to the original
        CGMemory::decref(this_ptr, this_idx);
        
        //the final step is to update all of the CoordGroup and CoordGroupArray
        //pointers that exist in this array (otherwise they will all
        //point to the original!)
        
        if (new_cgarrayarray->nCGArrays() > 0)
        {
            CoordGroupArray *cgarray = new_cgarrayarray->cgArrayData();
            CGArrayData *data = new_cgarrayarray->cgArrayDataData();
        
            for (quint32 i=0; i < new_cgarrayarray->nCGArrays(); ++i)
            {
                cgarray[i].d.weakRelease();
                cgarray[i].d.weakAssign( &(data[i]) );
            }
        }
        
        if (new_cgarrayarray->nCGroups() > 0)
        {
            CoordGroup2 *cgroups = new_cgarrayarray->cGroupData();
            CGData *data = new_cgarrayarray->cgDataData();
            
            for (quint32 i=0; i < new_cgarrayarray->nCGroups(); ++i)
            {
                cgroups[i].d.weakRelease();
                cgroups[i].d.weakAssign( &(data[i]) );
            }
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

/** Increase the reference count */
void CGMemory::incref(char *this_ptr, quint32 this_idx)
{
    ( (CGArrayArrayData*)(getRoot(this_ptr,this_idx)) )->incref();
}

/** Decrease the reference count */
void CGMemory::decref(char *this_ptr, quint32 this_idx)
{
    ( (CGArrayArrayData*)(getRoot(this_ptr,this_idx)) )->decref();
}

////////
//////// Implementation of CGArrayArrayData
////////

/** Null constructor */
CGArrayArrayData::CGArrayArrayData() 
                 : QSharedData(), 
                   cgarray0(0), cgarraydata0(0), ncgarrays(0),
                   cgroup0(0), cgdata0(0), aabox0(0), ncgroups(0),
                   coords0(0), ncoords(0)
{}

/** Construct metadata for the specified number of subgroups */
CGArrayArrayData::CGArrayArrayData(quint32 narray, quint32 ngroup, 
                                   quint32 ncoord)
                 : QSharedData(),
                   cgarray0(0), cgarraydata0(0), ncgarrays(narray),
                   cgroup0(0), cgdata0(0), aabox0(0), ncgroups(ngroup),
                   coords0(0), ncoords(ncoord)
{}
  
/** Copy constructor */               
CGArrayArrayData::CGArrayArrayData(const CGArrayArrayData &other)
                 : QSharedData(),
                   cgarray0(other.cgarray0), cgarraydata0(other.cgarraydata0),
                   ncgarrays(other.ncgarrays),
                   cgroup0(other.cgroup0), cgdata0(other.cgdata0),
                   aabox0(other.aabox0),
                   ncgroups(other.ncgroups),
                   coords0(other.coords0), ncoords(other.ncoords)
{}

/** Destructor */
CGArrayArrayData::~CGArrayArrayData()
{}

/** Return a pointer to the start of the memory storage for this container */
const char* CGArrayArrayData::memory() const
{
    return CGMemory::getRoot( (const char*)this, 0 );
}

/** Return a modifiable pointer to the start of the memory storage
    for this container - this detaches this container from implicitly
    shared storage */
char* CGArrayArrayData::memory()
{
    return CGMemory::getRoot( (char*)this, 0 );
}

/** Return a pointer to the null CoordGroupArray */
const CGArrayData* CGArrayArrayData::nullArray() const
{
    BOOST_ASSERT( ncgarrays == 0 );
    BOOST_ASSERT( cgarray0 != 0 );
    
    return (const CGArrayData*)( memory()[cgarraydata0] );
}

/** Return a pointer to the null CoordGroup */
const CGData* CGArrayArrayData::nullCGroup() const
{
    BOOST_ASSERT( ncgroups == 0 );
    BOOST_ASSERT( cgroup0 != 0 );
    
    return (const CGData*)( memory()[cgdata0] );
}

/** Return a pointer to a CGArrayArrayData copy of this that has
    been detached from shared storage */
CGArrayArrayData* CGArrayArrayData::detach()
{
    return (CGArrayArrayData*)( CGMemory::detach( (char*)(this), 0 ) );
}

/** Increment the reference count for this object */
void CGArrayArrayData::incref()
{
    this->ref.ref();
}
    
/** Decrease the reference count for this object - this will
    delete this object if the reference count drops to zero! */
void CGArrayArrayData::decref()
{
    if (not this->ref.deref())
        CGMemory::destroy(this);
}

/** Return a pointer to the first CGArrayData in this container. This
    returns 0 if there are no arrays in this container */
CGArrayData* CGArrayArrayData::cgArrayDataData()
{
    if (cgarraydata0 == 0)
        return 0;
    else
        return (CGArrayData*)( memory() + cgarraydata0 );
}

/** Return a pointer to the first CGArrayData in this container. This
    returns 0 if there are no arrays in this container */
const CGArrayData* CGArrayArrayData::cgArrayDataData() const
{
    if (cgarraydata0 == 0)
        return 0;
    else
        return (const CGArrayData*)( memory() + cgarraydata0 );
}

/** Return a pointer to the first CoordGroupArray in this container. This returns
    0 if there are no arrays in this container */
const CoordGroupArray* CGArrayArrayData::cgArrayData() const
{
    if (cgarray0 == 0)
        return 0;
    else
        return (const CoordGroupArray*)( memory() + cgarray0 );
}

/** Return a pointer to the first CGData in this container. This returns 
    0 if there are no CoordGroups in this container */
CGData* CGArrayArrayData::cgDataData()
{
    if (cgdata0 == 0)
        return 0;
    else
        return (CGData*)( memory() + cgdata0 );
}

/** Return a pointer to the first CGData in this container. This returns 
    0 if there are no CoordGroups in this container */
const CGData* CGArrayArrayData::cgDataData() const
{
    if (cgdata0 == 0)
        return 0;
    else
        return (const CGData*)( memory() + cgdata0 );
}

/** Return a pointer to the first CoordGroup in this container. This returns
    0 if there are no CoordGroups in this container */
const CoordGroup2* CGArrayArrayData::cGroupData() const
{
    if (cgroup0 == 0)
        return 0;
    else
        return (const CoordGroup2*)( memory() + cgroup0 );
}

/** Return a pointer to the first AABox in this container. This returns
    0 if there are no AABoxes in this container */
const AABox* CGArrayArrayData::aaBoxData() const
{
    if (aabox0 == 0)
        return 0;
    else
        return (const AABox*)( memory() + aabox0 );
}

/** Return a pointer to the first Vector in this container. This
    returns 0 if there are no vectors in this container */
const Vector* CGArrayArrayData::coordsData() const
{
    if (coords0 == 0)
        return 0;
    else
        return (const Vector*)( memory() + coords0 );
}

/** Return a pointer to the first CoordGroupArray in this container. This returns
    0 if there are no arrays in this container */
CoordGroupArray* CGArrayArrayData::cgArrayData()
{
    if (cgarray0 == 0)
        return 0;
    else
        return (CoordGroupArray*)( memory() + cgarray0 );
}

/** Return a pointer to the first CoordGroup in this container. This returns
    0 if there are no CoordGroups in this container */
CoordGroup2* CGArrayArrayData::cGroupData()
{
    if (cgroup0 == 0)
        return 0;
    else
        return (CoordGroup2*)( memory() + cgroup0 );
}

/** Return a pointer to the first AABox in this container. This returns
    0 if there are no AABoxes in this container */
AABox* CGArrayArrayData::aaBoxData()
{
    if (aabox0 == 0)
        return 0;
    else
        return (AABox*)( memory() + aabox0 );
}

/** Return a pointer to the first Vector in this container. This
    returns 0 if there are no vectors in this container */
Vector* CGArrayArrayData::coordsData()
{
    if (coords0 == 0)
        return 0;
    else
        return (Vector*)( memory() + coords0 );
}

/** Return the number of CoordGroup arrays in this container */
quint32 CGArrayArrayData::nCGArrays() const
{
    return ncgarrays;
}

/** Return the number of CoordGroups in this container (in total) */
quint32 CGArrayArrayData::nCGroups() const
{
    return ncgroups;
}

/** Return the total number of coordinates in this container */
quint32 CGArrayArrayData::nCoords() const
{
    return ncoords;
}

/** Set the number of CoordGroups in the ith CoordGroupArray */
void CGArrayArrayData::setNCGroupsInArray(quint32 i, quint32 ncgroups)
{
    BOOST_ASSERT( i < ncgarrays );    
    this->cgArrayDataData()[i].ncgroups = ncgroups;
}

/** Set the number of coordinates in the ith CoordGroup */
void CGArrayArrayData::setNPointsInCGroup(quint32 i, quint32 ncoords)
{
    BOOST_ASSERT( i < ncgroups );
    this->cgDataData()[i].ncoords = ncoords;
}

/** Close this array - call this once you have finished constructing
    the data */
void CGArrayArrayData::close()
{
    CGArrayData *cgarrays = this->cgArrayDataData();
    CGData *cgroups = this->cgDataData();
    
    //ensure that everything adds up...
    quint32 n_assigned_cgroups = 0;
    quint32 n_assigned_coords = 0;
    
    //first close out the CoordGroups
    for (quint32 i=0; i<ncgroups; ++i)
    {
        CGData &cgroup = cgroups[i];

        //tell the CoordGroup where its first point is located
        if (cgroup.nCoords() > 0)
            cgroup.coords0 = coords0 + n_assigned_coords * sizeof(Vector);
        else
            cgroup.coords0 = 0;
        
        //tell the CoordGroup where its AABox is located
        cgroup.aabox = aabox0 + i*sizeof(AABox);
        
        n_assigned_coords += cgroup.nCoords();
    }

    BOOST_ASSERT( n_assigned_coords == ncoords );

    n_assigned_coords = 0;

    //now close out the CoordGroupArrays
    for (quint32 i=0; i<ncgarrays; ++i)
    {
        CGArrayData &cgarray = cgarrays[i];
    
        if (cgarray.nCGroups() > 0)
        {
            //tell the array where its first CoordGroup is located
            cgarray.cgroup0 = cgroup0 + n_assigned_cgroups * sizeof(CoordGroup2);
        
            //tell the array where its first AABox is located
            cgarray.aabox0 = aabox0 + n_assigned_cgroups * sizeof(AABox);
        
            //sum up the number of coordinates in the CoordGroups in this array
            quint32 ncgcoords = 0;
            const CoordGroup2 *array_groups = cgarray.constCGroupData();
        
            for (quint32 j=0; j<cgarray.nCGroups(); ++j)
            {
                ncgcoords += array_groups[j].count();
            }
        
            cgarray.ncoords = ncgcoords;
            
            if (ncgcoords > 0)
            {
                //tell the array where its first point is located
                cgarray.coords0 = coords0 + n_assigned_coords * sizeof(Vector);
                n_assigned_coords += ncgcoords;
            }
            else
                cgarray.coords0 = 0;
        }
        else
        {
            cgarray.cgroup0 = 0;
            cgarray.aabox0 = 0;
            cgarray.coords0 = 0;
        }
        
        n_assigned_cgroups += cgarray.ncgroups;
    }
    
    BOOST_ASSERT( n_assigned_coords == ncoords );
    BOOST_ASSERT( n_assigned_cgroups == ncgroups );
}

////////
//////// Implementation of CGArrayData
////////

/** Null constructor */
CGArrayData::CGArrayData()
            : this_cgarray(0), cgroup0(0), aabox0(0),
              ncgroups(0), coords0(0), ncoords(0)
{}

/** Construct, telling this array that it is at index 'this_idx' */
CGArrayData::CGArrayData(quint32 this_idx)
            : this_cgarray(this_idx), cgroup0(0), aabox0(0),
              ncgroups(0), coords0(0), ncoords(0)
{}

/** Copy constructor */
CGArrayData::CGArrayData(const CGArrayData &other)
            : this_cgarray(other.this_cgarray), 
              cgroup0(other.cgroup0), 
              aabox0(other.aabox0), ncgroups(other.ncgroups),
              coords0(other.coords0), ncoords(other.ncoords)
{}

/** Destructor */
CGArrayData::~CGArrayData()
{}

/** Return a const pointer to the start of the raw memory
    that holds all of the data of everything in the container */
const char* CGArrayData::memory() const
{
    return CGMemory::getRoot( (const char*)this, this_cgarray );
}

/** Return a pointer to the start of the raw memory
    that holds all of the data of everything in the container.
    This will detach from shared storage */
char* CGArrayData::memory()
{
    return CGMemory::getRoot( (char*)this, this_cgarray );
}

/** Increment the reference count for this object */
void CGArrayData::incref()
{
    CGMemory::incref( (char*)this, this_cgarray );
}

/** Decrement the reference count for this object - this may delete it! */
void CGArrayData::decref()
{
    CGMemory::decref( (char*)this, this_cgarray );
}

/** Detach from shared storage */
CGArrayData* CGArrayData::detach()
{
    return (CGArrayData*)( CGMemory::detach( (char*)this, this_cgarray ) );
}

/** Return a pointer to the first CoordGroup in this array.
    This returns 0 if there are no CoordGroups in this array */
const CoordGroup2* CGArrayData::cGroupData() const
{
    if (cgroup0 == 0)
        return 0;
    else
        return (const CoordGroup2*)( memory() + cgroup0 );
}

/** Return a pointer to the AABox belonging to the first CoordGroup
    in this array. This returns 0 if there are no CoordGroups in this
    array */
const AABox* CGArrayData::aaBoxData() const
{
    if (aabox0 == 0)
        return 0;
    else
        return (const AABox*)( memory() + aabox0 );
}

/** Return a pointer to the coordinates of the first point in the 
    first CoordGroup in this array. This returns 0 if there are
    no points in this array */
const Vector* CGArrayData::coordsData() const
{
    if (coords0 == 0)
        return 0;
    else
        return (const Vector*)( memory() + coords0 );
}

/** Return a pointer to the first CoordGroup in this array.
    This returns 0 if there are no CoordGroups in this array */
const CoordGroup2* CGArrayData::constCGroupData() const
{
    if (cgroup0 == 0)
        return 0;
    else
        return (const CoordGroup2*)( memory() + cgroup0 );
}

/** Return a pointer to the AABox belonging to the first CoordGroup
    in this array. This returns 0 if there are no CoordGroups in this
    array */
const AABox* CGArrayData::constAABoxData() const
{
    if (aabox0 == 0)
        return 0;
    else
        return (const AABox*)( memory() + aabox0 );
}

/** Return a pointer to the coordinates of the first point in the 
    first CoordGroup in this array. This returns 0 if there are
    no points in this array */
const Vector* CGArrayData::constCoordsData() const
{
    if (coords0 == 0)
        return 0;
    else
        return (const Vector*)( memory() + coords0 );
}

/** Return a pointer to the first CoordGroup in this array.
    This returns 0 if there are no CoordGroups in this array */
CoordGroup2* CGArrayData::cGroupData()
{
    if (cgroup0 == 0)
        return 0;
    else
        return (CoordGroup2*)( memory() + cgroup0 );
}

/** Return a pointer to the AABox belonging to the first CoordGroup
    in this array. This returns 0 if there are no CoordGroups in this
    array */
AABox* CGArrayData::aaBoxData()
{
    if (aabox0 == 0)
        return 0;
    else
        return (AABox*)( memory() + aabox0 );
}

/** Return a pointer to the coordinates of the first point in the 
    first CoordGroup in this array. This returns 0 if there are
    no points in this array */
Vector* CGArrayData::coordsData()
{
    if (coords0 == 0)
        return 0;
    else
        return (Vector*)( memory() + coords0 );
}

/** Return the number of CoordGroups in this array */
quint32 CGArrayData::nCGroups() const
{
    return ncgroups;
}

/** Return the number of coordinates (in total) in this array */
quint32 CGArrayData::nCoords() const
{
    return ncoords;
}

////////
//////// Implementation of CGData
////////

/** Null constructor */
CGData::CGData()
       : this_cgroup(0), coords0(0), ncoords(0), aabox(0)
{}

/** Construct the data, telling it that it is at 
    index 'this_idx' in the memory storage array */
CGData::CGData(quint32 this_idx)
       : this_cgroup(this_idx), coords0(0), ncoords(0), aabox(0)
{}

/** Copy constructor */
CGData::CGData(const CGData &other)
       : this_cgroup(other.this_cgroup), coords0(other.coords0),
         ncoords(other.ncoords), aabox(other.aabox)
{}

/** Destructor */
CGData::~CGData()
{}

/** Return a pointer to a CGData that has been detached from 
    shared storage (could be a pointer to this CGData) */
CGData* CGData::detach()
{
    return (CGData*)( CGMemory::detach( (char*)this, this_cgroup ) );
}

/** Return a const pointer to the start of the storage
    array that contains this CGData */
const char* CGData::memory() const
{
    return CGMemory::getRoot( (const char*)this, this_cgroup );
}

/** Return a pointer to the start of the storage array that
    contains this CGData - this will detach from shared storage */
char* CGData::memory()
{
    return CGMemory::getRoot( (char*)this, this_cgroup );
}

/** Increment the reference count for this object */
void CGData::incref()
{
    CGMemory::incref( (char*)this, this_cgroup );
}

/** Decrement the reference count for this object - this may delete it! */
void CGData::decref()
{
    CGMemory::decref( (char*)this, this_cgroup );
}

/** Return a pointer to the coordinates of the first point in this
    CoordGroup. This returns 0 if there are no points in this CoordGroup */
const Vector* CGData::coordsData() const
{
    if (coords0 == 0)
        return 0;
    else
        return (const Vector*)( memory() + coords0 );
}

/** Return a pointer to the AABox that surrounds all of the points
    in this CoordGroup */
const AABox* CGData::aaBox() const
{
    return (const AABox*)( memory() + aabox );
}

/** Return a pointer to the coordinates of the first point in this
    CoordGroup. This returns 0 if there are no points in this CoordGroup */
Vector* CGData::coordsData()
{
    if (coords0 == 0)
        return 0;
    else
        return (Vector*)( memory() + coords0 );
}

/** Return a pointer to the AABox that surrounds all of the points
    in this CoordGroup */
AABox* CGData::aaBox()
{
    return (AABox*)( memory() + aabox );
}

/** Return the number of points in this CoordGroup */
quint32 CGData::nCoords() const
{
    return ncoords;
}

////////
//////// Implementation of CoordGroupBase
////////

static CGSharedPtr<CGArrayArrayData> shared_null;

static const CGSharedPtr<CGArrayArrayData>& getSharedNull()
{
    if (shared_null.constData() == 0)
    {
        shared_null = (CGArrayArrayData*)( CGMemory::create(0,0,0) );
        shared_null->close();
    }
    
    return shared_null;
}

static CGSharedPtr<CGData> getSharedNullCoordGroup()
{
    const CGSharedPtr<CGArrayArrayData> &array = ::getSharedNull();
    
    return CGSharedPtr<CGData>( array->nullCGroup() );
}

/** Null constructor */
CoordGroup2Base::CoordGroup2Base()
               : d( ::getSharedNullCoordGroup() )
{}

/** Private constructor used only by CGData */
CoordGroup2Base::CoordGroup2Base(CGData *data)
{
    d.weakAssign(data);
}

static CGSharedPtr<CGData> createCoordGroup(quint32 size)
{
    if (size == 0)
        return ::getSharedNullCoordGroup();

    //construct space for 1 array of 1 CoordGroup of size atoms
    char *storage = CGMemory::create(1, 1, size);
        
    CGArrayArrayData *array = (CGArrayArrayData*)storage;
        
    array->setNCGroupsInArray(0, 1);
    array->setNPointsInCGroup(0, size);
        
    //close the array - this validates that it is complete
    array->close();
    
    return CGSharedPtr<CGData>( &(array->cgDataData()[0]) );
}

/** Construct a CoordGroup that can hold up to 'size'
    coordinates, each of value 'value' */
CoordGroup2Base::CoordGroup2Base(quint32 size, const Vector &value)
                : d( createCoordGroup(size) )
{
    if (size > 0 and not value.isZero())
    {
        Vector *coords = d->coordsData();
        
        //initialise all of the coordinates to the specified value
        for (quint32 i=0; i<size; ++i)
        {
            coords[i] = value;
        }
        
        //update the AABox
        *(d->aaBox()) = AABox(value);
    }
}

/** Construct a CoordGroup that holds the 'size' points
    copied from the array 'values' */
CoordGroup2Base::CoordGroup2Base(quint32 size, const Vector *values)
                : d( createCoordGroup(size) )
{
    if (size > 0)
    {
        Vector *coords = d->coordsData();
    
        for (quint32 i=0; i<size; ++i)
        {
            coords[i] = values[i];
        }
        
        *(d->aaBox()) = AABox(*this);
    }
}

/** Construct a CoordGroup from the specified coordinates */
CoordGroup2Base::CoordGroup2Base(const QVector<Vector> &coordinates)
                : d( createCoordGroup(coordinates.count()) )
{
    //how many coordinates are there?
    quint32 ncoords = coordinates.count();

    if (ncoords > 0)
    {
        Vector *coords = d->coordsData();
        
        //block copy the coordinates
        void *output = qMemCopy( coords, coordinates.constData(),
                                 ncoords*sizeof(Vector) );

        BOOST_ASSERT( output == coords );
        
        //now update the AABox
        *(d->aaBox()) = AABox(coordinates);
    }
}

/** Copy constructor - this is fast as CoordGroup2Base is implicitly
    shared */
CoordGroup2Base::CoordGroup2Base(const CoordGroup2Base &other)
                : d(other.d)
{}

/** Destructor - this will only delete the data if this
    is the only reference to it */
CoordGroup2Base::~CoordGroup2Base()
{}

/** Copy assignment operator - CoordGroup2Base is implicitly
    shared so this will be fast */
CoordGroup2Base& CoordGroup2Base::operator=(const CoordGroup2Base &other)
{
    d = other.d;
    return *this;
}

/** Comparison operator */
bool CoordGroup2Base::operator==(const CoordGroup2Base &other) const
{
    if ( d.constData() == other.d.constData() )
    {
        return true;
    }
    else if (this->count() != other.count())
    {
        return false;
    }
    else
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
}

/** Comparison operator */
bool CoordGroup2Base::operator!=(const CoordGroup2Base &other) const
{
    return not this->operator==(other);
}

/** Return whether 'other' may be different to this group - this uses
    a simple comparison of the memory addresses of the storage of
    these two groups to see if they are definitely the same, or maybe
    different */
bool CoordGroup2Base::maybeDifferent(const CoordGroup2Base &other) const
{
    return d.constData() != other.d.constData();
}

/** Return whether this group is empty (has no coordinates) */
bool CoordGroup2Base::isEmpty() const
{
    return d->nCoords() == 0;
}

/** Return the enclosing AABox */
const AABox& CoordGroup2Base::aaBox() const
{
    return *(d->aaBox());
}

/** Return a raw pointer to the array containing all
    of the coordinates */
const Vector* CoordGroup2Base::constData() const
{
    return d->coordsData();
}

/** Return a raw pointer to the array containing all
    of the coordinates */
const Vector* CoordGroup2Base::data() const
{
    return d->coordsData();
}

/** Return the number of coordinates in this group */
int CoordGroup2Base::count() const
{
    return d->nCoords();
}

/** Return the number of coordinates in this group */
int CoordGroup2Base::size() const
{
    return d->nCoords();
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
void CoordGroup2Base::assertSameSize(const QVector<Vector> &coordinates) const
{
    assertSame(this->count(), coordinates.count());
}

/** Assert that the CoordGroup 'other' contains the same number of
    coordinates as are in this group

    \throw SireError::incompatible_error
*/
void CoordGroup2Base::assertSameSize(const CoordGroup2Base &other) const
{
    assertSame(this->count(), other.count());
}

/** Assert that 'i' is a valid index in this group

    \throw SireError::invalid_index
*/
void CoordGroup2Base::assertValidIndex(quint32 i) const
{
    if (i >= quint32(this->size()))
        throw SireError::invalid_index( QObject::tr(
            "Invalid index: %1 - maximum index == %2.")
                .arg(i).arg(this->size()), CODELOC );
}

/** Return the 'ith' coordinate of this group

    \throw SireError::invalid_index
*/
const Vector& CoordGroup2Base::at(quint32 i) const
{
    assertValidIndex(i);
    return d->coordsData()[i];
}

/** Return the 'ith' coordinate of this group

    \throw SireError::invalid_index
*/
const Vector& CoordGroup2Base::operator[](quint32 i) const
{
    assertValidIndex(i);
    return d->coordsData()[i];
}

////////
//////// Implementation of CoordGroup
////////

/** Null constructor */
CoordGroup2::CoordGroup2() : CoordGroup2Base()
{}

/** Private constructor used only by CGData */
CoordGroup2::CoordGroup2(CGData *data) : CoordGroup2Base(data)
{}

/** Construct a CoordGroup2 that holds 'size' coordinates */
CoordGroup2::CoordGroup2(quint32 size)
           : CoordGroup2Base(size)
{}

/** Construct a CoordGroup2 that holds 'size' coordinates,
    all of which have the value 'value' */
CoordGroup2::CoordGroup2(quint32 size, const Vector &value)
           : CoordGroup2Base(size,value)
{}

/** Construct a CoordGroup2 that holds the 'size' points
    copied from the array 'values' */
CoordGroup2::CoordGroup2(quint32 size, const Vector *values)
           : CoordGroup2Base(size, values)
{}

/** Construct a CoordGroup2 from the array of passed coordinates */
CoordGroup2::CoordGroup2(const QVector<Vector> &points)
           : CoordGroup2Base(points)
{}

/** Construct from a CoordGroup2Editor (only CoordGroup2Editor::commit()
    calls this function, and in this case it has already made sure that
    the AABox is up-to-date) */
CoordGroup2::CoordGroup2(const CoordGroup2Editor &editor)
            : CoordGroup2Base(editor)
{}

/** Copy constructor */
CoordGroup2::CoordGroup2(const CoordGroup2 &other)
           : CoordGroup2Base(other)
{}

/** Destructor */
CoordGroup2::~CoordGroup2()
{}

void CoordGroup2::throwInvalidCountError(uint nats0, uint nats1)
{
    throw SireError::invalid_state( QObject::tr(
        "You can only split a CoordGroup2 into groups if the total number "
        "of atoms in the single CoordGroup2 and the group of CoordGroup2s "
        "is the same. It isn't in this case! %1 vs. %2.")
            .arg(nats0).arg(nats1), CODELOC );
}

/** Copy assignment operator */
CoordGroup2& CoordGroup2::operator=(const CoordGroup2 &other)
{
    CoordGroup2Base::operator=(other);
    return *this;
}

/** Copy assignment from a CoordGroup2Editor */
CoordGroup2& CoordGroup2::operator=(CoordGroup2Editor &other)
{
    return this->operator=( other.commit() );
}

/** Return an editor that can be used to edit the
    coordinates in this group */
CoordGroup2Editor CoordGroup2::edit() const
{
    return CoordGroup2Editor(*this);
}

////////
//////// Implementation of CoordGroupEditor
////////

/** Null constructor */
CoordGroup2Editor::CoordGroup2Editor() 
                  : CoordGroup2Base(), needsupdate(false)
{}

/** Construct from a CoordGroup */
CoordGroup2Editor::CoordGroup2Editor(const CoordGroup2 &other)
                 : CoordGroup2Base(other), needsupdate(false)
{}

/** Copy constructor */
CoordGroup2Editor::CoordGroup2Editor(const CoordGroup2Editor &other)
                  : CoordGroup2Base(other), needsupdate(other.needsupdate)
{}

/** Destructor */
CoordGroup2Editor::~CoordGroup2Editor()
{}

/** Copy assignment operator */
CoordGroup2Editor& CoordGroup2Editor::operator=(const CoordGroup2Editor &other)
{
    CoordGroup2Base::operator=(other);
    needsupdate = other.needsupdate;
    
    return *this;
}

/** Assignment operator */
CoordGroup2Editor& CoordGroup2Editor::operator=(const CoordGroup2 &other)
{
    CoordGroup2Base::operator=(other);
    needsupdate = false;
    
    return *this;
}

/** Return a modifiable reference to the 'ith' coordinate in the group
    - this will throw an exception if 'i' refers to an invalid index

    \throw SireError::invalid_index
*/
Vector& CoordGroup2Editor::operator[](quint32 i)
{
    CoordGroup2Base::assertValidIndex(i);
    needsupdate = true;

    return d->coordsData()[i];
}

/** Return a modifiable pointer to the array of coordinates -
    *do not delete this pointer* */
Vector* CoordGroup2Editor::data()
{
    needsupdate = true;
    return d->coordsData();
}

/** Translate this CoordGroup2 by 'delta' */
CoordGroup2Editor& CoordGroup2Editor::translate(const Vector &delta)
{
    if (not delta.isZero())
    {
        //translate all of the coordinates
        quint32 ncoords = this->count();
        
        Vector *coords = d->coordsData();

        for (quint32 i=0; i<ncoords; ++i)
        {
            coords[i] += delta;
        }

        //now translate the AABox
        if (not needsupdate)
        {
            AABox new_box = this->aaBox();
            new_box.translate(delta);
            *(d->aaBox()) = new_box;
        }
    }

    return *this;
}

/** Translate the 'ith' point in the group by 'delta'

    \throw SireError::invalid_index
*/
CoordGroup2Editor& CoordGroup2Editor::translate(quint32 i, const Vector &delta)
{
    if (not delta.isZero())
        this->operator[](i) += delta;

    return *this;
}

/** Rotate (and perhaps scale and shear) this group by the matrix 'rotmat'
    about the point 'point' */
CoordGroup2Editor& CoordGroup2Editor::rotate(const Matrix &rotmat, const Vector &point)
{
    quint32 sz = this->count();
    Vector *coords = d->coordsData();

    for (quint32 i=0; i<sz; ++i)
    {
        coords[i] = SireMaths::rotate( coords[i], rotmat, point );
    }

    needsupdate = true;

    return *this;
}

/** Rotate the 'ith' point in the group using the matrix 'rotmat' about the
    point 'point'

    \throw SireError::index
*/
CoordGroup2Editor& CoordGroup2Editor::rotate(quint32 i, const Matrix &rotmat, 
                                           const Vector &point)
{
    Vector &coord = this->operator[](i);
    coord = SireMaths::rotate(coord, rotmat, point);

    return *this;
}

/** Rotate this group by the Quaternion 'quat' about the point 'point' */
CoordGroup2Editor& CoordGroup2Editor::rotate(const Quaternion &quat, 
                                             const Vector &point)
{
    return this->rotate(quat.toMatrix(), point);
}

/** Rotate the 'ith' point in the group using the quaternion 'quat' about the
    point 'point'

    \throw SireError::index
*/
CoordGroup2Editor& CoordGroup2Editor::rotate(quint32 i, const Quaternion &quat, 
                                           const Vector &point)
{
    return this->rotate(i, quat.toMatrix(), point);
}

/** Set the coordinates of the CoordGroup2 to 'newcoords' - this must
    have the same number of points as this CoordGroup2 or an exception
    will be thrown

    \throw SireError::incompatible_error
*/
CoordGroup2Editor& CoordGroup2Editor::setCoordinates(const QVector<Vector> &newcoords)
{
    CoordGroup2Base::assertSameSize(newcoords);

    int sz = this->count();
    Vector *coords = d->coordsData();
    const Vector *newcoords_array  = newcoords.constData();

    void *output = qMemCopy(coords, newcoords_array, sz*sizeof(Vector));
    needsupdate = true;

    BOOST_ASSERT(output == coords);

    return *this;
}

/** Set the coordinates of the ith atom to 'newcoords'

    \throw SireError::invalid_index
*/
CoordGroup2Editor& CoordGroup2Editor::setCoordinates(quint32 i, const Vector &newcoords)
{
    if (i >= quint32(this->count()))
        throw SireError::invalid_index( QObject::tr(
            "Cannot update the coordinates of point %1 as the number "
            "of points available is just %2.")
                .arg(i).arg(this->count()), CODELOC );
    
    Vector *coords = d->coordsData();
    coords[i] = newcoords;
    
    needsupdate = true;
    
    return *this;
}

/** Set the coordinates of the CoordGroup2 to 'newcoords' - this
    must have the same number of points as this CoordGroup2 or an
    exception will be thrown

    \throw SireError::incompatible_error
*/
CoordGroup2Editor& CoordGroup2Editor::setCoordinates(const CoordGroup2Base &newcoords)
{
    if ( this->constData() != newcoords.constData() )
    {
        CoordGroup2Base::assertSameSize(newcoords);

        int sz = this->count();
        Vector *coords = d->coordsData();
        const Vector *newcoords_array  = newcoords.constData();

        void *output = qMemCopy(coords, newcoords_array, sz*sizeof(Vector));
        needsupdate = true;

        BOOST_ASSERT(output == coords);
    }

    return *this;
}

/** Map all of these coordinates into the axis set 'axes' */
CoordGroup2Editor& CoordGroup2Editor::mapInto(const AxisSet &axes)
{
    int sz = this->count();
    
    Vector *coords = d->coordsData();
    
    for (int i=0; i<sz; ++i)
    {
        coords[i] = axes.fromIdentity(coords[i]);
    }
    
    needsupdate = true;
    
    return *this;
}

/** Map the coordinates of the ith point into the axis set 'axes'

    \throw SireError::invalid_index
*/
CoordGroup2Editor& CoordGroup2Editor::mapInto(quint32 i, const AxisSet &axes)
{
    if (i >= quint32(this->count()))
        throw SireError::invalid_index( QObject::tr(
            "Cannot update the coordinates of point %1 as the number "
            "of points available is just %2.")
                .arg(i).arg(this->count()), CODELOC );
    
    Vector *coords = d->coordsData();
    coords[i] = axes.fromIdentity(coords[i]);
    
    needsupdate = true;
    
    return *this;
}

/** Change the coordinate frame of all of the coordinates from
    'from_frame' to 'to_frame'. This has the same effect
    as 'mapInto' if 'from_frame' is the unit cartesian set */
CoordGroup2Editor& CoordGroup2Editor::changeFrame(const AxisSet &from_frame,
                                                const AxisSet &to_frame)
{
    int sz = this->count();
    
    Vector *coords = d->coordsData();

    //precalculate what we can...
    Matrix mat = from_frame.invMatrix() * to_frame.matrix();
    
    for (int i=0; i<sz; ++i)
    {
        coords[i] = (mat * (coords[i] - from_frame.origin())) + to_frame.origin();
    }
    
    needsupdate = true;
    
    return *this;
}

/** Map the coordinates of the ith point from the frame
    'from_frame' to the frame 'to_frame'

    \throw SireError::invalid_index
*/
CoordGroup2Editor& CoordGroup2Editor::changeFrame(quint32 i,
                                                const AxisSet &from_frame,
                                                const AxisSet &to_frame)
{
    if (i >= quint32(this->count()))
        throw SireError::invalid_index( QObject::tr(
            "Cannot update the coordinates of point %1 as the number "
            "of points available is just %2.")
                .arg(i).arg(this->count()), CODELOC );
    
    Vector *coords = d->coordsData();
    coords[i] = from_frame.toFrame(to_frame,coords[i]);
    
    needsupdate = true;
    
    return *this;
}

/** Return a CoordGroup2 which is a copy of this group. */
CoordGroup2 CoordGroup2Editor::commit()
{
    //return a copy of this CoordGroup2
    if (needsupdate)
    {
        *(d->aaBox()) = AABox(*this);
        needsupdate = false;
    }
    
    return CoordGroup2(*this);
}

////////
//////// Implementation of CoordGroupArray
////////

static CGSharedPtr<CGArrayData> getSharedNullCGArray()
{
    const CGSharedPtr<CGArrayArrayData> &array = ::getSharedNull();
    
    return CGSharedPtr<CGArrayData>( array->nullArray() );
}

/** Null constructor */
CoordGroupArray::CoordGroupArray()
                : d( ::getSharedNullCGArray() )
{}

/** Private constructor used only by CGArrayData */
CoordGroupArray::CoordGroupArray(CGArrayData *data)
{
    d.weakAssign(data);
}

static CGSharedPtr<CGArrayData> createCGArray(quint32 ngroups, quint32 ncoords)
{
    if (ngroups == 0)
        return ::getSharedNullCGArray();

    //construct space for 1 array of ngroups CoordGroups of ncoords coordinates
    char *storage = CGMemory::create(1, ngroups, ncoords);
        
    CGArrayArrayData *array = (CGArrayArrayData*)storage;
    
    return CGSharedPtr<CGArrayData>( &(array->cgArrayDataData()[0]) );
}

/** Construct from an array of CoordGroups */
CoordGroupArray::CoordGroupArray(const QVector<CoordGroup2> &cgroups)
{
    //count the number of groups and coordinates
    quint32 ncoords = 0;
    
    const CoordGroup2 *cgroups_array = cgroups.constData();
    
    for (int i=0; i<cgroups.count(); ++i)
    {
        ncoords += cgroups_array[i].count();
    }
    
    //now create space for the CoordGroups
    d = ::createCGArray(cgroups.count(), ncoords);
    
    if (cgroups.isEmpty())
        return;
    
    CGArrayArrayData *cgarrayarray = (CGArrayArrayData*)(d->memory());
    
    cgarrayarray->setNCGroupsInArray(0, cgroups.count());
    
    //tell the array about the sizes of the groups
    for (int i=0; i<cgroups.count(); ++i)
    {
        const CoordGroup2 &cgroup = cgroups_array[i];
        cgarrayarray->setNPointsInCGroup(i, cgroup.count());
    }
    
    cgarrayarray->close();
    
    Vector *coords = cgarrayarray->coordsData();
    AABox *aabox = cgarrayarray->aaBoxData();
    
    //finally, copy the data into the arrays
    for (int i=0; i<cgroups.count(); ++i)
    {
        const CoordGroup2 &cgroup = cgroups_array[i];
        
        //copy the coordinates
        void *output = qMemCopy(coords, cgroup.constData(),
                                cgroup.count() * sizeof(Vector));
                                
        BOOST_ASSERT(output == coords);
        
        coords += cgroup.count();
        
        //now copy the AABox
        aabox[i] = cgroup.aaBox();
    }
}

/** Copy constructor */
CoordGroupArray::CoordGroupArray(const CoordGroupArray &other)
                : d(other.d)
{}

/** Destructor */
CoordGroupArray::~CoordGroupArray()
{}

/** Assert that the index i points to a valid CoordGroup 

    \throw SireError::invalid_index
*/
void CoordGroupArray::assertValidCoordGroup(quint32 i) const
{
    if (i >= d->nCGroups())
        throw SireError::invalid_index( QObject::tr(
            "Cannot access the CoordGroup at index %1. The number "
            "of CoordGroups in this array is just %2.")
                .arg(i).arg(d->nCGroups()), CODELOC );
}

/** Assert that the index i points to a valid CoordGroup 

    \throw SireError::invalid_index
*/
void CoordGroupArray::assertValidIndex(quint32 i) const
{
    this->assertValidCoordGroup(i);
}

/** Assert that the index i points a valid coordinate

    \throw SireError::invalid_index
*/
void CoordGroupArray::assertValidCoordinate(quint32 i) const
{
    if (i >= d->nCoords())
        throw SireError::invalid_index( QObject::tr(
            "Cannot access the coordinate at index %1. The number "
            "of coordinates in this array is just %2.")
                .arg(i).arg(d->nCoords()), CODELOC );
}

/** Return a reference to the ith CoordGroup

    \throw SireError::invalid_index
*/
const CoordGroup2& CoordGroupArray::operator[](quint32 i) const
{
    assertValidCoordGroup(i);
    
    const CoordGroup2 *cgroup = d->cGroupData() + i;
    return *cgroup;
}

/** Return a reference to the ith CoordGroup

    \throw SireError::invalid_index
*/
const CoordGroup2& CoordGroupArray::at(quint32 i) const
{
    return this->operator[](i);
}

/** Return the number of CoordGroups in this array */
int CoordGroupArray::count() const
{
    return d->nCGroups();
}

/** Return the number of CoordGroups in this array */
int CoordGroupArray::size() const
{
    return d->nCGroups();
}

/** Return the number of CoordGroups in this array */
int CoordGroupArray::nCoordGroups() const
{
    return d->nCGroups();
}

/** Return the number of coordinates in this array */
int CoordGroupArray::nCoords() const
{
    return d->nCoords();
}

/** Return a raw pointer to the array of CoordGroups */
const CoordGroup2* CoordGroupArray::data() const
{
    return d->cGroupData();
}

/** Return a raw pointer to the array of CoordGroups */
const CoordGroup2* CoordGroupArray::constData() const
{
    return d->cGroupData();
}

/** Return a raw pointer to the array of all coordinates of all
    of the CoordGroups in this array */
const Vector* CoordGroupArray::coordsData() const
{
    return d->coordsData();
}

/** Return a raw pointer to the array of all coordinates of all
    of the CoordGroups in this array */
const Vector* CoordGroupArray::constCoordsData() const
{
    return d->coordsData();
}

/** Return a raw pointer to the array of all AABoxes for
    all of the CoordGroups in this array */
const AABox* CoordGroupArray::aaBoxData() const
{
    return d->aaBoxData();
}

/** Return a raw pointer to the array of all AABoxes for
    all of the CoordGroups in this array */
const AABox* CoordGroupArray::constAABoxData() const
{
    return d->aaBoxData();
}

/** Update the CoordGroup at index i so that it is equal to 'cgroup'. Note
    that 'cgroup' must contain the same number of coordinates as the existing
    CoordGroup at this index
    
    \throw SireError::invalid_index
    \throw SireError::incompatible_error
*/
void CoordGroupArray::update(quint32 i, const CoordGroup2 &cgroup)
{
    this->assertValidCoordGroup(i);
    
    CoordGroup2 &this_cgroup = d->cGroupData()[i];
    
    CGData *cgdata = const_cast<CGData*>( this_cgroup.d.constData() );
    
    if (cgdata->nCoords() != quint32(cgroup.count()))
        throw SireError::incompatible_error( QObject::tr(
            "Cannot update the CoordGroup at index %1 as this CoordGroup "
            "has a different number of coordinates (%2) than the "
            "new CoordGroup (%3).")
                .arg(i).arg(cgdata->nCoords()).arg(cgroup.count()),
                    CODELOC );
                    
    //copy the coordinates
    Vector *coords = cgdata->coordsData();
    
    void *output = qMemCopy(coords, cgroup.constData(),
                            cgdata->nCoords() * sizeof(Vector));
                            
    BOOST_ASSERT(output == coords);
    
    //copy the AABox
    *(cgdata->aaBox()) = cgroup.aaBox();
}

////////
//////// Implementation of CoordGroupArrayArray
////////
