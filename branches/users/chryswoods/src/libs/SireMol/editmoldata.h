#ifndef SIREMOL_EDITMOLDATA_H
#define SIREMOL_EDITMOLDATA_H
/**
  * @file
  *
  * C++ Interface: EditMol
  *
  * Description:
  * Interface for EditMol
  *
  * Author: Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */

#include <QSharedData>
#include <QString>
#include <QHash>
#include <QSet>
#include <QMap>

#include "idtypes.h"
#include "atomindex.h"
#include "resnum.h"
#include "cutgroupnum.h"
#include "moleculebonds.h"

SIRE_BEGIN_HEADER

namespace SireMaths
{
class Vector;
class Quaternion;
class Line;
class Triangle;
class Torsion;
class Angle;
}

namespace SireMol
{
class EditMolData;
}

class QDataStream;
QDataStream& operator<<(QDataStream&, const SireMol::EditMolData&);
QDataStream& operator>>(QDataStream&, SireMol::EditMolData&);

namespace SireMol
{

class EditMolDataPvt;
typedef boost::shared_ptr< QSharedDataPointer<EditMolDataPvt> > EditMolDataPtr;

class Element;

class EditMol;
class EditRes;

class Atom;
class Bond;
class Angle;
class Dihedral;
class Improper;

class WeightFunction;

class Molecule;
class MoleculeBonds;
class ResidueBonds;
class AtomIDGroup;
class MoleculeSignature;

class EditMolData_ResData;
class EditMolData_AtomData;

using SireMaths::Vector;
using SireMaths::Quaternion;

/**
This class holds the actual data for an EditMol. This class is used internally by EditMol and EditRes, as these two classes are actually merely viewer classes which present different views of EditMolData.

@author Christopher Woods
*/
class EditMolData : public QSharedData
{

friend class EditMolDataPvt;

friend QDataStream& ::operator<<(QDataStream&, const EditMolData&);
friend QDataStream& ::operator>>(QDataStream&, EditMolData&);

public:
    EditMolData(const QString &molname = QObject::tr("Unnamed"));

    EditMolData(const Molecule &mol);

    EditMolData(const EditMolData &other);

    ~EditMolData();

   ////// Dealing with the ID number and version ///////////
     MoleculeID ID() const;
     void setNewID();

     const MoleculeVersion& version() const;
   /////////////////////////////////////////////////////////


   //////// Operators //////////////////////////////
    EditMolData& operator=(const EditMolData &other);

    bool operator==(const EditMolData &other) const;
    bool operator!=(const EditMolData &other) const;
   /////////////////////////////////////////////////


   ////// Interfacing with Molecule ////////////////////////
    Molecule commit();
   /////////////////////////////////////////////////////////


   ///// Transforming between ID types /////////////////////
   ///// (to natural type of EditMol, ResNum/AtomName) /////
    AtomIndex operator[](const AtomIndex &atm) const;
    AtomIndex operator[](AtomID atomid) const;
    AtomIndex operator[](const ResNumAtomID &resatomid) const;
    AtomIndex operator[](const ResIDAtomID &resatomid) const;
    AtomIndex operator[](const CGAtomID &cgatomid) const;
    AtomIndex operator[](const CGNumAtomID &cgatomid) const;

    ResNum operator[](ResNum resnum) const;
    ResNum operator[](ResID resid) const;

    CutGroupNum operator[](CutGroupNum cgnum) const;
    CutGroupNum operator[](CutGroupID cgid) const;
   /////////////////////////////////////////////////////////


   ///// Querying the molecule /////////////////////////////
    AtomIndex at(const AtomIndex &atm) const;
    AtomIndex at(AtomID atomid) const;
    AtomIndex at(const ResNumAtomID &resatomid) const;
    AtomIndex at(const CGAtomID &cgatomid) const;
    AtomIndex at(const CGNumAtomID &cgatomid) const;

    ResNum at(ResID resid) const;
    ResNum at(ResNum resnum) const;

    CutGroupNum at(CutGroupID cgid) const;
    CutGroupNum at(CutGroupNum cgnum) const;

    const MoleculeBonds& connectivity() const;

    ResidueBonds connectivity(ResNum resnum) const;

    MoleculeInfo info() const;

    QVector<CutGroup> cutGroups() const;
    QHash<CutGroupNum,CutGroup> cutGroups(ResNum resnum) const;

    CutGroup cutGroup(CutGroupID id) const;

    QVector<CoordGroup> coordGroups() const;
    QHash<CutGroupNum,CoordGroup> coordGroups(ResNum resnum) const;

    CoordGroup coordGroup(CutGroupNum cgnum) const;

    Atom atom(const AtomIndex &atm) const;
    Vector coordinates(const AtomIndex &atm) const;

    QVector<Atom> atoms() const;
    QVector<Vector> coordinates() const;

    QVector<Atom> atoms(CutGroupNum cgid) const;
    QVector<Atom> atoms(ResNum resnum) const;

    QVector<Vector> coordinates(CutGroupNum cgid) const;
    QVector<Vector> coordinates(ResNum resnum) const;

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


   //////// Editing the molecule ///////////////////
    void setName(const QString &name);
    void setResidueName(ResNum resnum, const QString &name);

    void clean();

    void renumberResidue(ResNum resnum, ResNum newresnum);

    void add(const Atom &atom);
    void remove(const AtomIndex &atom);

    void add(const Bond &bond);
    void remove(const Bond &bond);

    void removeAllBonds(ResNum resnum);
    void removeAllBonds(const AtomIndex &atom);
    void removeAllBonds();

    void add(ResNum resnum, const QString &resnam);
    void add(ResNum resnum, const EditRes &tmpl);
    void add(ResNum resnum, const QString &resnam, const EditRes &tmpl);

    void remove(ResNum resnum);
   /////////////////////////////////////////////////


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
    void translate(ResNum resnum, AtomID atomid, const Vector &delta);
    void translate(ResNum resnum, const QSet<AtomID> &atomids, const Vector &delta);

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
    void rotate(ResNum resnum, AtomID atomid, const Quaternion &quat, const Vector &point);
    void rotate(ResNum resnum, const QSet<AtomID> &atomids,
                const Quaternion &quat, const Vector &point);

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
    void rotate(ResNum resnum, AtomID atomid, const Matrix &rotmat, const Vector &point);
    void rotate(ResNum resnum, const QSet<AtomID> &atomids,
                const Matrix &rotmat, const Vector &point);

    void setCoordinates(CutGroupID cgid, const CoordGroup &newcoords);
    void setCoordinates(const QHash<CutGroupID,CoordGroup> &newcoords);

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

    void setCoordinates(ResNum resnum, AtomID atomid, const Vector &newcoords);
    void setCoordinates(ResNum resnum, const QHash<AtomID,Vector> &newcoords);

    void setCoordinates(ResNum resnum, const QHash<QString,Vector> &newcoords);
   /////////////////////////////////////////////////


   //////// Internal geometry moves ////////////////
    void change(const Bond &bnd, double delta,
                const AtomIDGroup &group0, const AtomIDGroup &group1,
                const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors);

    void change(const SireMol::Angle &ang, const SireMaths::Angle &delta,
                const AtomIDGroup &group0, const AtomIDGroup &group1,
                const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors);

    void change(const Bond &bnd, const SireMaths::Angle &delta,
                const AtomIDGroup &group0, const AtomIDGroup &group1,
                const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors);

    void change(const Improper &improper, const SireMaths::Angle &delta,
                const AtomIDGroup &group0, const AtomIDGroup &group1,
                const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors);
   /////////////////////////////////////////////////


   //////// Assertations ///////////////////////////
    void assertAtomExists(const AtomIndex &atom) const;
    void assertAtomExists(AtomID atomid) const;
    void assertAtomExists(const CGAtomID &cgatomid) const;
    void assertAtomExists(const CGNumAtomID &cgnumatomid) const;
    void assertAtomExists(const ResNumAtomID &resatomid) const;
    void assertAtomExists(const ResIDAtomID &resatomid) const;

    void assertResidueExists(ResNum resnum) const;
    void assertResidueExists(ResID resid) const;

    void assertCutGroupExists(CutGroupID cgid) const;
    void assertCutGroupExists(CutGroupNum cgnum) const;
   /////////////////////////////////////////////////


   //////// Unsafe functions ///////////////////////
    Atom _unsafe_atom(const AtomIndex &atm) const;
    Vector _unsafe_coordinates(const AtomIndex &atm) const;
   /////////////////////////////////////////////////


private:
    const EditMolData_ResData& _unsafe_resdata(ResNum resnum) const;
    EditMolData_ResData& _unsafe_resdata(ResNum resnum) const;

    const EditMolData_AtomData& _unsafe_atomdata(const AtomIndex &atom) const;
    EditMolData_AtomData& _unsafe_atomdata(const AtomIndex &atom);

    /** The name of this molecule */
    QString molnme;

    /** The molecule's ID number */
    MoleculeID molid;

    /** The molecule's version number */
    MoleculeVersion molversion;

    /** Set of all atoms in each residue, indexed by residue number, and sorted
        by the order in which the atoms were added to the residue. */
    QHash<ResNum, EditMolData_ResData> atms;

    /** Sorted list of residue numbers of residues in this molecule
        in the order that they were added to this molecule */
    QList<ResNum> resnums;

    /** The atoms in each CutGroup, indexed by CutGroupNum and with the
        atoms sorted into the order in which they were added to the CutGroup */
    QHash< CutGroupNum, QList<AtomIndex> > cgatoms;

    /** Sorted list of CutGroup numbers of CutGroups in this molecule,
        in the order that they were added to this molecule */
    QList<CutGroupNum> cgnums;

    /** The bonding within the molecule */
    MoleculeBonds molbnds;
};

}

SIRE_END_HEADER

#endif
