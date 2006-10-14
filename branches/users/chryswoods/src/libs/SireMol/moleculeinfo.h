#ifndef SIREMOL_MOLECULEINFO_H
#define SIREMOL_MOLECULEINFO_H
/**
  * @file
  *
  * C++ Interface: MoleculeInfo
  *
  * Description:
  * Interface to MoleculeInfo
  *
  * Author: Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */

#include <QSharedDataPointer>

#include <QHash>
#include <QSet>

#include "idtypes.h"
#include "residuecutting.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class MoleculeInfo;
}

QDataStream& operator<<(QDataStream&, const SireMol::MoleculeInfo&);
QDataStream& operator>>(QDataStream&, SireMol::MoleculeInfo&);

namespace SireMol
{

class AtomInfo;
class AtomIndex;
class ResidueInfo;
class AtomInfoGroup;

class EditMolData;

class MoleculeInfoPvt;

/**
This class holds all of the metainfo about a molecule. This is information that allows mapping from an atom or residue-based ID to the actual information for the atom. Also it contains all non-coordinate information about the molecule. This information is separated off so that changing the coordinates will not cause any copying of the metainfo. In addition, this metainfo object can be used in other classes, e.g. the parameter table classes, where it can be combined with lists of other parameters.

The other main purpose of this class is to allow the many different ways of indexing an atom
(e.g. by residue number and atom name, or by index into the molecule) to be mapped efficiently
to the indexing method used internally by the code - namely CGAtomID.

@author Christopher Woods
*/
class SIREMOL_EXPORT MoleculeInfo
{

friend QDataStream& ::operator<<(QDataStream&, const MoleculeInfo&);
friend QDataStream& ::operator>>(QDataStream&, MoleculeInfo&);

public:
    MoleculeInfo();
    MoleculeInfo(const EditMolData &moldata);

    MoleculeInfo(const MoleculeInfo &other);

    ~MoleculeInfo();

   ////// Operators ////////////////////////////////////////
    MoleculeInfo& operator=(const MoleculeInfo &other);

    bool operator==(const MoleculeInfo &other) const;
    bool operator!=(const MoleculeInfo &other) const;

    const AtomInfoGroup& operator[](CutGroupID cgid) const;
    const AtomInfoGroup& operator[](CutGroupNum cgnum) const;

    const ResidueInfo& operator[](ResID resid) const;
    const ResidueInfo& operator[](ResNum resnum) const;

    const CGAtomID& operator[](AtomID atmid) const;
    const CGAtomID& operator[](const AtomIndex &atomindex) const;
    const CGAtomID& operator[](const ResNumAtomID &resatomid) const;
    const CGAtomID& operator[](const ResIDAtomID &resatomid) const;
    const CGAtomID& operator[](const CGAtomID &cgatomid) const;
    const CGAtomID& operator[](const CGNumAtomID &cgatomid) const;
   /////////////////////////////////////////////////////////

   ///// Querying the molecule /////////////////////////////
    const AtomInfoGroup& at(CutGroupID cgid) const;
    const AtomInfoGroup& at(CutGroupNum cgnum) const;

    const ResidueInfo& at(ResID resid) const;
    const ResidueInfo& at(ResNum resnum) const;

    const CGAtomID& at(AtomID atmid) const;
    const CGAtomID& at(const AtomIndex &atomindex) const;
    const CGAtomID& at(const ResNumAtomID &resatomid) const;
    const CGAtomID& at(const ResIDAtomID &resatomid) const;
    const CGAtomID& at(const CGNumAtomID &cgatomid) const;
    const CGAtomID& at(const CGAtomID &cgatomid) const;

    const AtomInfo& atom(AtomID atomid) const;
    const AtomInfo& atom(const AtomIndex &atomindex) const;
    const AtomInfo& atom(const ResNumAtomID &rsid) const;
    const AtomInfo& atom(const ResIDAtomID &rsid) const;
    const AtomInfo& atom(const CGAtomID &cgid) const;
    const AtomInfo& atom(const CGNumAtomID &cgid) const;

    const AtomInfoGroup& atomGroup(CutGroupID cgid) const;
    const AtomInfoGroup& atomGroup(CutGroupNum cgnum) const;

    QHash<CutGroupID,AtomInfoGroup> atomGroups() const;
    QHash<CutGroupID,AtomInfoGroup> atomGroups(ResNum resnum) const;
    QHash<CutGroupID,AtomInfoGroup> atomGroups(ResID resid) const;

    QHash<CutGroupID,AtomInfoGroup> atomGroups(const QSet<CutGroupID> &cgids) const;
    QHash<CutGroupID,AtomInfoGroup> atomGroups(const QSet<ResNum> &resnums) const;
    QHash<CutGroupID,AtomInfoGroup> atomGroups(const QSet<ResID> &resids) const;

    QVector<AtomInfo> atoms() const;

    QHash<AtomID,AtomInfo> atoms( const QSet<AtomID> &idxs ) const;
    QHash<CGAtomID,AtomInfo> atoms( const QSet<CGAtomID> &cgids ) const;
    QHash<CGNumAtomID,AtomInfo> atoms( const QSet<CGNumAtomID> &cgids) const;
    QHash<ResNumAtomID,AtomInfo> atoms( const QSet<ResNumAtomID> &rsids) const;
    QHash<ResIDAtomID,AtomInfo> atoms( const QSet<ResIDAtomID> &rsids) const;
    QHash<AtomIndex,AtomInfo> atoms( const QSet<AtomIndex> &atms ) const;

    QVector<AtomInfo> atoms(CutGroupID cgid) const;
    QVector<AtomInfo> atoms(CutGroupNum cgnum) const;
    QHash< CutGroupID, QVector<AtomInfo> > atoms(const QSet<CutGroupID> &cgids) const;
    QHash< CutGroupNum, QVector<AtomInfo> > atoms(const QSet<CutGroupNum> &cgnums) const;

    QVector<AtomInfo> atoms(ResNum resnum) const;
    QHash< ResNum, QVector<AtomInfo> > atoms(const QSet<ResNum> &resnums) const;

    QVector<AtomInfo> atoms(ResID resid) const;
    QHash< ResID, QVector<AtomInfo> > atoms(const QSet<ResID> &resids) const;

    const ResidueInfo& residue(AtomID atmid) const;
    const ResidueInfo& residue(ResID resid) const;
    const ResidueInfo& residue(ResNum resnum) const;

    QString toString() const;

    QString name() const;

    QString residueName(ResNum resnum) const;
    QString residueName(ResID resid) const;

    ResNum residueNumber(ResID resid) const;
    ResNum residueNumber(const QString &resname) const;

    int nResidues() const;
    int nCutGroups() const;

    CutGroupID cutGroupID(CutGroupNum cgnum) const;
    CutGroupNum cutGroupNum(CutGroupID cgid) const;

    int nAtoms() const;
    int nAtoms(ResNum resnm) const;
    int nAtoms(ResID resid) const;
    int nAtoms(CutGroupID cgid) const;
    int nAtoms(CutGroupNum cgnum) const;

    QVector<CutGroupNum> cutGroupNums() const;

    QVector<ResNum> residueNumbers() const;
    QVector<ResNum> residueNumbers(const QString &resname) const;
    QVector<ResNum> residueNumbers(CutGroupID cgid) const;
    QVector<ResNum> residueNumbers(CutGroupNum cgnum) const;

    QStringList residueNames() const;

    QStringList atomNames(ResNum resnum) const;
    QStringList atomNames(ResID resid) const;

    bool contains(CutGroupID cgid) const;
    bool contains(CutGroupNum cgnum) const;
    bool contains(ResNum resnum) const;
    bool contains(ResID resid) const;

    bool contains(ResNum resnum, const QString &atomname) const;
    bool contains(const AtomIndex &atm) const;
    bool contains(CutGroupID cgid, AtomID atomid) const;
    bool contains(const CGAtomID &cgatomid) const;
    bool contains(CutGroupNum cgnum, AtomID atomid) const;
    bool contains(const CGNumAtomID &cgatomid) const;
    bool contains(ResNum resnum, AtomID atomid) const;
    bool contains(const ResNumAtomID &resatomid) const;
    bool contains(ResID resid, AtomID atomid) const;
    bool contains(const ResIDAtomID &resatomid) const;

    bool isEmpty() const;
    bool isEmpty(ResNum resnum) const;
    bool isEmpty(ResID resid) const;
    bool isEmpty(CutGroupID cgid) const;
    bool isEmpty(CutGroupNum cgnum) const;

    void assertResidueExists(ResNum resnum) const;
    void assertResidueExists(ResID resid) const;

    void assertCutGroupExists(CutGroupID cgid) const;
    void assertCutGroupExists(CutGroupNum cgnum) const;

    void assertAtomExists(AtomID atomid) const;
    void assertAtomExists(const AtomIndex &atom) const;
    void assertAtomExists(const CGAtomID &cgatomid) const;
    void assertAtomExists(const CGNumAtomID &cgatomid) const;
    void assertAtomExists(const ResNumAtomID &resatomid) const;
    void assertAtomExists(const ResIDAtomID &resatomid) const;

    void assertNAtoms(int nats) const;
   /////////////////////////////////////////////////////////

private:
    /** Implicitly shared pointer to the data of this object */
    QSharedDataPointer<MoleculeInfoPvt> d;
};

}

Q_DECLARE_METATYPE(SireMol::MoleculeInfo)

SIRE_END_HEADER

#endif
