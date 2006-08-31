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

SIRE_BEGIN_HEADER

namespace SireMol
{
class ResidueInfo;
}

QDataStream& operator<<(QDataStream&, const SireMol::ResidueInfo&);
QDataStream& operator>>(QDataStream&, SireMol::ResidueInfo&);

namespace SireMol
{

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
    
    ResidueInfo& operator=(const ResidueInfo &other);
    
    bool operator==(const ResidueInfo &other) const;
    bool operator!=(const ResidueInfo &other) const;
    
    const CGAtomID& operator[](AtomID atmid) const;
    const CGAtomID& operator[](const QString &atmname) const;
    
    const CGAtomID& at(AtomID atmid) const;
    const CGAtomID& at(const QString &atmname) const;
    
    QString toString() const;
    
    int nAtoms() const;
    
    QString name() const;
    QString resName() const;
    
    ResNum number() const;
    ResNum resNum() const;
    
    const AtomInfo& atom(AtomID i) const;
    const AtomInfo& atom(const QString &atmname) const;
    const AtomInfo& atom(const CGAtomID &cgid) const;
    
    QStringList atomNames() const;
    
    QVector<CutGroupID> cutGroupIDs() const;
    QHash<CutGroupID,AtomInfoGroup> atomGroups() const;
    
    QVector<CGAtomID> indicies() const;
    
    bool contains(const QString &atmname) const;
    bool contains(const AtomIndex &atm) const;
    bool contains(AtomID atm) const;
    bool contains(CutGroupID cgid) const;
    bool contains(const CGAtomID &cgid) const;
    
private:
    /** Implicitly shared pointer to the data for this object */
    QSharedDataPointer<ResidueInfoPvt> d;
};

}

Q_DECLARE_METATYPE(SireMol::ResidueInfo)

SIRE_END_HEADER

#endif
