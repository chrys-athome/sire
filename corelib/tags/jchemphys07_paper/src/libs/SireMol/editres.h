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

#ifndef SIREMOL_EDITRES_H
#define SIREMOL_EDITRES_H

#include "editmoldata.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class EditRes;
}

class QDataStream;
QDataStream& operator<<(QDataStream&, const SireMol::EditRes&);
QDataStream& operator>>(QDataStream&, SireMol::EditRes&);

namespace SireMol
{

class EditMol;
class ResidueInfo;

/**
This class represents an editable residue within an EditMol. This contains functions
to add and query the atoms and connectivity of this residue.

This class is implemented as a view of the EditMolData class, which is used as the
underlying private object of both EditMol and EditRes.

Note that this class is reentrant, though not thread-safe.

@author Christopher Woods
*/
class SIREMOL_EXPORT EditRes
{

friend class EditMol;  // friend so it call call the EditMolData constructor
friend QDataStream& ::operator<<(QDataStream&, const EditRes&);
friend QDataStream& ::operator>>(QDataStream&, EditRes&);

public:
    EditRes();

    EditRes(const EditMol &editmol, ResNum resnum);
    EditRes(const EditMol &editmol, ResID resid);
    EditRes(const EditMol &editmol, const QString &resname);

    EditRes(const EditRes &other);

    ~EditRes();

   //// Operators /////////////////////////////////
    bool operator==(const EditRes &other) const;
    bool operator!=(const EditRes &other) const;

    EditRes& operator=(const EditRes &other);

    Atom operator[](AtomID atomid) const;
    Atom operator[](const QString &nam) const;
    Atom operator[](const AtomIndex &atom) const;

    CutGroup operator[](CutGroupID cgid) const;
    CutGroup operator[](CutGroupNum cgnum) const;
   ////////////////////////////////////////////////


   //// Interface with EditMol ////////////////////
    EditMol molecule() const;
   ////////////////////////////////////////////////


   //// Query functions ///////////////////////////
    Atom at(AtomID i) const;
    Atom at(const QString &atomname) const;
    Atom at(const AtomIndex &atom) const;

    CutGroup at(CutGroupID cgid) const;
    CutGroup at(CutGroupNum cgnum) const;

    ResidueBonds connectivity() const;

    ResidueInfo info() const;

    QHash<CutGroupID,CutGroup> cutGroupsByID() const;
    QHash<CutGroupNum,CutGroup> cutGroupsByNum() const;

    CutGroup cutGroup(CutGroupID cgid) const;
    CutGroup cutGroup(CutGroupNum cgnum) const;

    QHash<CutGroupID,CoordGroup> coordGroupsByID() const;
    QHash<CutGroupNum,CoordGroup> coordGroupsByNum() const;

    CoordGroup coordGroup(CutGroupID cgid) const;
    CoordGroup coordGroup(CutGroupNum cgnum) const;

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
    bool contains(CutGroupNum cgnum) const;
    bool contains(AtomID atomid) const;
    bool contains(const QString &atomname) const;
    bool contains(const AtomIndex &atom) const;

    bool contains(const Bond &bond) const;

    int nAtoms() const;
    int nAtoms(CutGroupID cgid) const;
    int nAtoms(CutGroupNum cgnum) const;

    int nCutGroups() const;

    int nBonds() const;
    int nIntraBonds() const;
    int nInterBonds() const;

    QStringList atomNames() const;

    QHash<ResNum,EditRes> bondedResidues() const;
    QHash<ResNum,EditRes> residuesBondedTo(AtomID atom) const;
    QHash<ResNum,EditRes> residuesBondedTo(const QString &atomname) const;
    QHash<ResNum,EditRes> residuesBondedTo(const AtomIndex &atom) const;

    SireMaths::Line bond(const Bond &bnd) const;
    SireMaths::Triangle angle(const SireMol::Angle &ang) const;
    SireMaths::Torsion dihedral(const Dihedral &dih) const;
    SireMaths::Torsion improper(const Improper &improper) const;

    double measure(const Bond &bnd) const;
    SireUnits::Dimension::Angle measure(const SireMol::Angle &ang) const;
    SireUnits::Dimension::Angle measure(const Dihedral &dih) const;
    SireUnits::Dimension::Angle measure(const Improper &improper) const;

    double getWeight(const QStringList &group0, const QStringList &group1,
                     const WeightFunction &weightfunc) const;

    double getWeight(const QSet<AtomIndex> &group0, const QSet<AtomIndex> &group1,
                     const WeightFunction &weightfunc) const;
   ////////////////////////////////////////////////


   //// Editing functions /////////////////////////
    void setName(QString name);
    void setNumber(ResNum newnum);

    void add(const QString &atm);
    void add(const Atom &atm);

    void add(const QString &atm, CutGroupNum cgnum);
    void add(const Atom &atm, CutGroupNum cgnum);

    void remove(const QString &atm);
    void remove(const AtomIndex &atm);

    void addBond(const QString &atmnam0, const QString &atmnam1);
    void removeBond(const QString &atmnam0, const QString &atmnam1);

    void add(const Bond &bond);
    void remove(const Bond &bond);

    void addAutoBonds();
    void addAutoBonds(const BondAddingFunction &bondfunc);

    void removeAllBonds(const QString &atomname);
    void removeAllBonds();
   ////////////////////////////////////////////////

    void assertSameResidue(const AtomIndex &atom) const;
    void assertSameResidue(const QSet<AtomIndex> &atoms) const;
    void assertSameResidue(CutGroupNum cgnum) const;
    void assertSameResidue(CutGroupID cgid) const;
    void assertSameResidue(const Bond &bond) const;

private:
    EditRes(const QSharedDataPointer<EditMolData> &ptr, ResNum resnum);
    
    QHash<ResNum,EditRes> getResidues(const QSet<ResNum> &resnums) const;
    
    /** Implicitly shared pointer to the actual data of this residue */
    QSharedDataPointer<EditMolData> d;

    /** The residue number of this residue */
    ResNum rnum;
};

}

Q_DECLARE_METATYPE(SireMol::EditRes)

SIRE_END_HEADER

#endif
