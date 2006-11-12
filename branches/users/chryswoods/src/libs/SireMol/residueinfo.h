#ifndef SIREMOL_RESIDUEINFO_H
#define SIREMOL_RESIDUEINFO_H
/**
  * @file
  *
  * C++ Interface: ResidueInfo
  *
  * Description:
  * Interface to ResidueInfo
  *
  * Author: Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */

#include <QSharedDataPointer>
#include <QHash>
#include <QStringList>

#include "sireglobal.h"

#include "idtypes.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class ResidueInfo;
}

QDataStream& operator<<(QDataStream&, const SireMol::ResidueInfo&);
QDataStream& operator>>(QDataStream&, SireMol::ResidueInfo&);

namespace SireMol
{

class ResidueInfoPvt;
class MoleculeInfoPvt;

class AtomIndex;
class AtomInfo;
class AtomInfoGroup;

class AtomID;
class ResID;
class ResNum;
class CutGroupID;

class CGAtomID;

class EditMolData;

/**
  * This class holds all of the metainfo for a residue. This class is designed
  * to be used with MoleculeInfo. This holds the name and number of the residue,
  * together with the names and metainfo for all of the atoms in the molecule.
  *
  * @author Christopher Woods
  *
  */
class SIREMOL_EXPORT ResidueInfo
{

friend class MoleculeInfoPvt;  // so can call private constructor

friend QDataStream& ::operator<<(QDataStream&, const ResidueInfo&);
friend QDataStream& ::operator>>(QDataStream&, ResidueInfo&);

public:
    ResidueInfo();

    ResidueInfo(const ResidueInfo &other);

    ~ResidueInfo();

   ////// Operators ////////////////////////////////////////
    ResidueInfo& operator=(const ResidueInfo &other);

    bool operator==(const ResidueInfo &other) const;
    bool operator!=(const ResidueInfo &other) const;

    const CGAtomID& operator[](AtomID atmid) const;
    const CGAtomID& operator[](const QString &atmname) const;

    const AtomInfoGroup& operator[](CutGroupID cgid) const;
    const AtomInfoGroup& operator[](CutGroupNum cgnum) const;
   /////////////////////////////////////////////////////////

   ///// Querying the molecule /////////////////////////////
    const CGAtomID& at(AtomID atmid) const;
    const CGAtomID& at(const QString &atmname) const;

    const AtomInfoGroup& at(CutGroupID cgid) const;
    const AtomInfoGroup& at(CutGroupNum cgnum) const;

    const AtomInfo& atom(AtomID i) const;
    const AtomInfo& atom(const QString &atmname) const;
    const AtomInfo& atom(const CGAtomID &cgid) const;
    const AtomInfo& atom(const CGNumAtomID &cgid) const;

    const AtomInfoGroup& atomGroup(CutGroupID cgid) const;
    const AtomInfoGroup& atomGroup(CutGroupNum cgnum) const;

    QHash<CutGroupID,AtomInfoGroup> atomGroups() const;
    QHash<CutGroupID,AtomInfoGroup> atomGroups(const QSet<CutGroupID> &cgids) const;
    QHash<CutGroupNum,AtomInfoGroup> atomGroups(const QSet<CutGroupNum> &cgnums) const;

    QVector<AtomInfo> atoms() const;

    QHash<AtomID,AtomInfo> atoms( const QSet<AtomID> &idxs ) const;
    QHash<CGAtomID,AtomInfo> atoms( const QSet<CGAtomID> &cgids ) const;
    QHash<CGNumAtomID,AtomInfo> atoms( const QSet<CGNumAtomID> &cgids ) const;
    QHash<QString,AtomInfo> atoms( const QSet<QString> &atms ) const;

    QVector<AtomInfo> atoms(CutGroupID cgid) const;
    QVector<AtomInfo> atoms(CutGroupNum cgnum) const;

    QHash< CutGroupID, QVector<AtomInfo> > atoms(const QSet<CutGroupID> &cgids) const;
    QHash< CutGroupNum, QVector<AtomInfo> > atoms(const QSet<CutGroupNum> &cgnums) const;

    const QVector<CGAtomID>& indicies() const;
    const QSet<CutGroupID>& cutGroupIDs() const;
    QSet<CutGroupNum> cutGroupNums() const;

    QString toString() const;

    QString name() const;
    QString resName() const;

    ResNum number() const;
    ResNum resNum() const;

    int nCutGroups() const;

    int nAtoms() const;
    int nAtoms(CutGroupID cgid) const;
    int nAtoms(CutGroupNum cgnum) const;

    QString atomName(AtomID atomid) const;
    QHash<AtomID,QString> atomNames(const QSet<AtomID> &atomids) const;

    QStringList atomNames() const;

    bool contains(const QString &atmname) const;
    bool contains(const AtomIndex &atm) const;
    bool contains(AtomID atm) const;
    bool contains(CutGroupID cgid) const;
    bool contains(CutGroupID cgid, AtomID atomid) const;
    bool contains(const CGAtomID &cgid) const;
    bool contains(CutGroupNum cgnum) const;
    bool contains(CutGroupNum cgnum, AtomID atomid) const;
    bool contains(const CGNumAtomID &cgid) const;

    bool isEmpty() const;
    bool isEmpty(CutGroupID cgid) const;
    bool isEmpty(CutGroupNum cgnum) const;

    void assertAtomExists(const QString &atomname) const;
    void assertAtomExists(AtomID atomid) const;

    void assertSameResidue(const AtomIndex &atom) const;
    void assertSameResidue(const QSet<AtomIndex> &atoms) const;
    void assertSameResidue(CutGroupID cgid) const;
    void assertSameResidue(CutGroupNum cgnum) const;

    void assertNAtoms(int n) const;
   /////////////////////////////////////////////////////////

private:
    ResidueInfo(ResNum resnum, const EditMolData &moldata, 
                const QHash<CutGroupID,AtomInfoGroup> &atominfos);

    /** Implicitly shared pointer to the data for this object */
    QSharedDataPointer<ResidueInfoPvt> d;
};

}

Q_DECLARE_METATYPE(SireMol::ResidueInfo)

SIRE_END_HEADER

#endif
