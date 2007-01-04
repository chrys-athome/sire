#ifndef SIREMOL_MOLECULE_H
#define SIREMOL_MOLECULE_H
/**
  * @file
  *
  * C++ Interface: Molecule
  *
  * Description:
  * Interface for Molecule
  *
  * Author: Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */

#include <QSharedDataPointer>

#include <QList>
#include <QVector>
#include <QSet>
#include <QHash>

#include "idtypes.h"
#include "atomindex.h"

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

namespace SireVol
{
class CoordGroup;
}

namespace SireMol
{

class EditMol;

class MoleculeData;
class NewAtom;
class Residue;
class CutGroup;
class Atom;

class MoleculeID;
class ResID;
class AtomID;
class ResNum;
class AtomIndex;
class CutGroupID;

class Property;
class PropertyBase;

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

using SireVol::CoordGroup;

namespace detail
{
class MolData;
}

/**
A Molecule represents a complete molecule. This class is merely a view on the underlying
MoleculeData class.

@author Christopher Woods
*/
class SIREMOL_EXPORT Molecule
{

friend class NewAtom; //so it can see the MoleculeData object
friend class Residue; //so it can call the MoleculeData constructor

friend QDataStream& ::operator<<(QDataStream&, const Molecule&);
friend QDataStream& ::operator>>(QDataStream&, Molecule&);

public:
   ////// Constructors / destructor ////////////////////////
    Molecule();

    Molecule(const Residue &residue);
    Molecule(const NewAtom &atom);

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
    Molecule& operator=(const Molecule &other);
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


   //////// Moving the molecule ////////////////////
    void translate(const Vector &delta);
    void translate(const AtomIDGroup &group, const Vector &delta);
    void translate(const AtomIndex &atom, const Vector &delta);
    void translate(const QSet<AtomIndex> &atoms, const Vector &delta);
    void translate(ResNum resnum, const QStringList &atoms, const Vector &delta);
    void translate(ResNum resnum, const Vector &delta);
    void translate(const QSet<ResNum> &resnums, const Vector &delta);
    void translate(ResID resid, const QStringList &atoms, const Vector &delta);
    void translate(ResID resid, const Vector &delta);
    void translate(const QSet<ResID> &resids, const Vector &delta);
    void translate(CutGroupID cgid, const Vector &delta);
    void translate(const QSet<CutGroupID> &cgids, const Vector &delta);

    void rotate(const Quaternion &quat, const Vector &point);
    void rotate(const AtomIDGroup &group, const Quaternion &quat, const Vector &point);
    void rotate(const AtomIndex &atom, const Quaternion &quat, const Vector &point);
    void rotate(const QSet<AtomIndex> &atoms, const Quaternion &quat, const Vector &point);
    void rotate(ResNum resnum, const QStringList &atoms, const Quaternion &quat,
                const Vector &point);
    void rotate(ResNum resnum, const Quaternion &quat, const Vector &point);
    void rotate(const QSet<ResNum> &resnums, const Quaternion &quat, const Vector &point);
    void rotate(ResID resid, const QStringList &atoms, const Quaternion &quat,
                const Vector &point);
    void rotate(ResID resid, const Quaternion &quat, const Vector &point);
    void rotate(const QSet<ResID> &resids, const Quaternion &quat, const Vector &point);
    void rotate(CutGroupID cgid, const Quaternion &quat, const Vector &point);
    void rotate(const QSet<CutGroupID> &cgids, const Quaternion &quat, const Vector &point);

    void rotate(const Matrix &matrix, const Vector &point);
    void rotate(const AtomIDGroup &group, const Matrix &matrix, const Vector &point);
    void rotate(const AtomIndex &atom, const Matrix &matrix, const Vector &point);
    void rotate(const QSet<AtomIndex> &atoms, const Matrix &matrix, const Vector &point);
    void rotate(ResNum resnum, const QStringList &atoms, const Matrix &matrix,
                const Vector &point);
    void rotate(ResNum resnum, const Matrix &matrix, const Vector &point);
    void rotate(const QSet<ResNum> &resnums, const Matrix &matrix, const Vector &point);
    void rotate(ResID resid, const QStringList &atoms, const Matrix &matrix,
                const Vector &point);
    void rotate(ResID resid, const Matrix &matrix, const Vector &point);
    void rotate(const QSet<ResID> &resids, const Matrix &matrix, const Vector &point);
    void rotate(CutGroupID cgid, const Matrix &matrix, const Vector &point);
    void rotate(const QSet<CutGroupID> &cgids, const Matrix &matrix, const Vector &point);

    void setCoordinates(CutGroupID cgid, const CoordGroup &newcoords);
    void setCoordinates(const QHash<CutGroupID,CoordGroup> &newcoords);
    void setCoordinates(const QVector<CoordGroup> &newcoords);

    void setCoordinates(const QVector<Vector> &newcoords);

    void setCoordinates(CutGroupID cgid, const QVector<Vector> &newcoords);
    void setCoordinates(const QHash< CutGroupID,QVector<Vector> > &newcoords);

    void setCoordinates(ResNum resnum, const QVector<Vector> &newcoords);
    void setCoordinates(const QHash< ResNum,QVector<Vector> > &newcoords);

    void setCoordinates(ResID resid, const QVector<Vector> &newcoords);
    void setCoordinates(const QHash< ResID,QVector<Vector> > &newcoords);

    void setCoordinates(const AtomIndex &atom, const Vector &newcoords);
    void setCoordinates(const QHash<AtomIndex,Vector> &newcoords);

    void setCoordinates(const CGAtomID &cgatomid, const Vector &newcoords);
    void setCoordinates(const QHash<CGAtomID,Vector> &newcoords);

    void setCoordinates(const ResNumAtomID &resatomid, const Vector &newcoords);
    void setCoordinates(const QHash<ResNumAtomID,Vector> &newcoords);

    void setCoordinates(const ResIDAtomID &resatomid, const Vector &newcoords);
    void setCoordinates(const QHash<ResIDAtomID,Vector> &newcoords);
   /////////////////////////////////////////////////


   //////// Internal geometry moves ////////////////
    void change(const Bond &bond, double delta,
                const QSet<AtomIndex> &anchors = QSet<AtomIndex>());
    void change(const Bond &bond, double delta, const WeightFunction &func,
                const QSet<AtomIndex> &anchors = QSet<AtomIndex>());

    void change(const Angle &angle, const SireMaths::Angle &delta,
                const QSet<AtomIndex> &anchors = QSet<AtomIndex>());
    void change(const Angle &angle, const SireMaths::Angle &delta,
                const WeightFunction &func,
                const QSet<AtomIndex> &anchors = QSet<AtomIndex>());

    void change(const Dihedral &dihedral, const SireMaths::Angle &delta,
                const QSet<AtomIndex> &anchors = QSet<AtomIndex>());
    void change(const Dihedral &dihedral, const SireMaths::Angle &delta,
                const WeightFunction &func,
                const QSet<AtomIndex> &anchors = QSet<AtomIndex>());

    void change(const Bond &bond, const SireMaths::Angle &delta,
                const QSet<AtomIndex> &anchors = QSet<AtomIndex>());
    void change(const Bond &bond, const SireMaths::Angle &delta,
                const WeightFunction &func,
                const QSet<AtomIndex> &anchors = QSet<AtomIndex>());

    void change(const Improper &improper, const SireMaths::Angle &delta,
                const QSet<AtomIndex> &anchors = QSet<AtomIndex>());
    void change(const Improper &improper, const SireMaths::Angle &delta,
                const WeightFunction &func,
                const QSet<AtomIndex> &anchors = QSet<AtomIndex>());

    void set(const Bond &bond, double lgth,
             const QSet<AtomIndex> &anchors = QSet<AtomIndex>());
    void set(const Bond &bond, double lgth, const WeightFunction &func,
             const QSet<AtomIndex> &anchors = QSet<AtomIndex>());

    void set(const SireMol::Angle &angle, const SireMaths::Angle &ang,
             const QSet<AtomIndex> &anchors = QSet<AtomIndex>());
    void set(const SireMol::Angle &angle, const SireMaths::Angle &ang,
             const WeightFunction &func,
             const QSet<AtomIndex> &anchors = QSet<AtomIndex>());

    void set(const Dihedral &dihedral, const SireMaths::Angle &ang,
             const QSet<AtomIndex> &anchors = QSet<AtomIndex>());
    void set(const Dihedral &dihedral, const SireMaths::Angle &ang,
             const WeightFunction &func,
             const QSet<AtomIndex> &anchors = QSet<AtomIndex>());

    void setAll(const Dihedral &dihedral, const SireMaths::Angle &ang,
                const QSet<AtomIndex> &anchors = QSet<AtomIndex>());
    void setAll(const Dihedral &dihedral, const SireMaths::Angle &ang,
                const WeightFunction &func,
                const QSet<AtomIndex> &anchors = QSet<AtomIndex>());

    void set(const Improper &improper, const SireMaths::Angle &ang,
             const QSet<AtomIndex> &anchors = QSet<AtomIndex>());
    void set(const Improper &improper, const SireMaths::Angle &ang,
             const WeightFunction &func,
             const QSet<AtomIndex> &anchors = QSet<AtomIndex>());
   /////////////////////////////////////////////////

    void assertSameMolecule(const Molecule &other) const;
    void assertSameMajorVersion(const Molecule &other) const;
    void assertSameVersion(const Molecule &other) const;

private:
    /** Implicitly shared pointer to the data for this molecule */
    QSharedDataPointer<MoleculeData> d;
};

}

Q_DECLARE_METATYPE(SireMol::Molecule)

SIRE_END_HEADER

#endif
