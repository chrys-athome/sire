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

#ifndef SIREMOL_MOLECULE_H
#define SIREMOL_MOLECULE_H

#include <QList>
#include <QVector>
#include <QSet>
#include <QHash>

#include "idtypes.h"
#include "atomindex.h"
#include "moleculeview.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class Molecule;
}

QDataStream& operator<<(QDataStream&, const SireMol::Molecule&);
QDataStream& operator>>(QDataStream&, SireMol::Molecule&);

uint qHash(const SireMol::Molecule &molecule);

namespace SireMaths
{
class Vector;
class Matrix;
class Quaternion;
class Line;
class Angle;
class Triangle;
class Torsion;
}

namespace SireBase
{
class Property;
class PropertyBase;
}

namespace SireVol
{
class CoordGroup;
}

namespace SireMol
{

class EditMol;

class NewAtom;
class Residue;
class PartialMolecule;
class CutGroup;
class Atom;

class MoleculeID;
class ResID;
class AtomID;
class ResNum;
class AtomIndex;
class CutGroupID;

class MoleculeProperty;

class Bond;
class Angle;
class Dihedral;
class Improper;

class AtomIDGroup;

class MoleculeVersion;
class MoleculeInfo;

class MoleculeBonds;
class ResidueBonds;
class WeightFunction;

using SireMaths::Vector;
using SireMaths::Matrix;
using SireMaths::Quaternion;

using SireBase::Property;
using SireBase::PropertyBase;

using SireVol::CoordGroup;

namespace detail
{
class MolData;
}

/**
A Molecule represents a complete molecule. This class is merely a view on the underlying
MoleculeView class.

@author Christopher Woods
*/
class SIREMOL_EXPORT Molecule : public MoleculeView
{

friend QDataStream& ::operator<<(QDataStream&, const Molecule&);
friend QDataStream& ::operator>>(QDataStream&, Molecule&);

public:
   ////// Constructors / destructor ////////////////////////
    Molecule();

    Molecule(const MolDataView &molecule);

    Molecule(const Molecule &other);

    ~Molecule();
   /////////////////////////////////////////////////////////


   ////// Dealing with the ID number and version ///////////
     MoleculeID ID() const;
     void setNewID();

     const MoleculeVersion& version() const;

     QString idString() const;
   /////////////////////////////////////////////////////////


   ////// Operators ////////////////////////////////////////
    Molecule& operator=(const MolDataView &other);
    Molecule& operator=(const detail::MolData &moldata);

    bool operator==(const Molecule &other) const;
    bool operator!=(const Molecule &other) const;

    CutGroup operator[](CutGroupID cgid) const;

    Residue operator[](ResID resid) const;
    Residue operator[](ResNum resnum) const;

    Atom operator[](const CGAtomID &cgatomid) const;
    Atom operator[](const IDMolAtom &atomid) const;
   /////////////////////////////////////////////////////////


   ///// Interfacing with Residue //////////////////////////
    Residue residue(ResNum resnum) const;
    Residue residue(ResID resid) const;
    Residue residue(const QString &resname) const;

    QHash<ResNum,Residue> residues() const;
    QHash<ResNum,Residue> residues(const QSet<ResNum> &resnums) const;

    Residue at(ResNum resnum) const;
    Residue at(ResID i) const;
   /////////////////////////////////////////////////////////


   ////// Interfacing with EditMol /////////////////////////
    EditMol edit() const;
   /////////////////////////////////////////////////////////


   ///// Getting and setting properties ////////////////////
    const Property& getProperty(const QString &name) const;

    void setProperty(const QString &name, const PropertyBase &value);
    void addProperty(const QString &name, const PropertyBase &value);

    void setProperty(const QString &name, const Property &value);
    void addProperty(const QString &name, const Property &value);

    void setProperty(const QString &name, const QVariant &value);
    void addProperty(const QString &name, const QVariant &value);

    const QHash<QString,Property>& properties() const;
   /////////////////////////////////////////////////////////


   ///// Querying the molecule /////////////////////////////
    CutGroup at(CutGroupID cgid) const;

    Atom at(const CGAtomID &cgatomid) const;
    Atom at(const IDMolAtom &atomid) const;

    MoleculeBonds connectivity() const;

    ResidueBonds connectivity(ResNum resnum) const;
    ResidueBonds connectivity(ResID resid) const;

    const MoleculeInfo& info() const;

    QVector<CutGroup> cutGroups() const;
    QHash<CutGroupID,CutGroup> cutGroups(ResNum resnum) const;
    QHash<CutGroupID,CutGroup> cutGroups(ResID resid) const;

    CutGroup cutGroup(CutGroupID id) const;

    QVector<CoordGroup> coordGroups() const;
    QHash<CutGroupID,CoordGroup> coordGroups(ResNum resnum) const;
    QHash<CutGroupID,CoordGroup> coordGroups(ResID resid) const;

    QHash<CutGroupID,CoordGroup> coordGroups(const QSet<CutGroupID> &cgids) const;
    QHash<CutGroupID,CoordGroup> coordGroups(const QSet<ResNum> &resnums) const;
    QHash<CutGroupID,CoordGroup> coordGroups(const QSet<ResID> &resids) const;

    CoordGroup coordGroup(CutGroupID id) const;

    Atom atom(const CGAtomID &cgatmid) const;
    Atom atom(const IDMolAtom &atomid) const;

    Vector coordinates(const CGAtomID &cgatomid) const;
    Vector coordinates(const IDMolAtom &atomid) const;

    QVector<Atom> atoms() const;
    QVector<Vector> coordinates() const;

    QHash<AtomID,Atom> atoms(const QSet<AtomID> &atomids) const;
    QHash<CGAtomID,Atom> atoms(const QSet<CGAtomID> &cgatomids) const;
    QHash<ResNumAtomID,Atom> atoms(const QSet<ResNumAtomID> &resatomids) const;
    QHash<ResIDAtomID,Atom> atoms(const QSet<ResIDAtomID> &resatomids) const;
    QHash<AtomIndex,Atom> atoms(const QSet<AtomIndex> &atoms) const;

    QHash<AtomID,Vector> coordinates(const QSet<AtomID> &atomids) const;
    QHash<CGAtomID,Vector> coordinates(const QSet<CGAtomID> &cgatomids) const;
    QHash<ResNumAtomID,Vector> coordinates(const QSet<ResNumAtomID> &resatomids) const;
    QHash<ResIDAtomID,Vector> coordinates(const QSet<ResIDAtomID> &resatomids) const;
    QHash<AtomIndex,Vector> coordinates(const QSet<AtomIndex> &atoms) const;

    QVector<Atom> atoms(CutGroupID cgid) const;
    QHash< CutGroupID,QVector<Atom> > atoms(const QSet<CutGroupID> &cgids) const;

    QVector<Atom> atoms(ResNum resnum) const;
    QHash< ResNum,QVector<Atom> > atoms(const QSet<ResNum> &resnums) const;

    QVector<Atom> atoms(ResID resid) const;
    QHash< ResID,QVector<Atom> > atoms(const QSet<ResID> &resids) const;

    QVector<Vector> coordinates(CutGroupID cgid);
    QHash< CutGroupID,QVector<Vector> >
          coordinates(const QSet<CutGroupID> &cgids) const;

    QVector<Vector> coordinates(ResNum resnum);
    QHash< ResNum,QVector<Vector> >
          coordinates(const QSet<ResNum> &resnums) const;

    QVector<Vector> coordinates(ResID resid);
    QHash< ResID,QVector<Vector> >
          coordinates(const QSet<ResID> &resids) const;

    QString name() const;

    QString residueName(ResNum resnum) const;
    QString residueName(ResID resid) const;

    ResNum residueNumber(ResID resid) const;
    ResNum residueNumber(const QString &resname) const;

    bool isEmpty() const;
    bool isEmpty(ResNum resnum) const;
    bool isEmpty(ResID resid) const;
    bool isEmpty(CutGroupID cgid) const;

    QVector<ResNum> residueNumbers() const;
    QStringList residueNames() const;

    QVector<ResNum> residueNumbers(const QString &resnam) const;
    QVector<ResNum> residueNumbers(CutGroupID cgid) const;

    QHash<ResNum,Residue> residuesBondedTo(ResNum resnum) const;
    QHash<ResNum,Residue> residuesBondedTo(ResID resid) const;

    bool contains(CutGroupID cgid) const;
    bool contains(ResNum resnum) const;
    bool contains(ResID resid) const;

    bool contains(const CGAtomID &cgatomid) const;
    bool contains(const IDMolAtom &atomid) const;

    bool contains(const Bond &bond) const;

    int nAtoms() const;
    int nAtoms(ResNum resnum) const;
    int nAtoms(ResID resid) const;
    int nAtoms(CutGroupID id) const;

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

    void assertSameMolecule(const Molecule &other) const;
    void assertSameMajorVersion(const Molecule &other) const;
    void assertSameVersion(const Molecule &other) const;
};

}

Q_DECLARE_METATYPE(SireMol::Molecule)

SIRE_END_HEADER

#endif
