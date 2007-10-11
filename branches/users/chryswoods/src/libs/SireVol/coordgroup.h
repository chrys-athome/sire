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

#ifndef SIREVOL_COORDGROUP_H
#define SIREVOL_COORDGROUP_H

#include <QSharedData>
#include <QVector>

#include "aabox.h"

#include "SireMaths/vector.h"

namespace SireMaths
{
class Quaternion;
class Matrix;
class AxisSet;
}

namespace SireVol
{

namespace detail
{
class CoordGroupData;
}

class CoordGroupBase;
class CoordGroup;
}

SIRE_BEGIN_HEADER

QDataStream& operator<<(QDataStream&, const SireVol::CoordGroup&);
QDataStream& operator>>(QDataStream&, SireVol::CoordGroup&);

namespace SireVol
{

using SireMaths::Vector;
using SireMaths::Quaternion;
using SireMaths::Matrix;

class CoordGroupEditor;

namespace detail
{

/** This class holds the data about the CoordGroup
    (the AABox enclosing all of the coordinates and
     the number of coordinates in the group)

    @author Christopher Woods
*/
class SIREVOL_EXPORT CoordGroupData : public QSharedData
{
public:
    CoordGroupData();
    CoordGroupData(quint32 size);

    CoordGroupData(const CoordGroupData &other);

    ~CoordGroupData();

    CoordGroupData& operator=(const CoordGroupData &other);

    bool operator==(const CoordGroupData &other) const;
    bool operator!=(const CoordGroupData &other) const;

    bool isEmpty() const;

    quint32 count() const;

    const AABox& aaBox() const;

    bool needsUpdate() const;

    void setNeedsUpdate();
    void update(const AABox &new_aabox);

    static CoordGroupData* getSharedNull();

private:
    /** Pointer to the global null CoordGroupData */
    static CoordGroupData *shared_null;

    /** axis-aligned box that completely encloses all of
        the points in this group */
    AABox aabox;

    /** The number of coordinates in this group */
    quint32 sz;

    /** Whether or not the AABox for this group needs
        to be updated */
    bool needsupdate;
};

/** Return whether or not this is empty */
inline bool CoordGroupData::isEmpty() const
{
    return sz == 0;
}

/** Return the number of coordinates in the group */
inline quint32 CoordGroupData::count() const
{
    return sz;
}

/** Return the AABox that encloses the coordinates - you should
    always ensure that the box is up-to-date (e.g. 'update()' has
    been called after any editing of the coordinates) */
inline const AABox& CoordGroupData::aaBox() const
{
    return aabox;
}

/** Return whether or not the AABox that encloses all of the points
    needs to be updated. */
inline bool CoordGroupData::needsUpdate() const
{
    return needsupdate;
}

/** Require that the AABox that encloses all of the points
    needs to be updated */
inline void CoordGroupData::setNeedsUpdate()
{
    needsupdate = true;
}

/** Update the AABox that encloses all of the points
    to equal new_aabox */
inline void CoordGroupData::update(const AABox &new_aabox)
{
    aabox = new_aabox;
    needsupdate = false;
}

} // end of namespace detail

/** This is the base class of all CoordGroup-like classes
    (e.g. CoordGroup and CoordGroupEditor). CoordGroup classes
    hold a group of coordinates, together with an axis-aligned
    box that completely encloses all of those points. The
    class is implicitly shared, and, since it is used in the
    most performance-sensitive parts of the code, has
    a slightly more complex implicit-sharing design.

    Instead of CoordGroup holding a pointer to CoordGroupData,
    which would then hold a pointer to the array of coordinates
    in the group, CoordGroup is designed to create CoordGroupData
    and the array of coordinates in the same block of memory
    (using placement new) - CoordGroupData is placed first in
    the memory block, then the array of coordinates. CoordGroup
    then holds just a pointer to the first coordinate in the array.
    This allows rapid lookup of coordinates in the group, as
    it is the same as a normal array lookup (and not a pointer
    dereference plus array lookup, as is the case in the
    first described design). A pointer to the CoordGroupData object
    can be obtained from the array pointer by subtracting the
    size of CoordGroupData.

    @author Christopher Woods
*/
class SIREVOL_EXPORT CoordGroupBase
{

public:
    ~CoordGroupBase();

    bool operator==(const CoordGroupBase &other) const;
    bool operator!=(const CoordGroupBase &other) const;

    bool maybeDifferent(const CoordGroupBase &other) const;

    const Vector& at(quint32 i) const;
    const Vector& operator[](quint32 i) const;

    const AABox& aaBox() const;

    const Vector* constData() const;
    const Vector* data() const;

    bool isEmpty() const;

    int count() const;
    int size() const;

    void assertValidIndex(quint32 i) const;

    void assertSameSize(const QVector<Vector> &coordinates) const;
    void assertSameSize(const CoordGroupBase &other) const;

protected:
    CoordGroupBase();

    CoordGroupBase(quint32 size, const Vector &value = Vector());
    CoordGroupBase(quint32 size, const Vector *values);

    CoordGroupBase(const QVector<Vector> &coordinates);

    CoordGroupBase(const CoordGroupBase &other);

    CoordGroupBase& operator=(const CoordGroupBase &other);

    void setNeedsUpdate();
    bool needsUpdate() const;
    void update();

    const char* memory() const;
    char* memory();

    const char* constMemory() const;

    const detail::CoordGroupData& _pvt_group() const;
    detail::CoordGroupData& _pvt_group();

    const detail::CoordGroupData& _pvt_constGroup() const;

    const Vector* _pvt_data() const;
    Vector* _pvt_data();

    const Vector* _pvt_constData() const;

private:
    void detach();

    static void destroy(detail::CoordGroupData *group_ptr);

    /** Pointer to the memory in which the
        CoordGroupData and array of coordinates
        are placed */
    char *storage;
};

/** Return a pointer to the raw memory in which the
    CoordGroupData and array of coordinates are placed */
inline const char* CoordGroupBase::memory() const
{
    return storage;
}

/** Return a pointer to the raw memory in which the
    CoordGroupData and array of coordinates are placed */
inline char* CoordGroupBase::memory()
{
    detach();
    return storage;
}

/** Return a pointer to the raw memory in which the
    CoordGroupData and array of coordinates are placed */
inline const char* CoordGroupBase::constMemory() const
{
    return storage;
}

/** Return a reference to the CoordGroupData that contains the
    metadata about this group */
inline const detail::CoordGroupData& CoordGroupBase::_pvt_group() const
{
    return *( (detail::CoordGroupData*)(memory()) );
}

/** Return whether this group is empty (has no coordinates) */
inline bool CoordGroupBase::isEmpty() const
{
    return _pvt_group().isEmpty();
}

/** Return a raw pointer to the array of coordinates */
inline const Vector* CoordGroupBase::_pvt_data() const
{
    if (not isEmpty())
        return (Vector*)( constMemory() + sizeof(detail::CoordGroupData) );
    else
        return 0;
}

/** Return a modifiable raw pointer to the array of
    coordinates */
inline Vector* CoordGroupBase::_pvt_data()
{
    if (not isEmpty())
        return (Vector*)( memory() + sizeof(detail::CoordGroupData) );
    else
        return 0;
}

/** Return a raw pointer to the array of coordinates */
inline const Vector* CoordGroupBase::_pvt_constData() const
{
    return _pvt_data();
}

/** Return whether or not this group needs to be updated */
inline bool CoordGroupBase::needsUpdate() const
{
    return _pvt_group().needsUpdate();
}

/** Return the enclosing AABox */
inline const AABox& CoordGroupBase::aaBox() const
{
    return _pvt_group().aaBox();
}

/** Return a raw pointer to the array containing all
    of the coordinates */
inline const Vector* CoordGroupBase::constData() const
{
    return _pvt_data();
}

/** Return a raw pointer to the array containing all
    of the coordinates */
inline const Vector* CoordGroupBase::data() const
{
    return _pvt_data();
}

/** Return the number of coordinates in this group */
inline int CoordGroupBase::count() const
{
    return _pvt_group().count();
}

/** Return the number of coordinates in this group */
inline int CoordGroupBase::size() const
{
    return _pvt_group().count();
}

/**
This class holds a group of coordinates. This group forms the basis of the
Molecular CutGroup, as defined in SireMol. A CoordGroup contains a list of
coordinates, together with an AABox which provides information as to the
center and extents of this group. SireVol is designed to calculate distances
between points in different CoordGroups, or to calculate distances between
points within a CoordGroup. A CoordGroup is implicitly shared and is
designed to be fast to use, and fast to copy.

@author Christopher Woods
*/
class SIREVOL_EXPORT CoordGroup : public CoordGroupBase
{

friend QDataStream& ::operator<<(QDataStream&, const CoordGroup&);
friend QDataStream& ::operator>>(QDataStream&, CoordGroup&);

public:
    CoordGroup();
    CoordGroup(quint32 size);
    CoordGroup(quint32 size, const Vector &value);
    CoordGroup(quint32 size, const Vector *values);
    CoordGroup(const QVector<Vector> &points);

    CoordGroup(const CoordGroupBase &other);
    CoordGroup(const CoordGroup &other);

    ~CoordGroup();

    CoordGroup& operator=(const CoordGroup &other);
    CoordGroup& operator=(const CoordGroupEditor &other);

    CoordGroupEditor edit() const;

    template<class T>
    static CoordGroup combine(const T &groups);

    template<class T>
    static T split(const CoordGroup &group, const T &groups);

private:
    static void throwInvalidCountError(uint nats0, uint nats1);
};

#ifndef SKIP_TEMPLATE_DEFINITIONS

/** Combine a whole load of CoordGroups into a single CoordGroup. */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
CoordGroup CoordGroup::combine(const T &groups)
{
    int ncg = groups.count();

    if (ncg == 0)
    {
        return CoordGroup();
    }
    else if (ncg == 1)
    {
        return groups.first();
    }
    else
    {
        //we have at least two groups to combine together! - calculate the
        //total number of points in all of the groups
        int ntotal = 0;

        for (typename T::const_iterator it = groups.constBegin();
             it != groups.constEnd();
             ++it)
        {
            ntotal += it->count();
        }

        if (ntotal == 0)
            //there are no points at all - return a null CoordGroup
            return CoordGroup();

        //create an array to hold that many points
        CoordGroup combined_group(ntotal);

        //get a pointer to the array of points
        Vector *combined_array = combined_group._pvt_data();

        //now go through each CoordGroup and copy their coordinates into
        //the new array, and combine together their AABoxes...
        AABox combined_box;

        int i = 0;

        for (typename T::const_iterator it = groups.constBegin();
             it != groups.constEnd();
             ++it)
        {
            int npoints = it->count();
            const Vector *group_array = it->constData();

            //copy the coordinates
            qMemCopy( &(combined_array[i]), group_array,
                      npoints*sizeof(Vector) );

            i += npoints;

            //combine the bounding boxes
            combined_box += it->aaBox();
        }

        combined_group._pvt_group().update(combined_box);

        return combined_group;
    }
}

/** Split a single CoordGroup into lots of little CoordGroups, split
    up into parts according to 'groups' - this function does the reverse
    operation of 'combine', e.g. you can write;

    \code
    //combine 'groups' into a single CoordGroup
    CoordGroup group = CoordGroup::combine(groups);

    //do stuff with the single CoordGroup

    //now split this back into its original groups
    groups = CoordGroup::split(group, groups);
    \endcode
*/
template<class T>
T CoordGroup::split(const CoordGroup &group, const T &groups)
{
    T new_groups = groups;

    //ensure we have the same number of atoms...
    uint nats = 0;

    foreach (const CoordGroup &cgroup, groups)
    {
        nats += cgroup.count();
    }

    if (group.count() != nats)
        throwInvalidCountError(group.count(), nats);

    if (nats == 0)
    {
        return new_groups;
    }
    else if (groups.count() == 1)
    {
        new_groups[0] = group;
        return new_groups;
    }
    else
    {
        //there is more than one CoordGroup - we need to combine them together
        int ngroups = groups.count();

        const Vector *coords = group.constData();

        for (int i=0; i<ngroups; ++i)
        {
            CoordGroup &cgroup = new_groups[i];

            quint32 nats = cgroup.count();

            cgroup = CoordGroup(nats, coords);
            coords += nats;
        }

        return new_groups;
    }
}

#endif // #ifndef SKIP_TEMPLATE_DEFINITIONS

/**
This class is used to edit a CoordGroup. This class is used when you want to
make several small changes to a CoordGroup, but do not want the CoordGroup to
update its internal state after each change (e.g. you are moving each point in
turn, and do not want the AABox to be updated for every step!)

You use a CoordGroupEditor like this;

\code

//create a CoordGroup with space for 100 coordinates
CoordGroup coordgroup(100);

//create an editor for this group
CoordGroupEditor editor = coordgroup.edit();

//manipulate each coordinate in turn
for (int i=0; i<100; ++i)
    editor[i] = Vector(i,i+1,i+2);

//commit the changes
coordgroup = editor.commit();

\endcode

@author Christopher Woods
*/
class SIREVOL_EXPORT CoordGroupEditor : public CoordGroupBase
{
public:
    CoordGroupEditor();
    CoordGroupEditor(const CoordGroupBase &other);

    ~CoordGroupEditor();

    CoordGroupEditor& operator=(const CoordGroupBase &other);

    Vector& operator[](quint32 i);

    Vector* data();

    CoordGroupEditor& translate(const Vector &delta);
    CoordGroupEditor& translate(quint32 i, const Vector &delta);

    CoordGroupEditor& rotate(const Quaternion &quat, const Vector &point);
    CoordGroupEditor& rotate(const Matrix &rotmat, const Vector &point);

    CoordGroupEditor& rotate(quint32 i, const Quaternion &quat, const Vector &point);
    CoordGroupEditor& rotate(quint32 i, const Matrix &rotmat, const Vector &point);

    CoordGroupEditor& setCoordinates(const QVector<Vector> &newcoords);
    CoordGroupEditor& setCoordinates(const CoordGroupBase &newcoords);

    CoordGroupEditor& setCoordinates(quint32 i, const Vector &newcoords);

    CoordGroupEditor& mapInto(const SireMaths::AxisSet &axes);

    CoordGroup commit();
};

}

Q_DECLARE_METATYPE(SireVol::CoordGroup);

SIRE_END_HEADER

#endif
