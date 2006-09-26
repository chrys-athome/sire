#ifndef SIREVOL_COORDGROUP_H
#define SIREVOL_COORDGROUP_H
/**
  * @file
  *
  * C++ Interface: CoordGroupBase, CoordGroup and CoordGroupEditor
  *
  * Description:
  * Interface to CoordGroupBase, CoordGroup and CoordGroupEditor
  *
  * Author: Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */

#include <QSharedDataPointer>

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
class CoordGroup;
}

QDataStream& operator<<(QDataStream&, const SireVol::CoordGroup&);
QDataStream& operator>>(QDataStream&, SireVol::CoordGroup&);

namespace SireVol
{

using SireMaths::Vector;
using SireMaths::Quaternion;
using SireMaths::Matrix;

class CoordGroupPvt;
class CoordGroupEditor;

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

    CoordGroupBase& operator=(const CoordGroupBase &other);

    bool operator==(const CoordGroupBase &other) const;
    bool operator!=(const CoordGroupBase &other) const;

protected:
    void setCoordinates(const QVector<Vector> &newcoords);

    /** Implicitly shared pointer to the coordinate data */
    QSharedDataPointer<CoordGroupPvt> d;
};

/**
This class holds a group of coordinates. This group forms the basis of the Molecular CutGroup, as defined in SireMol. A CoordGroup contains a list of coordinates, together with an AABox which provides information as to the center and extents of this group. SireVol is designed to calculate distances between points in different CoordGroups, or to calculate distances between points within a CoordGroup. A CoordGroup is implicitly shared and is designed to be fast to use, and fast to copy.

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

    CoordGroup& operator=(const CoordGroupBase &other);

    const Vector& at(int i) const;
    const Vector& operator[](int i) const;

    bool isNull() const;

    int count() const;
    int size() const;

    const AABox& aaBox() const;
    const Vector* constData() const;

    void translate(const Vector &delta);
    void rotate(const Quaternion &quat, const Vector &point);
    void rotate(const Matrix &rotmat, const Vector &point);

    CoordGroupEditor edit();
};

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

    const Vector& at(int i) const;

    const Vector& operator[](int i) const;
    Vector& operator[](int i);

    const Vector* constData() const;
    const Vector* data() const;
    Vector* data();

    bool isNull() const;

    int count() const;
    int size() const;

    void translate(const Vector &delta);
    void translate(int i, const Vector &delta);

    void rotate(const Quaternion &quat, const Vector &point);
    void rotate(const Matrix &rotmat, const Vector &point);

    void rotate(int i, const Quaternion &quat, const Vector &point);
    void rotate(int i, const Matrix &rotmat, const Vector &point);

    CoordGroup commit();
};

}

Q_DECLARE_METATYPE(SireVol::CoordGroup)

SIRE_END_HEADER

#endif
