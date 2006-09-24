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

#include <QHash>
#include <QStringList>

#include "sireglobal.h"

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

class AtomIndex;
class AtomInfo;
class AtomInfoGroup;

class AtomID;
class ResID;
class ResNum;
class CutGroupID;

class CGAtomID;

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
   /////////////////////////////////////////////////////////

   ///// Querying the molecule /////////////////////////////
    const CGAtomID& at(AtomID atmid) const;
    const CGAtomID& at(const QString &atmname) const;
  
    const AtomInfoGroup& at(CutGroupID cgid) const;

    const AtomInfo& atom(AtomID i) const;
    const AtomInfo& atom(const QString &atmname) const;
    const AtomInfo& atom(const CGAtomID &cgid) const;

    const AtomInfoGroup& atomGroup(CutGroupID cgid) const;
    
    QHash<CutGroupID,AtomInfoGroup> atomGroups() const;
    QHash<CutGroupID,AtomInfoGroup> atomGroups(const QSet<CutGroupID> &cgids) const;
    
    QVector<AtomInfo> atoms() const;
    
    QHash<AtomID,AtomInfo> atoms( const QSet<AtomID> &idxs ) const;
    QHash<CGAtomID,AtomInfo> atoms( const QSet<CGAtomID> &cgids ) const;
    QHash<QString,AtomInfo> atoms( const QSet<QString> &atms ) const;

    QVector<AtomInfo> atoms(CutGroupID cgid) const;
    QHash< CutGroupID, QVector<AtomInfo> > atoms(const QSet<CutGroupID> &cgids) const;

    const QVector<CGAtomID>& indicies() const;
    const QVector<CutGroupID>& cutGroupIDs() const;

    QString toString() const;

    QString name() const;
    QString resName() const;

    ResNum number() const;
    ResNum resNum() const;

    int nCutGroups() const;

    int nAtoms() const;
    int nAtoms(CutGroupID cgid) const;

    QStringList atomNames() const;

    bool contains(const QString &atmname) const;
    bool contains(const AtomIndex &atm) const;
    bool contains(AtomID atm) const;
    bool contains(CutGroupID cgid) const;
    bool contains(CutGroupID cgid, AtomID atomid) const;
    bool contains(const CGAtomID &cgid) const;

    bool isEmpty() const;
    bool isEmpty(CutGroupID cgid) const;

    void assertAtomExists(const QString &atomname) const;
    void assertAtomExists(AtomID atomid) const;
   /////////////////////////////////////////////////////////

private:
    /** Implicitly shared pointer to the data for this object */
    QSharedDataPointer<ResidueInfoPvt> d;
};

}

Q_DECLARE_METATYPE(SireMol::ResidueInfo)

SIRE_END_HEADER

#endif
