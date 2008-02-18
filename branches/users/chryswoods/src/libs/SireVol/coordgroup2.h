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

#ifndef SIREVOL_COORDGROUP2_H
#define SIREVOL_COORDGROUP2_H

#include <QSharedData>

#include "SireMaths/vector.h"

#include "aabox.h"

SIRE_BEGIN_HEADER

namespace SireVol
{
class CoordGroup2Base;
class CoordGroup2;
class CoordGroup2Editor;
class CoordGroup2Array;
class CoordGroup2ArrayArray;
}

QDataStream& operator<<(QDataStream&, const SireVol::CoordGroup2&);
QDataStream& operator>>(QDataStream&, SireVol::CoordGroup2&);

namespace SireMaths
{
class AxisSet;
class Matrix;
class Vector;
class Quaternion;
}

namespace SireVol
{

namespace detail
{
class CGArrayArrayData;
class CGArrayData;
class CGData;

template<class T>
class CGSharedPtr
{
public:
    CGSharedPtr(const T *p = 0)
    {
        ptr = const_cast<T*>(p);
    
        if (ptr)
            ptr->incref();
    }
    
    CGSharedPtr(const CGSharedPtr &other) : ptr(other.ptr)
    {
        if (ptr)
            ptr->incref();
    }
    
    ~CGSharedPtr()
    {
        if (ptr)
            ptr->decref();
    }
    
    CGSharedPtr<T>& operator=(const CGSharedPtr &other)
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
    
private:
    /** Actual pointer */
    T *ptr;
};

};

using SireMaths::Vector;
using SireMaths::Quaternion;
using SireMaths::Matrix;

/** This is the base class of all CoordGroup-like classes
    (e.g. CoordGroup and CoordGroupEditor). CoordGroup classes
    hold a group of coordinates, together with an axis-aligned
    box that completely encloses all of those points. The
    class is implicitly shared, and, since it is used in the
    most performance-sensitive parts of the code, has
    a slightly more complex implicit-sharing design.

    @author Christopher Woods
*/
class SIREVOL_EXPORT CoordGroup2Base
{

public:
    ~CoordGroup2Base();

    bool operator==(const CoordGroup2Base &other) const;
    bool operator!=(const CoordGroup2Base &other) const;

    bool maybeDifferent(const CoordGroup2Base &other) const;

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
    void assertSameSize(const CoordGroup2Base &other) const;

protected:
    CoordGroup2Base();

    CoordGroup2Base(quint32 size, const Vector &value = Vector());
    CoordGroup2Base(quint32 size, const Vector *values);

    CoordGroup2Base(const QVector<Vector> &coordinates);

    CoordGroup2Base(const CoordGroup2Base &other);

    CoordGroup2Base& operator=(const CoordGroup2Base &other);

    /** Pointer to the CGData object that describes
        this CoordGroup */
    detail::CGSharedPtr<detail::CGData> d;
};

/** This class holds a group of coordinates. This group forms the basis of the
    Molecular CutGroup, as defined in SireMol. A CoordGroup contains a list of
    coordinates, together with an AABox which provides information as to the
    center and extents of this group. SireVol is designed to calculate distances
    between points in different CoordGroups, or to calculate distances between
    points within a CoordGroup. A CoordGroup is implicitly shared and is
    designed to be fast to use, and fast to copy.

    @author Christopher Woods
*/
class SIREVOL_EXPORT CoordGroup2 : public CoordGroup2Base
{

friend QDataStream& ::operator<<(QDataStream&, const CoordGroup2&);
friend QDataStream& ::operator>>(QDataStream&, CoordGroup2&);

friend class CoordGroup2Editor;

public:
    CoordGroup2();
    CoordGroup2(quint32 size);
    CoordGroup2(quint32 size, const Vector &value);
    CoordGroup2(quint32 size, const Vector *values);
    CoordGroup2(const QVector<Vector> &points);

    CoordGroup2(const CoordGroup2 &other);

    ~CoordGroup2();

    CoordGroup2& operator=(const CoordGroup2 &other);
    CoordGroup2& operator=(CoordGroup2Editor &other);

    CoordGroup2Editor edit() const;

    template<class T>
    static CoordGroup2 combine(const T &groups);

    template<class T>
    static T split(const CoordGroup2 &group, const T &groups);

private:
    CoordGroup2(const CoordGroup2Editor &other);

    static void throwInvalidCountError(uint nats0, uint nats1);
};

/** This class is used to edit a CoordGroup. This class is used when you want to
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
class SIREVOL_EXPORT CoordGroup2Editor : public CoordGroup2Base
{
public:
    CoordGroup2Editor();
    CoordGroup2Editor(const CoordGroup2 &other);

    CoordGroup2Editor(const CoordGroup2Editor &other);

    ~CoordGroup2Editor();

    CoordGroup2Editor& operator=(const CoordGroup2 &cgroup);
    CoordGroup2Editor& operator=(const CoordGroup2Editor &other);

    Vector& operator[](quint32 i);

    Vector* data();

    CoordGroup2Editor& translate(const Vector &delta);
    CoordGroup2Editor& translate(quint32 i, const Vector &delta);

    CoordGroup2Editor& rotate(const Quaternion &quat, const Vector &point);
    CoordGroup2Editor& rotate(const Matrix &rotmat, const Vector &point);

    CoordGroup2Editor& rotate(quint32 i, const Quaternion &quat, const Vector &point);
    CoordGroup2Editor& rotate(quint32 i, const Matrix &rotmat, const Vector &point);

    CoordGroup2Editor& setCoordinates(const QVector<Vector> &newcoords);
    CoordGroup2Editor& setCoordinates(const CoordGroup2Base &newcoords);

    CoordGroup2Editor& setCoordinates(quint32 i, const Vector &newcoords);

    CoordGroup2Editor& mapInto(const SireMaths::AxisSet &axes);
    CoordGroup2Editor& mapInto(quint32 i, const SireMaths::AxisSet &axes);
    
    CoordGroup2Editor& changeFrame(const SireMaths::AxisSet &from_frame,
                                  const SireMaths::AxisSet &to_frame);
                                  
    CoordGroup2Editor& changeFrame(quint32 i,
                                  const SireMaths::AxisSet &from_frame,
                                  const SireMaths::AxisSet &to_frame);

    CoordGroup2 commit();

private:
    /** Whether or not the AABox needs to be recalculated */
    bool needsupdate;
};

}

SIRE_END_HEADER

#endif
