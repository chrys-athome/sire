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

#ifndef SIREMOL_MOLECULEMOVER_H
#define SIREMOL_MOLECULEMOVER_H

#include <QVector>
#include <QHash>

#include "atomindex.h"
#include "atomaliases.h"
#include "moleculeview.h"

#include "SireUnits/dimensions.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class MoleculeMover;
}

QDataStream& operator<<(QDataStream&, const SireMol::MoleculeMover&);
QDataStream& operator>>(QDataStream&, SireMol::MoleculeMover&);

namespace SireVol
{
class Space;
class CoordGroup;
}

namespace SireMaths
{
class Vector;
class Matrix;
class Quaternion;
class Line;
class Triangle;
class Torsion;
}

namespace SireMol
{

class AtomIDGroup;
class Bond;
class Angle;
class Dihedral;
class Improper;

class MoleculeID;
class ResID;
class AtomID;
class ResNum;
class AtomIndex;
class CutGroupID;

class WeightFunction;

using SireVol::CoordGroup;
using SireVol::Space;

using SireMaths::Vector;
using SireMaths::Matrix;
using SireMaths::Quaternion;

/** This class is used to add a nice API to the MoleculeView based classes to
    allow the movement of parts of the molecule,

    e.g. can type mol.move().translate(delta)
                  res.move().change( bnd, delta )
                  mol.move().angle( atm0, atm1, atm2, delta )
                  mol.move().mapInto( space, &cgids_that_moved )
                  atom.move().rotate( matrix, point );

                  mol.move().change(bnd0,delta,&moved).change(bnd1,delta,&moved)
                            .change(ang0,delta,&moved).change(ang1,delta,&moved)

    All of the move functions take an optional pointer to a QSet<CutGroupID>
    which can be used to keep track of which CutGroups have been affected
    by the moves.

    In addition, this allows me to have the atom moving code
    all in one place, without the need to update multiple MoleculeView
    derived classes APIs

    @author Christopher Woods
*/
class SIREMOL_EXPORT MoleculeMover : public MolDataView
{

friend QDataStream& ::operator<<(QDataStream&, const MoleculeMover&);
friend QDataStream& ::operator>>(QDataStream&, MoleculeMover&);

public:
    MoleculeMover();

    MoleculeMover(const MoleculeView &molecule);

    MoleculeMover(const MoleculeView &molecule,
                  const SelectionFromMol &selection);

    MoleculeMover(const MoleculeMover &other);

    ~MoleculeMover();

    MoleculeMover& operator=(const MoleculeMover &other);

    MoleculeMover mapInto(const Space &space) const;

    MoleculeMover translate(const Vector &delta) const;
    MoleculeMover translate(const AtomIDGroup &group,
                            const Vector &delta) const;
    MoleculeMover translate(const AtomIndex &atom,
                            const Vector &delta) const;
    MoleculeMover translate(const QSet<AtomIndex> &atoms,
                            const Vector &delta) const;
    MoleculeMover translate(ResNum resnum, const QStringList &atoms,
                            const Vector &delta) const;
    MoleculeMover translate(ResNum resnum, const Vector &delta) const;
    MoleculeMover translate(const QSet<ResNum> &resnums,
                            const Vector &delta) const;
    MoleculeMover translate(ResID resid, const QStringList &atoms,
                            const Vector &delta) const;
    MoleculeMover translate(ResID resid, const Vector &delta) const;
    MoleculeMover translate(const QSet<ResID> &resids, const Vector &delta) const;
    MoleculeMover translate(CutGroupID cgid, const Vector &delta) const;
    MoleculeMover translate(const QSet<CutGroupID> &cgids, const Vector &delta) const;

    MoleculeMover rotate(const Quaternion &quat, const Vector &point) const;
    MoleculeMover rotate(const AtomIDGroup &group, const Quaternion &quat,
                         const Vector &point) const;
    MoleculeMover rotate(const AtomIndex &atom, const Quaternion &quat,
                         const Vector &point) const;
    MoleculeMover rotate(const QSet<AtomIndex> &atoms, const Quaternion &quat,
                         const Vector &point) const;
    MoleculeMover rotate(ResNum resnum, const QStringList &atoms,
                         const Quaternion &quat, const Vector &point) const;
    MoleculeMover rotate(ResNum resnum, const Quaternion &quat,
                         const Vector &point) const;
    MoleculeMover rotate(const QSet<ResNum> &resnums, const Quaternion &quat,
                         const Vector &point) const;
    MoleculeMover rotate(ResID resid, const QStringList &atoms,
                         const Quaternion &quat, const Vector &point) const;
    MoleculeMover rotate(ResID resid, const Quaternion &quat,
                         const Vector &point) const;
    MoleculeMover rotate(const QSet<ResID> &resids, const Quaternion &quat,
                         const Vector &point) const;
    MoleculeMover rotate(CutGroupID cgid, const Quaternion &quat,
                         const Vector &point) const;
    MoleculeMover rotate(const QSet<CutGroupID> &cgids, const Quaternion &quat,
                         const Vector &point) const;

    MoleculeMover rotate(const Matrix &matrix, const Vector &point) const;
    MoleculeMover rotate(const AtomIDGroup &group, const Matrix &matrix,
                         const Vector &point) const;
    MoleculeMover rotate(const AtomIndex &atom, const Matrix &matrix,
                         const Vector &point) const;
    MoleculeMover rotate(const QSet<AtomIndex> &atoms, const Matrix &matrix,
                         const Vector &point) const;
    MoleculeMover rotate(ResNum resnum, const QStringList &atoms, const Matrix &matrix,
                         const Vector &point) const;
    MoleculeMover rotate(ResNum resnum, const Matrix &matrix, const Vector &point) const;
    MoleculeMover rotate(const QSet<ResNum> &resnums, const Matrix &matrix,
                         const Vector &point) const;
    MoleculeMover rotate(ResID resid, const QStringList &atoms, const Matrix &matrix,
                         const Vector &point) const;
    MoleculeMover rotate(ResID resid, const Matrix &matrix, const Vector &point) const;
    MoleculeMover rotate(const QSet<ResID> &resids, const Matrix &matrix,
                         const Vector &point) const;
    MoleculeMover rotate(CutGroupID cgid, const Matrix &matrix,
                         const Vector &point) const;
    MoleculeMover rotate(const QSet<CutGroupID> &cgids, const Matrix &matrix,
                         const Vector &point) const;

    MoleculeMover translateAndRotate(const Vector &delta,
                                     const Quaternion &quat, const Vector &point) const;

    MoleculeMover setCoordinates(CutGroupID cgid, const CoordGroup &newcoords) const;
    MoleculeMover setCoordinates(const QHash<CutGroupID,CoordGroup> &newcoords) const;
    MoleculeMover setCoordinates(const QVector<CoordGroup> &newcoords) const;

    MoleculeMover setCoordinates(const QVector<Vector> &newcoords) const;

    MoleculeMover setCoordinates(CutGroupID cgid,
                                 const QVector<Vector> &newcoords) const;
    MoleculeMover setCoordinates(const QHash< CutGroupID,QVector<Vector> >
                                                                &newcoords) const;

    MoleculeMover setCoordinates(ResNum resnum, const QVector<Vector> &newcoords) const;
    MoleculeMover setCoordinates(const QHash< ResNum,QVector<Vector> > &newcoords) const;

    MoleculeMover setCoordinates(ResID resid, const QVector<Vector> &newcoords) const;
    MoleculeMover setCoordinates(const QHash< ResID,QVector<Vector> > &newcoords) const;

    MoleculeMover setCoordinates(const AtomIndex &atom, const Vector &newcoords) const;
    MoleculeMover setCoordinates(const QHash<AtomIndex,Vector> &newcoords) const;

    MoleculeMover setCoordinates(const CGAtomID &cgatomid,
                                 const Vector &newcoords) const;
    MoleculeMover setCoordinates(const QHash<CGAtomID,Vector> &newcoords) const;

    MoleculeMover setCoordinates(const ResNumAtomID &resatomid,
                                 const Vector &newcoords) const;
    MoleculeMover setCoordinates(const QHash<ResNumAtomID,Vector> &newcoords) const;

    MoleculeMover setCoordinates(const ResIDAtomID &resatomid,
                                 const Vector &newcoords) const;
    MoleculeMover setCoordinates(const QHash<ResIDAtomID,Vector> &newcoords) const;

    MoleculeMover change(const Bond &bond, double delta,
                         const QSet<AtomIndex> &anchors = QSet<AtomIndex>()) const;
    MoleculeMover change(const Bond &bond, double delta, const WeightFunction &func,
                         const QSet<AtomIndex> &anchors = QSet<AtomIndex>()) const;

    MoleculeMover change(const Angle &angle, SireUnits::Dimension::Angle delta,
                         const QSet<AtomIndex> &anchors = QSet<AtomIndex>()) const;
    MoleculeMover change(const Angle &angle, SireUnits::Dimension::Angle delta,
                         const WeightFunction &func,
                         const QSet<AtomIndex> &anchors = QSet<AtomIndex>()) const;

    MoleculeMover change(const Dihedral &dihedral, SireUnits::Dimension::Angle delta,
                         const QSet<AtomIndex> &anchors = QSet<AtomIndex>()) const;
    MoleculeMover change(const Dihedral &dihedral, SireUnits::Dimension::Angle delta,
                         const WeightFunction &func,
                         const QSet<AtomIndex> &anchors = QSet<AtomIndex>()) const;

    MoleculeMover change(const Bond &bond, SireUnits::Dimension::Angle delta,
                         const QSet<AtomIndex> &anchors = QSet<AtomIndex>()) const;
    MoleculeMover change(const Bond &bond, SireUnits::Dimension::Angle delta,
                         const WeightFunction &func,
                         const QSet<AtomIndex> &anchors = QSet<AtomIndex>()) const;

    MoleculeMover change(const Improper &improper, SireUnits::Dimension::Angle delta,
                         const QSet<AtomIndex> &anchors = QSet<AtomIndex>()) const;
    MoleculeMover change(const Improper &improper, SireUnits::Dimension::Angle delta,
                         const WeightFunction &func,
                         const QSet<AtomIndex> &anchors = QSet<AtomIndex>()) const;

    MoleculeMover set(const Bond &bond, double lgth,
                      const QSet<AtomIndex> &anchors = QSet<AtomIndex>()) const;
    MoleculeMover set(const Bond &bond, double lgth, const WeightFunction &func,
                      const QSet<AtomIndex> &anchors = QSet<AtomIndex>()) const;

    MoleculeMover set(const SireMol::Angle &angle, SireUnits::Dimension::Angle ang,
                      const QSet<AtomIndex> &anchors = QSet<AtomIndex>()) const;
    MoleculeMover set(const SireMol::Angle &angle, SireUnits::Dimension::Angle ang,
                      const WeightFunction &func,
                      const QSet<AtomIndex> &anchors = QSet<AtomIndex>()) const;

    MoleculeMover set(const Dihedral &dihedral, SireUnits::Dimension::Angle ang,
                      const QSet<AtomIndex> &anchors = QSet<AtomIndex>()) const;
    MoleculeMover set(const Dihedral &dihedral, SireUnits::Dimension::Angle ang,
                      const WeightFunction &func,
                      const QSet<AtomIndex> &anchors = QSet<AtomIndex>()) const;

    MoleculeMover setAll(const Dihedral &dihedral, SireUnits::Dimension::Angle ang,
                         const QSet<AtomIndex> &anchors = QSet<AtomIndex>()) const;
    MoleculeMover setAll(const Dihedral &dihedral, SireUnits::Dimension::Angle ang,
                         const WeightFunction &func,
                         const QSet<AtomIndex> &anchors = QSet<AtomIndex>()) const;

    MoleculeMover set(const Improper &improper, SireUnits::Dimension::Angle ang,
                      const QSet<AtomIndex> &anchors = QSet<AtomIndex>()) const;
    MoleculeMover set(const Improper &improper, SireUnits::Dimension::Angle ang,
                      const WeightFunction &func,
                      const QSet<AtomIndex> &anchors = QSet<AtomIndex>()) const;

    MoleculeMover align(const MolDataView &other,
                        const AtomAliases &aliases = AtomAliases()) const;
    MoleculeMover align(const MolDataView &other,
                        const AtomSelection &selected_atoms,
                        const AtomAliases &aliases = AtomAliases()) const;
};

}

Q_DECLARE_METATYPE(SireMol::MoleculeMover)

SIRE_END_HEADER

#endif
