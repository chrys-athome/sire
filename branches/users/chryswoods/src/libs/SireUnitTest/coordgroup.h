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

#include "SireVol/aabox.h"
#include "SireMaths/vector.h"

namespace SireMaths
{
class Quaternion;
class Matrix;
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
    CoordGroupBase();
    
    CoordGroupBase(quint32 size);
    CoordGroupBase(quint32 size, const Vector &value);
    
    CoordGroupBase(const QVector<Vector> &coordinates);
    
    CoordGroupBase(const CoordGroupBase &other);
    
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

    quint32 count() const;
    quint32 size() const;
    
    CoordGroupBase& operator=(const CoordGroupBase &other);

protected:
    const char* memory() const;
    char* memory();

    const char* constMemory() const;

    const detail::CoordGroupData& _pvt_group() const;
    detail::CoordGroupData& _pvt_group();
    
    const detail::CoordGroupData& _pvt_constGroup() const;
    
    const Vector* _pvt_data() const;
    Vector* _pvt_data();
    
    const Vector* _pvt_constData() const;

    void assertValidIndex(quint32 i) const;

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

}

//Q_DECLARE_METATYPE(SireVol::CoordGroup);

#endif
