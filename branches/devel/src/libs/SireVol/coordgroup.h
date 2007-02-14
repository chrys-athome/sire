/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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

#include <QSharedDataPointer>

#include <boost/scoped_array.hpp>

#include "SireMaths/vector.h"

#include "aabox.h"

SIRE_BEGIN_HEADER

namespace SireMaths
{
class Quaternion;
class Matrix;
}

namespace SireVol
{
class CoordGroupBase;
class CoordGroup;
class CoordGroupPvt;
}

QDataStream& operator<<(QDataStream&, const SireVol::CoordGroup&);
QDataStream& operator>>(QDataStream&, SireVol::CoordGroup&);

QDataStream& operator<<(QDataStream&, const SireVol::CoordGroupPvt&);
QDataStream& operator>>(QDataStream&, SireVol::CoordGroupPvt&);

namespace SireVol
{

using SireMaths::Vector;
using SireMaths::Quaternion;
using SireMaths::Matrix;

class CoordGroupEditor;

/** Private class which holds the data for CoordGroup and CoordGroupEditor

    @author Christopher Woods
*/
class SIREVOL_EXPORT CoordGroupPvt : public QSharedData
{

friend QDataStream& ::operator<<(QDataStream&, const CoordGroupPvt&);
friend QDataStream& ::operator>>(QDataStream&, CoordGroupPvt&);

public:
    CoordGroupPvt();
    CoordGroupPvt(int size);
    CoordGroupPvt(int size, const Vector &value);
    CoordGroupPvt(const QVector<Vector> &coords);

    CoordGroupPvt(const CoordGroupPvt &other);

    ~CoordGroupPvt();

    CoordGroupPvt& operator=(const CoordGroupPvt &other);

    bool operator==(const CoordGroupPvt &other) const;
    bool operator!=(const CoordGroupPvt &other) const;

    const Vector& at(int i) const;

    const Vector& operator[](int i) const;
    Vector& operator[](int i);

    bool isEmpty() const;

    int count() const;
    int size() const;

    const AABox& aaBox() const;
    AABox& aaBox();

    const Vector* constData() const;
    const Vector* data() const;
    Vector* data();

    void translate(const Vector &delta);
    void translate(int i, const Vector &delta);

    void rotate(const Quaternion &quat, const Vector &point);
    void rotate(const Matrix &rotmat, const Vector &point);

    void rotate(int i, const Quaternion &quat, const Vector &point);
    void rotate(int i, const Matrix &rotmat, const Vector &point);

    bool needsUpdate() const;
    void update();

    void setCoordinates(const QVector<Vector> &newcoords);

    void assertSameSize(const QVector<Vector> &newcoords) const;
    void assertSameSize(const CoordGroupBase &newcoords) const;

private:
    void checkIndex(int i) const;

    void assertSameSize(int n) const;

    /** Pointer to the array of coordinates */
    boost::scoped_array<Vector> coords;
    /** The number of coordinates in the group */
    qint32 sz;

    /** Whether or not the AABox is in sync with the coordinates */
    bool needsupdate;

    /** The AABox which should completely enclose all of the points */
    AABox aabox;
};

/** Return the number of coordinates in the group */
inline int CoordGroupPvt::count() const
{
    return sz;
}

/** Return the number of coordinates in the group */
inline int CoordGroupPvt::size() const
{
    return sz;
}

/** Return the AABox that encloses the coordinates - you should
    always ensure that the box is up-to-date (e.g. 'update()' has
    been called after any editing of the coordinates) */
inline const AABox& CoordGroupPvt::aaBox() const
{
    return aabox;
}

/** Return a modifiable reference to the AABox - if you change the box
    yourself then you must be *very sure* that it exactly encloses the
    points in the group. */
inline AABox& CoordGroupPvt::aaBox()
{
    return aabox;
}

/** Return a const-pointer to the array of coordinates */
inline const Vector* CoordGroupPvt::constData() const
{
    return coords.get();
}

/**
This is the common base class of CoordGroup and CoordGroupEditor, thus allowing both classes to exchange information with one another.

@author Christopher Woods
*/
class SIREVOL_EXPORT CoordGroupBase
{
public:
    CoordGroupBase();
    CoordGroupBase(int size);
    CoordGroupBase(int size, const Vector &value);
    CoordGroupBase(const QVector<Vector> &coords);

    CoordGroupBase(const CoordGroupBase &other);

    ~CoordGroupBase();

    bool operator==(const CoordGroupBase &other) const;
    bool operator!=(const CoordGroupBase &other) const;

    const Vector& at(int i) const;
    const Vector& operator[](int i) const;

    const AABox& aaBox() const;

    const Vector* constData() const;
    const Vector* data() const;

    bool isEmpty() const;

    int count() const;
    int size() const;

protected:
    CoordGroupBase& operator=(const CoordGroupBase &other);

    /** Implicitly shared pointer to the coordinate data */
    QSharedDataPointer<CoordGroupPvt> d;
};

/** Return the number of coordinates in this group */
inline int CoordGroupBase::count() const
{
    return d->count();
}

/** Return the number of coordinates in this group */
inline int CoordGroupBase::size() const
{
    return d->size();
}

/** Return the AABox which is guaranteed to always exactly enclose
    the coordinates in this group */
inline const AABox& CoordGroupBase::aaBox() const
{
    return d->aaBox();
}

/** Return a const-pointer to the array holding the coordinates in
    this group. */
inline const Vector* CoordGroupBase::constData() const
{
    return d->constData();
}

/** Return the 'ith' coordinate in the group - this will throw an exception
    if 'i' refers to an invalid index

    \throw SireError::invalid_index
*/
inline const Vector& CoordGroupBase::at(int i) const
{
    return d->at(i);
}

/** Return the 'ith' coordinate in the group - this will throw an exception
    if 'i' refers to an invalid index

    \throw SireError::invalid_index
*/
inline const Vector& CoordGroupBase::operator[](int i) const
{
    return d->operator[](i);
}

/** Return whether or not this is an empty CoordGroup */
inline bool CoordGroupBase::isEmpty() const
{
    return d->isEmpty();
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
    CoordGroup(int size);
    CoordGroup(int size, const Vector &value);
    CoordGroup(const QVector<Vector> &points);

    CoordGroup(const CoordGroupBase &other);

    ~CoordGroup();

    CoordGroup& operator=(const CoordGroup &other);

    CoordGroupEditor edit() const;

    template<class T>
    static CoordGroup combine(const T &groups);
};

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
        QSharedDataPointer<CoordGroupPvt> d( new CoordGroupPvt(ntotal) );

        //get a pointer to the array of points
        Vector *combined_array = d->data();

        //now go through each CoordGroup and copy their coordinates into
        //the new array, and combine together their AABoxes...
        AABox &combined_box = d->aaBox();

        int i = 0;

        for (typename T::const_iterator it = groups.constBegin();
             it != groups.constEnd();
             ++it)
        {
            int npoints = it->count();
            const Vector *group_array = it->constData();

            //copy the coordinates
            void *output = qMemCopy( group_array, &(combined_array[i]),
                                     npoints*sizeof(Vector) );

            i += npoints;

            //combine the bounding boxes
            combined_box += it->aaBox();
        }

        CoordGroup combined_group;
        combined_group.d = d;

        return combined_group;
    }
}

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

    Vector& operator[](int i);

    Vector* data();

    void translate(const Vector &delta);
    void translate(int i, const Vector &delta);

    void rotate(const Quaternion &quat, const Vector &point);
    void rotate(const Matrix &rotmat, const Vector &point);

    void rotate(int i, const Quaternion &quat, const Vector &point);
    void rotate(int i, const Matrix &rotmat, const Vector &point);

    void setCoordinates(const QVector<Vector> &newcoords);
    void setCoordinates(const CoordGroupBase &newcoords);

    CoordGroup commit();

protected:
    bool needsUpdate() const;
};

}

Q_DECLARE_METATYPE(SireVol::CoordGroup)

SIRE_END_HEADER

#endif
