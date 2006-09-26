#ifndef SIREMOL_MOLECULEDATA_H
#define SIREMOL_MOLECULEDATA_H
/**
  * @file
  *
  * C++ Interface: MoleculeData
  *
  * Description:
  * Interface for MoleculeData
  *
  * Author: Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */

#include <QSharedData>

#include <QVector>
#include <QHash>
#include <QSet>
#include <QMutex>

#include "moleculeid.h"
#include "moleculeversion.h"
#include "moleculeinfo.h"
#include "moleculebonds.h"

#include "SireVol/coordgroup.h"

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
class MoleculeData;
}

class QDataStream;
QDataStream& operator<<(QDataStream&, const SireMol::MoleculeData&);
QDataStream& operator>>(QDataStream&, SireMol::MoleculeData&);

namespace SireMol
{

class Molecule;
class Residue;
class ResidueBonds;

class AtomID;
class ResNum;
class ResID;
class CutGroupID;
class MoleculeID;

class ResNumAtomID;
class ResIDAtomID;
class CGAtomID;

class Atom;
class Bond;
class Angle;
class Dihedral;
class Improper;

class AtomIDGroup;
class WeightFunction;
class ConvertFunction;

class CutGroup;

namespace detail{ class MoveWorkspace;}

using SireVol::CoordGroup;

using SireMaths::Vector;
using SireMaths::Matrix;
using SireMaths::Quaternion;
using SireMaths::Line;
using SireMaths::Triangle;
using SireMaths::Torsion;

/**
This class holds the shared molecule data for the Molecule and Residue classes
(which are both just views on this MolculeData class). This is very similar to
the EditMolData class, on which EditMol and EditRes are merely views.

This class is not part of the standard API of the program and should not
be used in your own code. This class is designed only for use with
Molecule and Residue.

@author Christopher Woods
*/
class MoleculeData : public QSharedData
{

friend QDataStream& ::operator<<(QDataStream&, const MoleculeData&);
friend QDataStream& ::operator>>(QDataStream&, MoleculeData&);

public:
   ////// Constructors / destructor ////////////////////////
    MoleculeData();

    MoleculeData(const EditMol &editmol,
                 const ConvertFunction &convertfunc);

    MoleculeData(const MoleculeData &other);

    ~MoleculeData();
   /////////////////////////////////////////////////////////


   ////// Dealing with the ID number and version ///////////
     MoleculeID ID() const;
     void setNewID();

     const MoleculeVersion& version() const;
   /////////////////////////////////////////////////////////


   ////// Operators ////////////////////////////////////////
    MoleculeData& operator=(const MoleculeData &other);

    bool operator==(const MoleculeData &other) const;
    bool operator!=(const MoleculeData &other) const;
   /////////////////////////////////////////////////////////


   ////// Interfacing with EditMol /////////////////////////
    EditMol toEditMol() const;
    void update(const EditMol &editmol, const ConvertFunction &convertfunc);
   /////////////////////////////////////////////////////////


   ///// Querying the molecule /////////////////////////////
    CutGroup at(CutGroupID cgid) const;

    Atom at(AtomID atomid) const;
    Atom at(const ResNumAtomID &resatomid) const;
    Atom at(const ResIDAtomID &resatomid) const;
    Atom at(const CGAtomID &cgatomid) const;
    Atom at(const AtomIndex &atm) const;

    const MoleculeBonds& connectivity() const;

    ResidueBonds connectivity(ResNum resnum) const;
    ResidueBonds connectivity(ResID resid) const;

    const MoleculeInfo& info() const;

    QVector<Atom> atoms() const;
    QVector<Atom> atoms(ResNum resnum) const;
    QVector<Atom> atoms(ResID resid) const;

    QVector<CutGroup> cutGroups() const;
    QHash<CutGroupID,CutGroup> cutGroups(ResNum resnum) const;
    QHash<CutGroupID,CutGroup> cutGroups(ResID resid) const;

    CutGroup cutGroup(CutGroupID id) const;

    QVector<CoordGroup> coordGroups() const;
    QHash<CutGroupID,CoordGroup> coordGroups(ResNum resnum) const;
    QHash<CutGroupID,CoordGroup> coordGroups(ResID resid) const;

    CoordGroup coordGroup(CutGroupID id) const;

    Atom atom(AtomID atomid) const;
    Atom atom(CutGroupID cgid, AtomID atomid) const;
    Atom atom(const CGAtomID &cgatmid) const;
    Atom atom(ResNum resnum, AtomID atomid) const;
    Atom atom(const ResNumAtomID &resatomid) const;
    Atom atom(ResID resid, AtomID atomid) const;
    Atom atom(const ResIDAtomID &resatomid) const;
    Atom atom(const AtomIndex &atm) const;
    Atom atom(ResNum resnum, const QString &atomname) const;

    Vector coordinates(CutGroupID cgid, AtomID atomid) const;
    Vector coordinates(const CGAtomID &cgatomid) const;
    Vector coordinates(ResNum resnum, AtomID atomid) const;
    Vector coordinates(const ResNumAtomID &resatomid) const;
    Vector coordinates(ResID resid, AtomID atomid) const;
    Vector coordinates(const ResIDAtomID &resatomid) const;
    Vector coordinates(const AtomIndex &atm) const;
    Vector coordinates(ResNum resnum, const QString &atomname) const;

    QHash<CGAtomID,Vector> coordinates(const QSet<CGAtomID> &cgatomids) const;
    QHash<ResNumAtomID,Vector> coordinates(const QSet<ResNumAtomID> &resatomids) const;
    QHash<ResIDAtomID,Vector> coordinates(const QSet<ResIDAtomID> &resatomids) const;
    QHash<AtomIndex,Vector> coordinates(const QSet<AtomIndex> &atoms) const;

    QVector<Vector> coordinates(CutGroupID cgid) const;
    QHash< CutGroupID,QVector<Vector> >
          coordinates(const QSet<CutGroupID> &cgids) const;

    QVector<Vector> coordinates(ResNum resnum) const;
    QHash< ResNum,QVector<Vector> >
          coordinates(const QSet<ResNum> &resnums) const;

    QVector<Vector> coordinates(ResID resid) const;
    QHash< ResID,QVector<Vector> >
          coordinates(const QSet<ResID> &resids) const;

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

    void setCoordinates(const Residue &residue);

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

   static QSharedDataPointer<MoleculeData> null();

private:
    static MoleculeID getNewID();
    static QMutex idmutex;
    static MoleculeID lastid;

    void incrementMajorVersion();
    void incrementMinorVersion();

    void translate(const ResidueInfo &resinfo, const QStringList &atoms,
                   const Vector &delta);
    void translate(const ResidueInfo &resinfo, const Vector &delta);

    void translate(const ResidueInfo &resinfo, const QStringList &atoms,
                   const Vector &delta, detail::MoveWorkspace &ws) const;
    void translate(const ResidueInfo &resinfo,
                   const Vector &delta, detail::MoveWorkspace &ws) const;

    void rotate(const ResidueInfo &resinfo, const QStringList &atoms,
                const Quaternion &quat, const Vector &point);
    void rotate(const ResidueInfo &resinfo, const Quaternion &quat,
                const Vector &point);

    void rotate(const ResidueInfo &resinfo, const QStringList &atoms,
                const Quaternion &quat, const Vector &point,
                detail::MoveWorkspace &ws) const;
    void rotate(const ResidueInfo &resinfo, const Quaternion &quat,
                const Vector &point, detail::MoveWorkspace &ws) const;

    void rotate(const ResidueInfo &resinfo, const QStringList &atoms,
                const Matrix &matrix, const Vector &point);
    void rotate(const ResidueInfo &resinfo, const Matrix &matrix,
                const Vector &point);

    void rotate(const ResidueInfo &resinfo, const QStringList &atoms,
                const Matrix &matrix, const Vector &point,
                detail::MoveWorkspace &ws) const;
    void rotate(const ResidueInfo &resinfo, const Matrix &matrix,
                const Vector &point, detail::MoveWorkspace &ws) const;

    void translate(const Vector &delta, detail::MoveWorkspace &ws) const;
    void translate(const AtomIDGroup &group, const Vector &delta,
                   detail::MoveWorkspace &ws) const;
    void translate(const AtomIndex &atom, const Vector &delta,
                   detail::MoveWorkspace &ws) const;
    void translate(const QSet<AtomIndex> &atoms, const Vector &delta,
                   detail::MoveWorkspace &ws) const;
    void translate(ResNum resnum, const QStringList &atoms, const Vector &delta,
                   detail::MoveWorkspace &ws) const;
    void translate(ResNum resnum, const Vector &delta,
                   detail::MoveWorkspace &ws) const;
    void translate(const QSet<ResNum> &resnums, const Vector &delta,
                   detail::MoveWorkspace &ws) const;
    void translate(ResID resid, const QStringList &atoms, const Vector &delta,
                   detail::MoveWorkspace &ws) const;
    void translate(ResID resid, const Vector &delta,
                   detail::MoveWorkspace &ws) const;
    void translate(const QSet<ResID> &resids, const Vector &delta,
                   detail::MoveWorkspace &ws) const;
    void translate(CutGroupID cgid, const Vector &delta,
                   detail::MoveWorkspace &ws) const;
    void translate(const QSet<CutGroupID> &cgids, const Vector &delta,
                   detail::MoveWorkspace &ws) const;

    void rotate(const Quaternion &quat, const Vector &point,
                detail::MoveWorkspace &ws) const;
    void rotate(const AtomIDGroup &group, const Quaternion &quat, const Vector &point,
                detail::MoveWorkspace &ws) const;
    void rotate(const AtomIndex &atom, const Quaternion &quat, const Vector &point,
                detail::MoveWorkspace &ws) const;
    void rotate(const QSet<AtomIndex> &atoms, const Quaternion &quat, const Vector &point,
                detail::MoveWorkspace &ws) const;
    void rotate(ResNum resnum, const QStringList &atoms, const Quaternion &quat,
                const Vector &point, detail::MoveWorkspace &ws) const;
    void rotate(ResNum resnum, const Quaternion &quat, const Vector &point,
                detail::MoveWorkspace &ws) const;
    void rotate(const QSet<ResNum> &resnums, const Quaternion &quat, const Vector &point,
                detail::MoveWorkspace &ws) const;
    void rotate(ResID resid, const QStringList &atoms, const Quaternion &quat,
                const Vector &point, detail::MoveWorkspace &ws) const;
    void rotate(ResID resid, const Quaternion &quat, const Vector &point,
                detail::MoveWorkspace &ws) const;
    void rotate(const QSet<ResID> &resids, const Quaternion &quat, const Vector &point,
                detail::MoveWorkspace &ws) const;
    void rotate(CutGroupID cgid, const Quaternion &quat, const Vector &point,
                detail::MoveWorkspace &ws) const;
    void rotate(const QSet<CutGroupID> &cgids, const Quaternion &quat, const Vector &point,
                detail::MoveWorkspace &ws) const;

    void rotate(const Matrix &matrix, const Vector &point,
                detail::MoveWorkspace &ws) const;
    void rotate(const AtomIDGroup &group, const Matrix &matrix, const Vector &point,
                detail::MoveWorkspace &ws) const;
    void rotate(const AtomIndex &atom, const Matrix &matrix, const Vector &point,
                detail::MoveWorkspace &ws) const;
    void rotate(const QSet<AtomIndex> &atoms, const Matrix &matrix, const Vector &point,
                detail::MoveWorkspace &ws) const;
    void rotate(ResNum resnum, const QStringList &atoms, const Matrix &matrix,
                const Vector &point, detail::MoveWorkspace &ws) const;
    void rotate(ResNum resnum, const Matrix &matrix, const Vector &point,
                detail::MoveWorkspace &ws) const;
    void rotate(const QSet<ResNum> &resnums, const Matrix &matrix, const Vector &point,
                detail::MoveWorkspace &ws) const;
    void rotate(ResID resid, const QStringList &atoms, const Matrix &matrix,
                const Vector &point, detail::MoveWorkspace &ws) const;
    void rotate(ResID resid, const Matrix &matrix, const Vector &point,
                detail::MoveWorkspace &ws) const;
    void rotate(const QSet<ResID> &resids, const Matrix &matrix, const Vector &point,
                detail::MoveWorkspace &ws) const;
    void rotate(CutGroupID cgid, const Matrix &matrix, const Vector &point,
                detail::MoveWorkspace &ws) const;
    void rotate(const QSet<CutGroupID> &cgids, const Matrix &matrix, const Vector &point,
                detail::MoveWorkspace &ws) const;

    QHash<CutGroupID,CutGroup> cutGroups(const ResidueInfo &resinfo) const;
    QHash<CutGroupID,CoordGroup> coordGroups(const ResidueInfo &resinfo) const;

    /** ID number used to identify the molecule */
    MoleculeID _id;

    /** The version number of the metadata */
    MoleculeVersion _molversion;

    /** The metainfo about the molecule - this contains the names of the molecule,
        residue and all atoms, and additional metainfo about all of the residues
        and atoms. This object may also be used to map from atom or residue IDs
        to CGAtomIDs (which are used to lookup the coordinates) */
    MoleculeInfo _molinfo;

    /** The connectivity of this molecule */
    MoleculeBonds _molbonds;

    /** The coordinates of all of the atoms in this molecule, arranged into
        CoordGroups, with the index into the vector being the CutGroupID
        of the CutGroup to which these coordinates belong. */
    QVector<CoordGroup> _coords;

};

}

SIRE_END_HEADER

#endif
