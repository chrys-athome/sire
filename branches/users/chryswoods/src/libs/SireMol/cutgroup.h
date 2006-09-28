#ifndef SIREMOL_CUTGROUP_H
#define SIREMOL_CUTGROUP_H
/**
  * @file
  *
  * C++ Interface: CutGroup
  *
  * Description:
  * Interface for CutGroup
  *
  * Author: Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */

#include <QVector>

#include "SireVol/coordgroup.h"

#include "atominfogroup.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class CutGroup;
}

class QDataStream;
QDataStream& operator<<(QDataStream&, const SireMol::CutGroup&);
QDataStream& operator>>(QDataStream&, SireMol::CutGroup&);

namespace SireMol
{

class AtomInfoGroup;
class AtomID;

using SireVol::CoordGroup;
using SireVol::CoordGroupEditor;

using SireMaths::Vector;
using SireMaths::Quaternion;
using SireMaths::Matrix;

/**
A CutGroup is a logical grouping of Atoms into a single group that is considered for intermolecular non-bonded cutting.

The data in a CutGroup is implicitly shared, meaning that copying a CutGroup is very fast, with most of the copying occuring when the copy is modified.

CutGroup is reentrant, but definitely not thread-safe! Copying a CutGroup is thread-safe.

You can edit the coordinates of the atoms in the CutGroup via the code;

\code
CutGroup cgroup;

CoordGroupEditor editor = cgroup.coordinates().edit();

editor[i] = ....

cgroup = editor.commit();
\endcode

@author Christopher Woods
*/
class SIREMOL_EXPORT CutGroup
{

friend QDataStream& ::operator<<(QDataStream&, const CutGroup&);
friend QDataStream& ::operator>>(QDataStream&, CutGroup&);

public:

    CutGroup();
    CutGroup(const QVector<Atom> atoms);
    CutGroup(const QList<Atom> atoms);

    CutGroup(const AtomInfoGroup &atominfos, const CoordGroup &coords);
    CutGroup(const AtomInfoGroup &atominfos, const QVector<Vector> &coords);
    CutGroup(const AtomInfoGroup &atominfos);

    CutGroup(const CutGroup &other);

    ~CutGroup();

    Atom at(AtomID i) const;
    Atom operator[](AtomID i) const;

    bool operator==(const CutGroup &other) const;
    bool operator!=(const CutGroup &other) const;

    CutGroup& operator=(const CutGroup &other);
    CutGroup& operator=(const CoordGroup &other);
    CutGroup& operator=(const QVector<Vector> &coords);

    bool isEmpty() const;

    QString toString() const;

    QVector<Atom> atoms() const;
    QVector<Atom> atoms(AtomID strt, AtomID end) const;

    QVector<Vector> coordinates() const;
    QVector<Vector> coordinates(AtomID strt, AtomID end) const;

    Atom atom(AtomID i) const;

    int nAtoms() const;
    int size() const;
    int count() const;

    void setCoordinates(const QVector<Vector> &newcoords);

    const CoordGroup& coordGroup() const;
    void setCoordGroup(const CoordGroup &newcoords);

    const AtomInfoGroup& atomGroup() const;

    void translate(const Vector &delta);
    void rotate(const Quaternion &quat, const Vector &point);
    void rotate(const Matrix &rotmat, const Vector &point);

private:
    void assertCompatible() const;

    /** The AtomInfoGroup that contains the metainfo for
        all of the atoms */
    AtomInfoGroup atominfos;

    /** The coordinates of the atoms */
    CoordGroup coords;
};

/** Return whether or not this is empty (has no atoms) */
inline bool CutGroup::isEmpty() const
{
    return coords.isEmpty();
}

/** Return the CoordGroup that contains the coordinates of the atoms */
inline const CoordGroup& CutGroup::coordGroup() const
{
    return coords;
}

/** Return the AtomInfoGroup that contains the metainfo for all
    of the atoms */
inline const AtomInfoGroup& CutGroup::atomGroup() const
{
    return atominfos;
}

/** Return the number of atoms in the CutGroup */
inline int CutGroup::nAtoms() const
{
    return coords.count();
}

/** Return the number of atoms in the CutGroup */
inline int CutGroup::size() const
{
    return coords.size();
}

/** Return the number of atoms in the CutGroup */
inline int CutGroup::count() const
{
    return coords.count();
}

/** Translate the CutGroup by 'delta' */
inline void CutGroup::translate(const Vector &delta)
{
    CoordGroupEditor editor = coords.edit();

    editor.translate(delta);

    coords = editor.commit();
}

/** Rotate the Atoms in the CutGroup by the Quaternion 'quat'
    about the point 'point' */
inline void CutGroup::rotate(const Quaternion &quat, const Vector &point)
{
    CoordGroupEditor editor = coords.edit();

    editor.rotate(quat, point);

    coords = editor.commit();
}

/** Rotate (scale and/or shear) the Atoms in the CutGroup by the matrix
    'rotmat' about the point 'point' */
inline void CutGroup::rotate(const Matrix &rotmat, const Vector &point)
{
    CoordGroupEditor editor = coords.edit();

    editor.rotate(rotmat, point);

    coords = editor.commit();
}

}

Q_DECLARE_METATYPE(SireMol::CutGroup)

SIRE_END_HEADER

#endif
