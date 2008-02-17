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

#include "SireError/errors.h"

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
    
    const CGArrayData* cgArrayData() const;
    const CGData* cGroupData() const;
    const AABox* aaBoxData() const;
    const Vector* coordsData() const;
    
    CGArrayData* cgArrayData();
    CGData* cGroupData();
    AABox* aaBoxData();
    Vector* coordsData();
    
    void setNCGroupsInArray(quint32 i, quint32 ncgroups);
    void setNPointsInCGroup(quint32 i, quint32 npoints);
    
    void close();
    
    quint32 nCGArrays() const;
    quint32 nCGroups() const;
    quint32 nCoords() const;
    
    const char* constMemory() const;
    
private:
    const char* memory() const;
    char *memory();
    
    /** The index in the storage array of the first CoordGroupArray  
        in this array */
    quint32 cgarray0;
    
    /** The number of CoordGroupArrays in this array */
    quint32 ncgarrays;
    
    /** The index in the storage array of the first CoordGroup in 
        this array */
    quint32 cgroup0;
    
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
    
    const CGData* cGroupData() const;
    const AABox* aaBoxData() const;
    const Vector* coordsData() const;
    
    const CGData* constCGroupData() const;
    const AABox* constAABoxData() const;
    const Vector* constCoordsData() const;
    
    CGData* cGroupData();
    AABox* aaBoxData();
    Vector* coordsData();
    
    quint32 nCGroups() const;
    quint32 nCoords() const;
    
    const char* constMemory() const;
    
private:
    const char* memory() const;
    char* memory();

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
    
    const Vector* coordsData() const;
    const AABox* aaBox() const;
    
    Vector* coordsData();
    AABox* aaBox();
    
    quint32 nCoords() const;
    
    const char* constMemory() const;
    
private:
    const char* memory() const;
    char* memory();

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
    
    static const char* constMemory(const char *this_ptr, quint32 this_idx);
    static char* memory(char *this_ptr, quint32 this_idx);

    static void incref(char *this_ptr, quint32 this_idx);
    static void decref(char *this_ptr, quint32 this_idx);

private:
    static char* getRoot(char *this_ptr, quint32 this_idx);
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
    return sizeof(CGArrayArrayData) + 
                  narrays * sizeof(CGArrayData) + 
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
    //  B = Array of CGArrayData objects  ( narrays * sizeof(CGArrayData) )
    //  C = Array of CGData objects       ( ncgroups * sizeof(CGData) )
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
                    
        //this is the location of the first CGArrayData :-)
        cgarrayarray->cgarray0 = idx;
                    
        //loop over each CGArray and create it in its place
        for (quint32 i=0; i<narrays; ++i)
        {
            //assert that there is sufficient space in the array
            BOOST_ASSERT(idx + sizeof(CGArrayData) < sz);
            
            //create the CGArrayData, letting it know where it
            //is relative to the beginning of the storage array
            new (storage + idx) CGArrayData(idx);
        
            //advance the index into the storage array to point
            //just after the just-created CGArrayData
            idx += sizeof(CGArrayData);
        }
        
        //we are now at the location of the first CGData
        cgarrayarray->cgroup0 = idx;
        
        //loop over each CoordGroup and create it in its place
        for (quint32 i=0; i<ncgroups; ++i)
        {
            //assert that there is sufficient space in the array
            BOOST_ASSERT(idx + sizeof(CGData) < sz);
            
            //create the CGData, letting it know where it is
            //relative to the beginning of the storage array
            new (storage + idx) CGData(idx);
            
            //advance the index into the storage array to point
            //just after the just-created CGData
            idx += sizeof(CGData); 
        }
        
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
            BOOST_ASSERT(idx + sizeof(Vector) < sz);

            new (storage + idx) Vector(0.0);
                
            idx += sizeof(Vector);
        }
        
        //we should now be at the end of the storage
        BOOST_ASSERT( idx + 1 == sz );
        
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

/** Private function - this converts the pointer to the object
    that starts at 'this_ptr' to a pointer to the first element
    of the storage array (given the location of the object at
    index 'this_idx' in the storage array) */
char* CGMemory::getRoot(char *this_ptr, quint32 this_idx)
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
        Vector *coords = (Vector*) (storage + array->coords0);
        
        for (quint32 i=ncoords-1; i>=0; --i)
        {
            coords[i].~Vector();
        }
    }
        
    //now delete all of the AABoxes
    quint32 ncgroups = array->nCGroups();
        
    if (ncgroups > 0)
    {
        AABox *aaboxes = (AABox*) (storage + array->aabox0);
            
        for (quint32 i=ncgroups-1; i>=0; --i)
        {
            aaboxes[i].~AABox();
        }
        
        //now delete all of the CGData objects
        CGData *cgroups = (CGData*) (storage + array->cgroup0);
            
        for (quint32 i=ncgroups-1; i>=0; --i)
        {
            cgroups[i].~CGData();
        }
    }
        
    //now delete all of the CGArrayData objects
    quint32 narrays = array->nCGArrays();
        
    if (narrays > 0)
    {
        CGArrayData *arrays = (CGArrayData*) (storage + array->cgarray0);
            
        for (quint32 i=narrays-1; i>=0; --i)
        {
            arrays[i].~CGArrayData();
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
        
        //return a pointer to the clone
        return new_storage + this_idx;
    }
    else
        //only one reference, so no need to clone
        return this_ptr;
}

/** Return a const pointer to the first element of the storage array */
const char* CGMemory::constMemory(const char *this_ptr, quint32 this_idx)
{
    return this_ptr - this_idx;
}

/** Return a modifiable pointer to the first element of the storage
    array - this detachs from implicitly shared storage */
char* CGMemory::memory(char *this_ptr, quint32 this_idx)
{
    this_ptr = CGMemory::detach(this_ptr, this_idx);
    return this_ptr - this_idx;
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
                   cgarray0(0), ncgarrays(0),
                   cgroup0(0), aabox0(0), ncgroups(0),
                   coords0(0), ncoords(0)
{}

/** Construct metadata for the specified number of subgroups */
CGArrayArrayData::CGArrayArrayData(quint32 narray, quint32 ngroup, 
                                   quint32 ncoord)
                 : QSharedData(),
                   cgarray0(0), ncgarrays(narray),
                   cgroup0(0), aabox0(0), ncgroups(ngroup),
                   coords0(0), ncoords(ncoord)
{}
  
/** Copy constructor */               
CGArrayArrayData::CGArrayArrayData(const CGArrayArrayData &other)
                 : QSharedData(),
                   cgarray0(other.cgarray0), ncgarrays(other.ncgarrays),
                   cgroup0(other.cgroup0), aabox0(other.aabox0),
                   ncgroups(other.ncgroups),
                   coords0(other.coords0), ncoords(other.ncoords)
{}

/** Destructor */
CGArrayArrayData::~CGArrayArrayData()
{}

/** Return a pointer to the start of the memory storage for this container */
const char* CGArrayArrayData::memory() const
{
    return CGMemory::constMemory( (const char*)this, 0 );
}

/** Return a modifiable pointer to the start of the memory storage
    for this container - this detaches this container from implicitly
    shared storage */
char* CGArrayArrayData::memory()
{
    return CGMemory::memory( (char*)this, 0 );
}

/** Return a pointer to the start of the memory storage for this container */
const char* CGArrayArrayData::constMemory() const
{
    return CGArrayArrayData::memory();
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

/** Return a pointer to the first CGArrayData in this container. This returns
    0 if there are no arrays in this container */
const CGArrayData* CGArrayArrayData::cgArrayData() const
{
    if (cgarray0 == 0)
        return 0;
    else
        return (const CGArrayData*)( memory() + cgarray0 );
}

/** Return a pointer to the first CGData in this container. This returns
    0 if there are no CoordGroups in this container */
const CGData* CGArrayArrayData::cGroupData() const
{
    if (cgroup0 == 0)
        return 0;
    else
        return (const CGData*)( memory() + cgroup0 );
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

/** Return a pointer to the first CGArrayData in this container. This returns
    0 if there are no arrays in this container */
CGArrayData* CGArrayArrayData::cgArrayData()
{
    if (cgarray0 == 0)
        return 0;
    else
        return (CGArrayData*)( memory() + cgarray0 );
}

/** Return a pointer to the first CGData in this container. This returns
    0 if there are no CoordGroups in this container */
CGData* CGArrayArrayData::cGroupData()
{
    if (cgroup0 == 0)
        return 0;
    else
        return (CGData*)( memory() + cgroup0 );
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
    this->cgArrayData()[i].ncgroups = ncgroups;
}

/** Set the number of coordinates in the ith CoordGroup */
void CGArrayArrayData::setNPointsInCGroup(quint32 i, quint32 ncoords)
{
    BOOST_ASSERT( i < ncgroups );
    this->cGroupData()[i].ncoords = ncoords;
}

/** Close this array - call this once you have finished constructing
    the data */
void CGArrayArrayData::close()
{
    CGArrayData *cgarrays = this->cgArrayData();
    CGData *cgroups = this->cGroupData();
    
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
            cgarray.cgroup0 = cgroup0 + n_assigned_cgroups * sizeof(CGData);
        
            //tell the array where its first AABox is located
            cgarray.aabox0 = aabox0 + n_assigned_cgroups * sizeof(AABox);
        
            //sum up the number of coordinates in the CoordGroups in this array
            quint32 ncgcoords = 0;
            const CGData *array_groups = cgarray.constCGroupData();
        
            for (quint32 j=0; j<cgarray.nCGroups(); ++i)
            {
                ncgcoords += array_groups[j].nCoords();
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
            : this_cgarray(other.this_cgarray), cgroup0(other.cgroup0), 
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
    return CGMemory::constMemory( (const char*)this, this_cgarray );
}

/** Return a pointer to the start of the raw memory
    that holds all of the data of everything in the container.
    This will detach from shared storage */
char* CGArrayData::memory()
{
    return CGMemory::memory( (char*)this, this_cgarray );
}

/** Return a const pointer to the start of the raw memory
    that holds all of the data of everything in the container */
const char* CGArrayData::constMemory() const
{
    return CGArrayData::memory();
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

/** Return a pointer to the CGData of the first CoordGroup in this array.
    This returns 0 if there are no CoordGroups in this array */
const CGData* CGArrayData::cGroupData() const
{
    if (cgroup0 == 0)
        return 0;
    else
        return (const CGData*)( memory() + cgroup0 );
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

/** Return a pointer to the CGData of the first CoordGroup in this array.
    This returns 0 if there are no CoordGroups in this array */
CGData* CGArrayData::cGroupData()
{
    if (cgroup0 == 0)
        return 0;
    else
        return (CGData*)( memory() + cgroup0 );
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

/** Return a const pointer to the start of the storage
    array that contains this CGData */
const char* CGData::memory() const
{
    return CGMemory::constMemory( (const char*)this, this_cgroup );
}

/** Return a pointer to the start of the storage array that
    contains this CGData - this will detach from shared storage */
char* CGData::memory()
{
    return CGMemory::memory( (char*)this, this_cgroup );
}

/** Return a const pointer to the start of the raw memory
    that holds all of the data of everything in the container */
const char* CGData::constMemory() const
{
    return CGData::memory();
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
//////// Implementation of CoordGroupArrayArray
////////

////////
//////// Implementation of CoordGroupArray
////////

////////
//////// Implementation of CoordGroupBase
////////

/** Null constructor */
CoordGroup2Base::CoordGroup2Base()
               : d( CGMemory::getSharedNullCoordGroup() )
{}

static CGData* createCoordGroup(quint32 size)
{
    if (size == 0)
        return CGMemory::getSharedNullCoordGroup();

    //construct space for 1 array of 1 CoordGroup of size atoms
    char *storage = CGMemory::create(1, 1, size);
        
    CGArrayArrayData *array = (CGArrayArrayData*)storage;
        
    array->setNCGroupsInArray(0, 1);
    array->setNPointsInCGroup(0, size);
        
    //close the array - this validates that it is complete
    array->close();
    
    return &(array->cGroupData()[0]);
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
{
    //increment the reference count of the data
    d->incref();
}

/** Destructor - this will only delete the data if this
    is the only reference to it */
CoordGroup2Base::~CoordGroup2Base()
{
    d->decref();
}

/** Copy assignment operator - CoordGroup2Base is implicitly
    shared so this will be fast */
CoordGroup2Base& CoordGroup2Base::operator=(const CoordGroup2Base &other)
{
    if (d->constMemory() == other.d->constMemory())
    {
        //these are both pointing to CoordGroups in the same
        //array - just update this pointer (there is no need
        //to worry about reference counts)
        d = other.d;
    }
    else
    {
        //increase the reference count of 'other'
        CGData *d_copy = other.d;
        d_copy->incref();
        
        //now decrease the reference count of whatever we are
        //pointing to
        d->decref();
        
        //copy the pointer
        d = other.d;
    }
    
    return *this;
}

/** Comparison operator */
bool CoordGroup2Base::operator==(const CoordGroup2Base &other) const
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
    return _pvt_data()[i];
}

/** Return the 'ith' coordinate of this group

    \throw SireError::invalid_index
*/
const Vector& CoordGroup2Base::operator[](quint32 i) const
{
    assertValidIndex(i);
    return _pvt_data()[i];
}

/** Set that the AABox needs to be updated */
void CoordGroup2Base::setNeedsUpdate()
{
    _pvt_group().setNeedsUpdate();
}

/** Update the AABox with the current coordinates */
void CoordGroup2Base::update()
{
    _pvt_group().update( AABox(*this) );
}


/** Return a pointer to the raw memory in which the
    CoordGroupData and array of coordinates are placed */
inline const char* CoordGroup2Base::memory() const
{
    return storage;
}

/** Return a pointer to the raw memory in which the
    CoordGroupData and array of coordinates are placed */
inline char* CoordGroup2Base::memory()
{
    detach();
    return storage;
}

/** Return a pointer to the raw memory in which the
    CoordGroupData and array of coordinates are placed */
inline const char* CoordGroup2Base::constMemory() const
{
    return storage;
}

/** Return a reference to the CoordGroupData that contains the
    metadata about this group */
inline const detail::CoordGroupData& CoordGroup2Base::_pvt_group() const
{
    return *( (detail::CoordGroupData*)(memory()) );
}

/** Return whether this group is empty (has no coordinates) */
inline bool CoordGroup2Base::isEmpty() const
{
    return _pvt_group().isEmpty();
}

/** Return a raw pointer to the array of coordinates */
inline const Vector* CoordGroup2Base::_pvt_data() const
{
    if (not isEmpty())
        return (Vector*)( constMemory() + sizeof(detail::CoordGroupData) );
    else
        return 0;
}

/** Return a modifiable raw pointer to the array of
    coordinates */
inline Vector* CoordGroup2Base::_pvt_data()
{
    if (not isEmpty())
        return (Vector*)( memory() + sizeof(detail::CoordGroupData) );
    else
        return 0;
}

/** Return a raw pointer to the array of coordinates */
inline const Vector* CoordGroup2Base::_pvt_constData() const
{
    return _pvt_data();
}

/** Return whether or not this group needs to be updated */
inline bool CoordGroup2Base::needsUpdate() const
{
    return _pvt_group().needsUpdate();
}

/** Return the enclosing AABox */
inline const AABox& CoordGroup2Base::aaBox() const
{
    return _pvt_group().aaBox();
}

/** Return a raw pointer to the array containing all
    of the coordinates */
inline const Vector* CoordGroup2Base::constData() const
{
    return _pvt_data();
}

/** Return a raw pointer to the array containing all
    of the coordinates */
inline const Vector* CoordGroup2Base::data() const
{
    return _pvt_data();
}

/** Return the number of coordinates in this group */
inline int CoordGroup2Base::count() const
{
    return _pvt_group().count();
}

/** Return the number of coordinates in this group */
inline int CoordGroup2Base::size() const
{
    return _pvt_group().count();
}

////////
//////// Implementation of CoordGroup
////////

////////
//////// Implementation of CoordGroupEditor
////////
