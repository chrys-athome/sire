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

#ifndef SIREMOL_EDITMOL_H
#define SIREMOL_EDITMOL_H

#include "editmoldata.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class EditMol;

namespace detail
{
class MolData;
}

}

class QDataStream;
QDataStream& operator<<(QDataStream &ds, const SireMol::EditMol &editmol);
QDataStream& operator>>(QDataStream &ds, SireMol::EditMol &editmol);

uint qHash(const SireMol::EditMol &editmol);

namespace SireMol
{

class TemplateFunction;
class EditRes;
class Molecule;

/**
An EditMol is a molecule that may be edited. It is not based on CutGroup, but is instead
optimised to have atoms and bonds easily added or removed, and its entire data structure
is designed for rapid molecular editing. An EditMol represents an entire, single molecule.
This is the way that you edit molecules, convert from a Molecule to an EditMol, edit the molecule,
and then convert back again.

An EditMol consists of a set of EditRes (each representing a residue), each consisting of Atoms.
The EditMol may also contain Bonds that connect Atoms together.

Note that the functions in this class are re-entrant, though this class is not thread-safe.

The implementation of EditMol and EditRes is as views onto a common EditMolData class. This
class is implicitly shared between the different EditRes and EditMol views. This means that
copying an EditMol or EditRes is fast.

Note that EditMols follow the rules of self-managing objects, namely it is not possible
to change their internal state without using their own API. This means that an EditRes
is a copy of the residue, not the actual residue. Thus changing an atom in an EditRes
will not change it in the parent molecule! To change a molecule via the residue interface
you would do;

\code
EditMol editmol;

//populate editmol with atoms and bonds...

//get a residue view of the molecule
EditRes res = editmol[ ResNum(3) ];

//edit the residue
res.setName("HID");
res.add( "HE1", Vector(1.0,2.0,3.0) );

//copy back to the molecule
editmol = res.molecule();
\endcode

Note that this means that you can't edit more than one residue at a time, as otherwise
you would lose the changes in one or other of the copies;

\code
EditMol editmol;

EditRes res0 = editmol[ ResNum(3) ];
EditRes res1 = editmol[ ResNum(4) ];

//res0 and res1 are independent copies of 'editmol'
res0.setName("HID");

res1.setName("ALA");

//commit the changes in res0
editmol = res0.molecule();

//committing the changes in res0 will now lose the
//changes from res0!
editmol = res1.molecule();
\endcode

The correct way to edit in this case is to do it sequentially;

\code
EditMol editmol;

EditRes res = editmol[ ResNum(3) ];

res.setName("HID");

editmol = res.molecule();

res = editmol[ ResNum(4) ];

res.setName("ALA");

editmol = res.molecule();
\endcode

@author Christopher Woods
*/
class SIREMOL_EXPORT EditMol
{

friend class EditRes;  //friend so can see the EditMolData
friend QDataStream& ::operator<<(QDataStream&, const EditMol&);
friend QDataStream& ::operator>>(QDataStream&, EditMol&);

public:
   ////// Constructors / destructor ////////////////////////
    EditMol();
    EditMol(const QString &name);
    EditMol(const QString &name, const CuttingFunction &cuttingfunc);
    EditMol(const CuttingFunction &cuttingfunc);

    EditMol(const EditRes &other);
    EditMol(const EditMol &other);

    ~EditMol();
   /////////////////////////////////////////////////////////


   ////// Operators ////////////////////////////////////////
    EditMol& operator=(const EditMol &other);

    bool operator==(const EditMol &other) const;
    bool operator!=(const EditMol &other) const;

    CutGroup operator[](CutGroupID cgid) const;
    CutGroup operator[](CutGroupNum cgnum) const;

    EditRes operator[](ResID resid) const;
    EditRes operator[](ResNum resnum) const;

    Atom operator[](AtomID atomid) const;
    Atom operator[](const CGAtomID &cgatomid) const;
    Atom operator[](const CGNumAtomID &cgatomid) const;
    Atom operator[](const ResNumAtomID &resatomid) const;
    Atom operator[](const ResIDAtomID &resatomid) const;
    Atom operator[](const AtomIndex &atm) const;
   /////////////////////////////////////////////////////////


   ///// Interfacing with EditRes //////////////////////////
    EditRes residue(ResNum resnum) const;
    EditRes residue(ResID resid) const;
    EditRes residue(const QString &resname) const;

    QHash<ResNum,EditRes> residues() const;
    QHash<ResNum,EditRes> residues(const QSet<ResNum> &resnums) const;

    EditRes at(ResNum resnum) const;
    EditRes at(ResID i) const;
   /////////////////////////////////////////////////////////


   ////// Interfacing with Molecule ////////////////////////
    detail::MolData commit() const;
   /////////////////////////////////////////////////////////


   ///// Querying the molecule /////////////////////////////
    CutGroup at(CutGroupID cgid) const;
    CutGroup at(CutGroupNum cgnum) const;

    Atom at(AtomID atomid) const;
    Atom at(const ResNumAtomID &resatomid) const;
    Atom at(const ResIDAtomID &resatomid) const;
    Atom at(const CGAtomID &cgatomid) const;
    Atom at(const CGNumAtomID &cgatomid) const;
    Atom at(const AtomIndex &atm) const;

    MoleculeBonds connectivity() const;

    ResidueBonds connectivity(ResNum resnum) const;
    ResidueBonds connectivity(ResID resid) const;

    MoleculeInfo info() const;

    QVector<CutGroup> cutGroups() const;

    QHash<CutGroupID,CutGroup> cutGroupsByID(ResNum resnum) const;
    QHash<CutGroupID,CutGroup> cutGroupsByID(ResID resid) const;

    QHash<CutGroupNum,CutGroup> cutGroupsByNum(ResNum resnum) const;
    QHash<CutGroupNum,CutGroup> cutGroupsByNum(ResID resid) const;

    QHash<CutGroupID,CutGroup> cutGroups(const QSet<CutGroupID> &cgids) const;
    QHash<CutGroupNum,CutGroup> cutGroups(const QSet<CutGroupNum> &cgnums) const;

    QHash<CutGroupID,CutGroup> cutGroupsByID(const QSet<ResNum> &resnums) const;
    QHash<CutGroupID,CutGroup> cutGroupsByID(const QSet<ResID> &resids) const;

    QHash<CutGroupNum,CutGroup> cutGroupsByNum(const QSet<ResNum> &resnums) const;
    QHash<CutGroupNum,CutGroup> cutGroupsByNum(const QSet<ResID> &resids) const;

    CutGroup cutGroup(CutGroupID id) const;
    CutGroup cutGroup(CutGroupNum cgnum) const;

    QVector<CoordGroup> coordGroups() const;

    QHash<CutGroupID,CoordGroup> coordGroupsByID(ResNum resnum) const;
    QHash<CutGroupID,CoordGroup> coordGroupsByID(ResID resid) const;

    QHash<CutGroupNum,CoordGroup> coordGroupsByNum(ResNum resnum) const;
    QHash<CutGroupNum,CoordGroup> coordGroupsByNum(ResID resid) const;

    QHash<CutGroupID,CoordGroup> coordGroups(const QSet<CutGroupID> &cgids) const;
    QHash<CutGroupNum,CoordGroup> coordGroups(const QSet<CutGroupNum> &cgnums) const;

    QHash<CutGroupID,CoordGroup> coordGroupsByID(const QSet<ResNum> &resnums) const;
    QHash<CutGroupID,CoordGroup> coordGroupsByID(const QSet<ResID> &resids) const;

    QHash<CutGroupNum,CoordGroup> coordGroupsByNum(const QSet<ResNum> &resnums) const;
    QHash<CutGroupNum,CoordGroup> coordGroupsByNum(const QSet<ResID> &resids) const;

    CoordGroup coordGroup(CutGroupID id) const;
    CoordGroup coordGroup(CutGroupNum cgnum) const;

    Atom atom(AtomID atomid) const;
    Atom atom(CutGroupID cgid, AtomID atomid) const;
    Atom atom(CutGroupNum cgnum, AtomID atomid) const;
    Atom atom(const CGAtomID &cgatmid) const;
    Atom atom(const CGNumAtomID &cgatmid) const;
    Atom atom(ResNum resnum, AtomID atomid) const;
    Atom atom(const ResNumAtomID &resatomid) const;
    Atom atom(ResID resid, AtomID atomid) const;
    Atom atom(const ResIDAtomID &resatomid) const;
    Atom atom(const AtomIndex &atm) const;
    Atom atom(ResNum resnum, const QString &atomname) const;

    Vector coordinates(AtomID atomid) const;
    Vector coordinates(CutGroupID cgid, AtomID atomid) const;
    Vector coordinates(CutGroupNum cgnum, AtomID atomid) const;
    Vector coordinates(const CGAtomID &cgatomid) const;
    Vector coordinates(const CGNumAtomID &cgatomid) const;
    Vector coordinates(ResNum resnum, AtomID atomid) const;
    Vector coordinates(const ResNumAtomID &resatomid) const;
    Vector coordinates(ResID resid, AtomID atomid) const;
    Vector coordinates(const ResIDAtomID &resatomid) const;
    Vector coordinates(const AtomIndex &atm) const;
    Vector coordinates(ResNum resnum, const QString &atomname) const;

    QVector<Atom> atoms() const;
    QVector<Vector> coordinates() const;

    QHash<AtomID,Atom> atoms(const QSet<AtomID> &atomids) const;
    QHash<CGAtomID,Atom> atoms(const QSet<CGAtomID> &cgatomids) const;
    QHash<CGNumAtomID,Atom> atoms(const QSet<CGNumAtomID> &cgatomids) const;
    QHash<ResNumAtomID,Atom> atoms(const QSet<ResNumAtomID> &resatomids) const;
    QHash<ResIDAtomID,Atom> atoms(const QSet<ResIDAtomID> &resatomids) const;
    QHash<AtomIndex,Atom> atoms(const QSet<AtomIndex> &atoms) const;

    QHash<AtomID,Vector> coordinates(const QSet<AtomID> &atomids) const;
    QHash<CGAtomID,Vector> coordinates(const QSet<CGAtomID> &cgatomids) const;
    QHash<CGNumAtomID,Vector> coordinates(const QSet<CGNumAtomID> &cgatomids) const;
    QHash<ResNumAtomID,Vector> coordinates(const QSet<ResNumAtomID> &resatomids) const;
    QHash<ResIDAtomID,Vector> coordinates(const QSet<ResIDAtomID> &resatomids) const;
    QHash<AtomIndex,Vector> coordinates(const QSet<AtomIndex> &atoms) const;

    QVector<Atom> atoms(CutGroupID cgid) const;
    QHash< CutGroupID,QVector<Atom> > atoms(const QSet<CutGroupID> &cgids) const;

    QVector<Atom> atoms(CutGroupNum cgnum) const;
    QHash< CutGroupNum,QVector<Atom> > atoms(const QSet<CutGroupNum> &cgnums) const;

    QVector<Atom> atoms(ResNum resnum) const;
    QHash< ResNum,QVector<Atom> > atoms(const QSet<ResNum> &resnums) const;

    QVector<Atom> atoms(ResID resid) const;
    QHash< ResID,QVector<Atom> > atoms(const QSet<ResID> &resids) const;

    QVector<Vector> coordinates(CutGroupID cgid) const;
    QHash< CutGroupID,QVector<Vector> >
          coordinates(const QSet<CutGroupID> &cgids) const;

    QVector<Vector> coordinates(CutGroupNum cgnum) const;
    QHash< CutGroupNum,QVector<Vector> >
          coordinates(const QSet<CutGroupNum> &cgnums) const;

    QVector<Vector> coordinates(ResNum resnum) const;
    QHash< ResNum,QVector<Vector> >
          coordinates(const QSet<ResNum> &resnums) const;

    QVector<Vector> coordinates(ResID resid) const;
    QHash< ResID,QVector<Vector> >
          coordinates(const QSet<ResID> &resids) const;

    QString name() const;

    QString residueName(ResNum resnum) const;
    QString residueName(ResID resid) const;

    ResNum residueNumber(ResID resid) const;
    ResNum residueNumber(const QString &resname) const;

    ResID residueID(ResNum resnum) const;

    bool isEmpty() const;
    bool isEmpty(ResNum resnum) const;
    bool isEmpty(ResID resid) const;
    bool isEmpty(CutGroupID cgid) const;
    bool isEmpty(CutGroupNum cgnum) const;

    QVector<ResNum> residueNumbers() const;
    QStringList residueNames() const;

    QVector<ResNum> residueNumbers(const QString &resnam) const;
    QVector<ResNum> residueNumbers(CutGroupID cgid) const;
    QVector<ResNum> residueNumbers(CutGroupNum cgnum) const;

    QHash<ResNum,EditRes> residuesBondedTo(ResNum resnum) const;
    QHash<ResNum,EditRes> residuesBondedTo(ResID resid) const;

    bool contains(CutGroupID cgid) const;
    bool contains(CutGroupNum cgnum) const;
    bool contains(ResNum resnum) const;
    bool contains(ResID resid) const;

    bool contains(ResNum resnum, const QString &atomname) const;
    bool contains(const AtomIndex &atm) const;
    bool contains(CutGroupID cgid, AtomID atomid) const;
    bool contains(CutGroupNum cgnum, AtomID atomid) const;
    bool contains(const CGAtomID &cgatomid) const;
    bool contains(const CGNumAtomID &cgatomid) const;
    bool contains(ResNum resnum, AtomID atomid) const;
    bool contains(const ResNumAtomID &resatomid) const;
    bool contains(ResID resid, AtomID atomid) const;
    bool contains(const ResIDAtomID &resatomid) const;

    bool contains(const Bond &bond) const;

    int nAtoms() const;
    int nAtoms(ResNum resnum) const;
    int nAtoms(ResID resid) const;
    int nAtoms(CutGroupID id) const;
    int nAtoms(CutGroupNum cgnum) const;

    int nResidues() const;

    int nCutGroups() const;

    int nBonds() const;
    int nBonds(ResNum resnum) const;
    int nBonds(ResID resid) const;

    int nInterBonds() const;
    int nInterBonds(ResNum resnum) const;
    int nInterBonds(ResID resid) const;

    int nIntraBonds() const;
    int nIntraBonds(ResNum resnum) const;
    int nIntraBonds(ResID resid) const;

    QStringList atomNames(ResNum resnum) const;
    QStringList atomNames(ResID resid) const;

    SireMaths::Line bond(const Bond &bnd) const;
    SireMaths::Triangle angle(const SireMol::Angle &ang) const;
    SireMaths::Torsion dihedral(const Dihedral &dih) const;
    SireMaths::Torsion improper(const Improper &improper) const;

    double measure(const Bond &bnd) const;
    SireMaths::Angle measure(const SireMol::Angle &ang) const;
    SireMaths::Angle measure(const Dihedral &dih) const;
    SireMaths::Angle measure(const Improper &improper) const;

    double getWeight(const AtomIDGroup &group0, const AtomIDGroup &group1,
                     const WeightFunction &weightfunc) const;
   /////////////////////////////////////////////////////////


   //// Editing functions /////////////////////////
    void setName(const QString &name);
    void setResidueName(ResNum resnum, const QString &name);

    void clean();

    void renumberResidue(ResNum resnum, ResNum newresnum);

    void add(const Atom &atom);
    void add(const Atom &atom, CutGroupNum cgnum);

    void remove(const AtomIndex &atom);

    void add(const Bond &bond);
    void remove(const Bond &bond);

    void removeAllBonds(ResNum resnum);
    void removeAllBonds(ResID resnum);
    void removeAllBonds(const AtomIndex &atom);
    void removeAllBonds();

    void removeAllIntraBonds(ResNum resnum);
    void removeAllIntraBonds(ResID resid);
    void removeAllIntraBonds(const AtomIndex &atom);
    void removeAllIntraBonds();

    void removeAllInterBonds(ResNum resnum);
    void removeAllInterBonds(ResID resid);
    void removeAllInterBonds(const AtomIndex &atom);
    void removeAllInterBonds();

    void addAutoBonds();
    void addAutoBonds(BondAddingFunction &bondfunc);

    void add(ResNum resnum, const QString &resnam);

    void remove(ResNum resnum);
    void remove(ResID resid);
   ////////////////////////////////////////////////


private:
    /** Implicitly shared pointer to the actual data of the molecule */
    QSharedDataPointer<EditMolData> d;
};

}

Q_DECLARE_METATYPE(SireMol::EditMol)

SIRE_END_HEADER

#endif
