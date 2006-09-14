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

SIRE_BEGIN_HEADER

namespace SireMol
{
class MoleculeInfo;
}

QDataStream& operator<<(QDataStream&, const SireMol::MoleculeInfo&);
QDataStream& operator>>(QDataStream&, SireMol::MoleculeInfo&);

namespace SireMol
{

class MoleculeInfoPvt;

/**
This class holds all of the metainfo about a molecule. This is information that allows mapping from an atom or residue-based ID to the actual information for the atom. Also it contains all non-coordinate information about the molecule. This information is separated off so that changing the coordinates will not cause any copying of the metainfo. In addition, this metainfo object can be used in other classes, e.g. the parameter table classes, where it can be combined with lists of other parameters.

@author Christopher Woods
*/
class SIREMOL_EXPORT MoleculeInfo
{

friend QDataStream& ::operator<<(QDataStream&, const MoleculeInfo&);
friend QDataStream& ::operator>>(QDataStream&, MoleculeInfo&);

public:
    MoleculeInfo();
    MoleculeInfo( const EditMol &editmol,
                  const ConvertFunction &converter = ResidueCutting() );

    MoleculeInfo(const MoleculeInfo &other);

    ~MoleculeInfo();

    MoleculeInfo& operator=(const MoleculeInfo &other);

    bool operator==(const MoleculeInfo &other) const;
    bool operator!=(const MoleculeInfo &other) const;

    const ResidueInfo& operator[](ResID resid) const;
    const ResidueInfo& operator[](ResNum resnum) const;

    const CGAtomID& operator[](AtomID atmid) const;
    const CGAtomID& operator[](const AtomIndex &atomindex) const;
    const CGAtomID& operator[](const ResNumAtomID &resatomid) const;
    const CGAtomID& operator[](const ResIDAtomID &resatomid) const;

    const ResidueInfo& at(ResID resid) const;
    const ResidueInfo& at(ResNum resnum) const;

    const CGAtomID& at(AtomID atmid) const;
    const CGAtomID& at(const AtomIndex &atomindex) const;
    const CGAtomID& at(const ResNumAtomID &resatomid) const;
    const CGAtomID& at(const ResIDAtomID &resatomid) const;

    const AtomInfo& atom(AtomID atomid) const;
    const AtomInfo& atom(const AtomIndex &atomindex) const;
    const AtomInfo& atom(const ResNumAtomID &rsid) const;
    const AtomInfo& atom(const ResIDAtomID &rsid) const;
    const AtomInfo& atom(const CGAtomID &cgid) const;

    QSet<AtomInfo> getAtoms( const QSet<AtomID> &idxs ) const;
    QSet<AtomInfo> getAtoms( const QSet<AtomIndex> &atms ) const;
    QSet<AtomInfo> getAtoms( const QSet<ResNumAtomID> &rsids) const;
    QSet<AtomInfo> getAtoms( const QSet<ResIDAtomID> &rsids) const;
    QSet<AtomInfo> getAtoms( const QSet<CGAtomID> &cgids ) const;

    const ResidueInfo& residue(AtomID atmid) const;
    const ResidueInfo& residue(ResID resid) const;
    const ResidueInfo& residue(ResNum resnum) const;

    QString name() const;

    bool isEmpty() const;
    bool isNull() const;

    QString toString() const;

    int nResidues() const;

    int nAtoms() const;
    int nAtoms(ResNum resnm) const;

    QVector<ResNum> residueNumbers() const;
    QVector<ResNum> residueNumbers(const QString &resname) const;

    QStringList residueNames() const;

    QHash<CutGroupID,AtomInfoGroup> atomGroups() const;

    bool contains(ResNum resnum) const;
    bool contains(ResID resid) const;
    bool contains(CutGroupID cgid) const;

    bool contains(AtomID atmid) const;
    bool contains(const AtomIndex &atm) const;
    bool contains(const CGAtomID &cgid) const;
    bool contains(const ResNumAtomID &resid) const;
    bool contains(const ResIDAtomID &resid) const;

private:
    /** Implicitly shared pointer to the data of this object */
    QSharedDataPointer<MoleculeInfoPvt> d;
};

}

Q_DECLARE_METATYPE(SireMol::MoleculeInfo)

SIRE_END_HEADER

#endif
