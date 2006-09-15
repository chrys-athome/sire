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

#include "atom.h"

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

using SireVol::CoordGroup;

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
    CutGroup(const AtomInfoGroup &atominfos);

    CutGroup(const CutGroup &other);

    ~CutGroup();

    Atom at(int i) const;
    Atom operator[](int i) const;

    CutGroup& operator=(const CutGroup &other);
    CutGroup& operator=(const CoordGroup &other);

    bool isNull() const;

    QString toString() const;

    QVector<Atom> atoms() const;
    QVector<Atom> atoms(int strt, int end) const;

    Atom atom(int i) const;

    int nAtoms() const;
    int size() const;
    int count() const;

    const CoordGroup& coordinates() const;
    void setCoordinates(const CoordGroup &newcoords);

    void translate(const Vector &delta);
    void rotate(const Quaternion &quat, const Vector &point);
    void rotate(const Matrix &rotmat, const Vector &point);

private:
    /** Set of AtomInfo objects, one for each atom in this CutGroup.
        These can be combined with coordinates of the inherited CoordGroup
        to form complete atoms. */
    QVector<AtomInfo> atominfos;

    /** The coordinates of the atoms */
    CoordGroup coords;
};

/** Return whether or not this is null (has no atoms) */
inline bool CutGroup::isNull() const
{
    return coords.isNull();
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
    coords.translate(delta);
}

/** Rotate the Atoms in the CutGroup by the Quaternion 'quat'
    about the point 'point' */
inline void CutGroup::rotate(const Quaternion &quat, const Vector &point)
{
    coords.rotate(quat, point);
}

/** Rotate (scale and/or shear) the Atoms in the CutGroup by the matrix
    'rotmat' about the point 'point' */
inline void CutGroup::rotate(const Matrix &rotmat, const Vector &point)
{
    coords.rotate(rotmat, point);
}

}

Q_DECLARE_METATYPE(SireMol::CutGroup)

SIRE_END_HEADER

#endif
