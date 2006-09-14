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

#include <QSharedDataPointer>

#include "atomindex.h"
#include "residueidset.h"

#include "moleculebonds.h"
#include "moleculeversion.h"
#include "convertfunction.h"

#include "moleculeinfo.h"

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

namespace SireVol
{
class CoordGroup;
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

class MoleculeDataPvt;

class Molecule;
class Residue;
class ResidueBonds;

class Bond;
class Angle;
class Dihedral;
class Improper;

class AtomIDGroup;
class WeightFunction;
class ConvertFunction;

using SireVol::CoordGroup;

/**
This class holds the shared molecule data for the Molecule and Residue classes (which are both just views on this MolculeData class). This is very similar to the EditMolData class, on which EditMol and EditRes are merely views.

@author Christopher Woods
*/
class SIREMOL_EXPORT MoleculeData
{

friend QDataStream& ::operator<<(QDataStream&, const MoleculeData&);
friend QDataStream& ::operator>>(QDataStream&, MoleculeData&);

public:
    MoleculeData();

    MoleculeData(const EditMol &editmol,
                 const ConvertFunction &convertfunc);

    MoleculeData(const MoleculeData &other);

    ~MoleculeData();

   ////// Dealing with the ID number and version ///////////
     MoleculeID ID() const;
     void setNewID();

     const MoleculeVersion& version() const;
     void incrementMajorVersion();
     void incrementMinorVersion();
   /////////////////////////////////////////////////////////


   ////// Operators ////////////////////////////////////////
    MoleculeData& operator=(const MoleculeData &other);

    bool operator==(const MoleculeData &other) const;
    bool operator!=(const MoleculeData &other) const;
   /////////////////////////////////////////////////////////


   ///// Interfacing with Molecule and Residue /////////////
    Molecule molecule() const;

    Residue residue(ResNum resnum) const;
    Residue residue(ResID resid) const;
    Residue residue(const QString &resname) const;

    void merge(const Residue &residue);

    QHash<ResNum,Residue> residues() const;

    Residue at(ResNum resnum) const;
    Residue at(ResID i) const;
   /////////////////////////////////////////////////////////


   ///// Querying the molecule /////////////////////////////
    MoleculeBonds connectivity() const;
    ResidueBonds connectivity(ResNum resnum) const;

    const MoleculeInfo& info() const;

    QVector<Atom> atoms() const;
    QVector<Atom> atoms(ResNum resnum) const;
    QVector<Atom> atoms(ResID resid) const;

    QHash<CutGroupID,CutGroup> cutGroups() const;
    QHash<CutGroupID,CutGroup> cutGroups(ResNum resnum) const;
    QHash<CutGroupID,CutGroup> cutGroups(ResID resid) const;

    CutGroup cutGroup(CutGroupID id) const;

    QHash<CutGroupID,CoordGroup> coordinates() const;
    QHash<CutGroupID,CoordGroup> coordinates(ResNum resnum) const;
    QHash<CutGroupID,CoordGroup> coordinates(ResID resid) const;

    CoordGroup coordinates(CutGroupID id) const;

    Atom atom(CutGroupID cgid, AtomID atomid) const;
    Atom atom(const CGAtomID &cgatmid) const;
    Atom atom(ResNum resnum, AtomID atomid) const;
    Atom atom(ResID resid, AtomID atomid) const;
    Atom atom(const AtomIndex &atm) const;

    Vector coordinates(CutGroupID cgid, AtomID atomid) const;
    Vector coordinates(const CGAtomID &cgatomid) const;
    Vector coordinates(ResNum resnum, AtomID atomid) const;
    Vector coordinates(ResID resid, AtomID atomid) const;
    Vector coordinates(const AtomIndex &atm) const;

    const QString& name() const;

    QString residueName(ResNum resnum) const;
    QString residueName(ResID resid) const;

    ResNum residueNumber(ResID resid) const;

    bool isEmpty() const;
    bool isEmpty(ResNum resnum) const;
    bool isEmpty(ResID resid) const;

    QVector<ResNum> residueNumbers() const;
    QStringList residueNames() const;

    QVector<ResNum> residueNumbers(const QString &resnam) const;

    QHash<ResNum,Residue> residuesBondedTo(ResNum resnum) const;
    QHash<ResNum,Residue> residuesBondedTo(ResID resid) const;

    bool contains(CutGroupID cgid) const;
    bool contains(ResNum resnum) const;
    bool contains(ResID resid) const;

    bool contains(const AtomIndex &atm) const;
    bool contains(CutGroupID cgid, AtomID atomid) const;
    bool contains(ResNum resnum, AtomID atomid) const;
    bool contains(ResID resid, AtomID atomid) const;

    bool contains(const Bond &bond) const;

    int nAtoms() const;
    int nAtoms(ResNum resnum) const;
    int nAtoms(ResID resid) const;
    int nAtoms(CutGroupID id) const;

    int nResidues() const;

    int nCutGroups() const;

    QStringList atomNames(ResNum resnum) const;
    QStringList atomNames(ResID resid) const;

    void checkResidue(ResNum) const;
    void checkResidue(ResID) const;

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
    void translate(ResID resid, const QStringList &atoms, const Vector &delta);
    void translate(ResID resid, const Vector &delta);

    template<class T>
    void translate(const T &atoms, const Vector &delta);

    void rotate(const Quaternion &quat, const Vector &point);
    void rotate(const AtomIDGroup &group, const Quaternion &quat, const Vector &point);
    void rotate(const AtomIndex &atom, const Quaternion &quat, const Vector &point);
    void rotate(const QSet<AtomIndex> &atoms, const Quaternion &quat, const Vector &point);
    void rotate(ResNum resnum, const QStringList &atoms, const Quaternion &quat,
                const Vector &point);
    void rotate(ResNum resnum, const Quaternion &quat, const Vector &point);
    void rotate(ResID resid, const QStringList &atoms, const Quaternion &quat,
                const Vector &point);
    void rotate(ResID resid, const Quaternion &quat, const Vector &point);

    template<class T>
    void rotate(const T &atoms, const Quaternion &quat, const Vector &point);

    void rotate(const Matrix &matrix, const Vector &point);
    void rotate(const AtomIDGroup &group, const Matrix &matrix, const Vector &point);
    void rotate(const AtomIndex &atom, const Matrix &matrix, const Vector &point);
    void rotate(const QSet<AtomIndex> &atoms, const Matrix &matrix, const Vector &point);
    void rotate(const AtomIndexSet &atoms, const Matrix &matrix, const Vector &point);
    void rotate(ResNum resnum, const QStringList &atoms, const Matrix &matrix,
                const Vector &point);
    void rotate(ResNum resnum, const Matrix &matrix, const Vector &point);
    void rotate(ResID resid, const QStringList &atoms, const Matrix &matrix,
                const Vector &point);
    void rotate(ResID resid, const Matrix &matrix, const Vector &point);

    template<class T>
    void rotate(const T &atoms, const Matrix &matrix, const Vector &point);
   /////////////////////////////////////////////////


   //////// Internal geometry moves ////////////////
    void change(const Bond &bnd, double delta,
                const AtomIDGroup &group0, const AtomIDGroup &group1,
                const WeightFunction &weightfunc, const AtomIndexSet &anchors);

    void change(const SireMol::Angle &ang, const SireMaths::Angle &delta,
                const AtomIDGroup &group0, const AtomIDGroup &group1,
                const WeightFunction &weightfunc, const AtomIndexSet &anchors);

    void change(const Bond &bnd, const SireMaths::Angle &delta,
                const AtomIDGroup &group0, const AtomIDGroup &group1,
                const WeightFunction &weightfunc, const AtomIndexSet &anchors);

    void change(const Improper &improper, const SireMaths::Angle &delta,
                const AtomIDGroup &group0, const AtomIDGroup &group1,
                const WeightFunction &weightfunc, const AtomIndexSet &anchors);
   /////////////////////////////////////////////////

private:
    /** Implicitly shared pointer to the private implementation of this class */
    QSharedDataPointer<MoleculeDataPvt> d;
};

/** Translate a whole load of objects in 'atoms' by 'delta'

    Exceptions will be thrown if any of the objects are not in this molecule.

    \throw SireMol::missing_atom
    \throw SireMol::missing_residue
    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
    \throw SireError::invalid_key
*/
template<class T>
void MoleculeData::translate(const T &atoms, const Vector &delta)
{
    //save the old state so that it can be restored if an exception
    //is thrown
    QSharedDataPointer<MoleculeDataPvt> old_d = d;

    try
    {
        for (T::const_iterator it = atoms.begin();
             it != atoms.end();
             ++it)
        {
            this->translate(*it, delta);
        }
    }
    catch(...)
    {
        //restore the old state, then rethrow the exception
        d = old_d;
        throw;
    }
}

/** Rotate a whole load of objects in 'atoms' by the matrix 'matrix'
    around the point 'point'

    Exceptions will be thrown if any of the objects are not in this molecule.

    \throw SireMol::missing_atom
    \throw SireMol::missing_residue
    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
    \throw SireError::invalid_key
*/
template<class T>
void MoleculeData::rotate(const T &atoms, const Matrix &matrix, const Vector &point)
{
    //save the old state so that it can be restored if an exception
    //is thrown
    QSharedDataPointer<MoleculeDataPvt> old_d = d;

    try
    {
        for (T::const_iterator it = atoms.begin();
             it != atoms.end();
             ++it)
        {
            this->rotate(*it, matrix, point);
        }
    }
    catch(...)
    {
        //restore the old state, then rethrow the exception
        d = old_d;
        throw;
    }
}

/** Rotate a whole load of objects in 'atoms' by the quaternion 'quat' about
    the point 'point'

    Exceptions will be thrown if any of the objects are not in this molecule.

    \throw SireMol::missing_atom
    \throw SireMol::missing_residue
    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
    \throw SireError::invalid_key
*/
template<class T>
void MoleculeData::rotate(const T &atoms, const Quaternion &quat, const Vector &point)
{
    this->rotate( atoms, quat.toMatrix(), point );
}

}

SIRE_END_HEADER

#endif
