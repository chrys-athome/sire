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

#ifndef SIREMOL_RESIDUE_H
#define SIREMOL_RESIDUE_H

#include <QSharedDataPointer>

#include "resnum.h"
#include "atomindex.h"
#include "moleculeview.h"

SIRE_BEGIN_HEADER

namespace SireMaths
{
class Vector;
class Matrix;
class Quaternion;
class Line;
class Triangle;
class Torsion;
class Angle;
}

namespace SireMol
{
class Residue;
}

QDataStream& operator<<(QDataStream&, const SireMol::Residue&);
QDataStream& operator>>(QDataStream&, SireMol::Residue&);

namespace SireMol
{

class Molecule;
class PartialMolecule;
class MoleculeVersion;
class NewAtom;
class ResidueBonds;
class Bond;
class Angle;
class Dihedral;
class Improper;
class ResidueInfo;

class WeightFunction;

/**
This class represents a Residue in a Molecule.

@author Christopher Woods
*/
class SIREMOL_EXPORT Residue : public MoleculeView
{

friend QDataStream& ::operator<<(QDataStream&, const Residue&);
friend QDataStream& ::operator>>(QDataStream&, Residue&);

public:
    Residue();
    Residue(const Molecule &mol, ResNum resnum);
    Residue(const Molecule &mol, ResID resid);
    Residue(const Molecule &mol, const QString &resname);

    Residue(const NewAtom &atom);

    Residue(const Residue &other);

    ~Residue();

   ///// Operators /////////////////////////////////////////
    bool operator==(const Residue &other) const;
    bool operator!=(const Residue &other) const;

    Residue& operator=(const Residue &other);

    Atom operator[](AtomID i) const;
    Atom operator[](const QString &atomname) const;
    Atom operator[](const AtomIndex &atom) const;

    CutGroup operator[](CutGroupID cgid) const;
    CutGroup operator[](CutGroupNum cgnum) const;
   /////////////////////////////////////////////////////////


   ///// Querying the residue //////////////////////////////
    MoleculeID ID() const;
    const MoleculeVersion& version() const;

    QString idString() const;

    Atom at(AtomID i) const;
    Atom at(const QString &atomname) const;
    Atom at(const AtomIndex &atom) const;

    CutGroup at(CutGroupID cgid) const;
    CutGroup at(CutGroupNum cgnum) const;

    ResidueBonds connectivity() const;

    const ResidueInfo& info() const;

    QHash<CutGroupID,CutGroup> cutGroups() const;
    CutGroup cutGroup(CutGroupID cgid) const;

    QHash<CutGroupID,CoordGroup> coordGroups() const;
    CoordGroup coordGroup(CutGroupID cgid) const;

    Atom atom(AtomID i) const;
    Atom atom(const QString &atomname) const;
    Atom atom(const AtomIndex &atom) const;

    Vector coordinates(AtomID i) const;
    Vector coordinates(const QString &atomname) const;
    Vector coordinates(const AtomIndex &atom) const;

    QVector<Atom> atoms() const;
    QVector<Vector> coordinates() const;

    QHash<AtomID,Atom> atoms(const QSet<AtomID> &atomids) const;
    QHash<AtomID,Vector> coordinates(const QSet<AtomID> &atomids) const;

    QHash<QString,Atom> atoms(const QSet<QString> &atomnames) const;
    QHash<QString,Vector> coordinates(const QSet<QString> &atomnames) const;

    QHash<AtomIndex,Atom> atoms(const QSet<AtomIndex> &atms) const;
    QHash<AtomIndex,Vector> coordinates(const QSet<AtomIndex> &atms) const;

    QString name() const;
    QString resName() const;

    ResNum number() const;
    ResNum resNum() const;

    bool isEmpty() const;

    bool contains(CutGroupID cgid) const;
    bool contains(AtomID atomid) const;
    bool contains(const QString &atomname) const;
    bool contains(const AtomIndex &atom) const;

    bool contains(const Bond &bond) const;

    int nAtoms() const;
    int nAtoms(CutGroupID cgid) const;

    int nCutGroups() const;

    int nBonds() const;
    int nIntraBonds() const;
    int nInterBonds() const;

    QStringList atomNames() const;

    QHash<ResNum,Residue> bondedResidues() const;
    QHash<ResNum,Residue> residuesBondedTo(AtomID atom) const;
    QHash<ResNum,Residue> residuesBondedTo(const QString &atomname) const;
    QHash<ResNum,Residue> residuesBondedTo(const AtomIndex &atom) const;

    SireMaths::Line bond(const Bond &bnd) const;
    SireMaths::Triangle angle(const SireMol::Angle &ang) const;
    SireMaths::Torsion dihedral(const Dihedral &dih) const;
    SireMaths::Torsion improper(const Improper &improper) const;

    double measure(const Bond &bnd) const;
    SireMaths::Angle measure(const SireMol::Angle &ang) const;
    SireMaths::Angle measure(const Dihedral &dih) const;
    SireMaths::Angle measure(const Improper &improper) const;

    double getWeight(const QStringList &group0, const QStringList &group1,
                     const WeightFunction &weightfunc) const;

    double getWeight(const QSet<AtomIndex> &group0, const QSet<AtomIndex> &group1,
                     const WeightFunction &weightfunc) const;
   /////////////////////////////////////////////////////////


   //// Moving the residue /////////////////////////////////
    void translate(const Vector &delta);
    void translate(AtomID atomid, const Vector &delta);
    void translate(const QSet<AtomID> &atomids, const Vector &delta);
    void translate(const QString &atom, const Vector &delta);
    void translate(const QStringList &atoms, const Vector &delta);
    void translate(const AtomIndex &atom, const Vector &delta);
    void translate(const QSet<AtomIndex> &atoms, const Vector &delta);

    void rotate(const Quaternion &quat, const Vector &point);
    void rotate(AtomID atomid, const Quaternion &quat, const Vector &point);
    void rotate(const QSet<AtomID> &atomids, const Quaternion &quat, const Vector &point);
    void rotate(const QString &atom, const Quaternion &quat, const Vector &point);
    void rotate(const QStringList &atoms, const Quaternion &quat, const Vector &point);
    void rotate(const AtomIndex &atom, const Quaternion &quat, const Vector &point);
    void rotate(const QSet<AtomIndex> &atoms, const Quaternion &quat, const Vector &point);

    void rotate(const Matrix &rotmat, const Vector &point);
    void rotate(AtomID atomid, const Matrix &rotmat, const Vector &point);
    void rotate(const QSet<AtomID> &atomids, const Matrix &rotmat, const Vector &point);
    void rotate(const QString &atom, const Matrix &rotmat, const Vector &point);
    void rotate(const QStringList &atoms, const Matrix &rotmat, const Vector &point);
    void rotate(const AtomIndex &atom, const Matrix &rotmat, const Vector &point);
    void rotate(const QSet<AtomIndex> &atoms, const Matrix &rotmat, const Vector &point);

    void setCoordinates(const QVector<Vector> &newcoords);

    void setCoordinates(AtomID atomid, const Vector &newcoords);
    void setCoordinates(const QHash<AtomID,Vector> &newcoords);

    void setCoordinates(const QString &atomname, const Vector &newcoords);
    void setCoordinates(const QHash<QString,Vector> &newcoords);

    void setCoordinates(const AtomIndex &atom, const Vector &newcoords);
    void setCoordinates(const QHash<AtomIndex,Vector> &newcoords);
   /////////////////////////////////////////////////////////


   //// Internal geometry moves ////////////////////////////
    void change(const Bond &bond, double delta,
                const QSet<AtomIndex> &anchors = QSet<AtomIndex>());
    void change(const Bond &bond, double delta, const WeightFunction &func,
                const QSet<AtomIndex> &anchors = QSet<AtomIndex>());

    void change(const Angle &angle, const SireMaths::Angle &delta,
                const QSet<AtomIndex> &anchors = QSet<AtomIndex>());
    void change(const Angle &angle, const SireMaths::Angle &delta,
                const WeightFunction &func, const QSet<AtomIndex> &anchors = QSet<AtomIndex>());

    void change(const Dihedral &dihedral, const SireMaths::Angle &delta,
                const QSet<AtomIndex> &anchors = QSet<AtomIndex>());
    void change(const Dihedral &dihedral, const SireMaths::Angle &delta,
                const WeightFunction &func, const QSet<AtomIndex> &anchors = QSet<AtomIndex>());

    void change(const Bond &bond, const SireMaths::Angle &delta,
                const QSet<AtomIndex> &anchors = QSet<AtomIndex>());
    void change(const Bond &bond, const SireMaths::Angle &delta,
                const WeightFunction &func, const QSet<AtomIndex> &anchors = QSet<AtomIndex>());

    void change(const Improper &improper, const SireMaths::Angle &delta,
                const QSet<AtomIndex> &anchors = QSet<AtomIndex>());
    void change(const Improper &improper, const SireMaths::Angle &delta,
                const WeightFunction &func, const QSet<AtomIndex> &anchors = QSet<AtomIndex>());

    void set(const Bond &bond, double lgth, const QSet<AtomIndex> &anchors = QSet<AtomIndex>());
    void set(const Bond &bond, double lgth, const WeightFunction &func,
             const QSet<AtomIndex> &anchors = QSet<AtomIndex>());

    void set(const SireMol::Angle &angle, const SireMaths::Angle &ang,
             const QSet<AtomIndex> &anchors = QSet<AtomIndex>());
    void set(const SireMol::Angle &angle, const SireMaths::Angle &ang,
             const WeightFunction &func, const QSet<AtomIndex> &anchors = QSet<AtomIndex>());

    void set(const Dihedral &dihedral, const SireMaths::Angle &ang,
             const QSet<AtomIndex> &anchors = QSet<AtomIndex>());
    void set(const Dihedral &dihedral, const SireMaths::Angle &ang,
             const WeightFunction &func, const QSet<AtomIndex> &anchors = QSet<AtomIndex>());

    void setAll(const Dihedral &dihedral, const SireMaths::Angle &ang,
                const QSet<AtomIndex> &anchors = QSet<AtomIndex>());
    void setAll(const Dihedral &dihedral, const SireMaths::Angle &ang,
                const WeightFunction &func, const QSet<AtomIndex> &anchors = QSet<AtomIndex>());

    void set(const Improper &improper, const SireMaths::Angle &ang,
             const QSet<AtomIndex> &anchors = QSet<AtomIndex>());
    void set(const Improper &improper, const SireMaths::Angle &ang,
             const WeightFunction &func, const QSet<AtomIndex> &anchors = QSet<AtomIndex>());
   /////////////////////////////////////////////////////////

private:
    /** The residue number of this residue */
    ResNum rnum;

};

}

Q_DECLARE_METATYPE(SireMol::Residue)

SIRE_END_HEADER

#endif
