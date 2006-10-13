#ifndef SIREMM_MOLBONDINFO_H
#define SIREMM_MOLBONDINFO_H

#include "internaldatatypes.h"
#include "molinternalinfo.hpp"

#include "internalgenerator.hpp"

SIRE_BEGIN_HEADER

namespace SireMM
{
class MolBondInfo;
}

QDataStream& operator<<(QDataStream&, const SireMM::MolBondInfo&);
QDataStream& operator>>(QDataStream&, SireMM::MolBondInfo&);

namespace SireMM
{

class ResBondInfo;

/**
This class is used to map from a bond into an index in a bond array. It performs the same sort of job as MoleculeCGInfo does, and as MolAngleInfo and MolDihedralInfo do.

This class does nothing much itself - it is just a nice view of MolInternalInfo<Bond>

@author Christopher Woods
*/
class SIREMM_EXPORT MolBondInfo : public MolInternalInfo<Bond>
{
public:
    MolBondInfo();
    MolBondInfo(const MoleculeInfo &molinfo);
    MolBondInfo(const Molecule &mol, const InternalGenerator<MolBondInfo> &bondgenerator);

    MolBondInfo(const MolBondInfo &other);

    ~MolBondInfo();

    GroupIndexID addBond(const Bond &bond);

    void removeBond(const Bond &bond);
    void removeBond(const GroupIndexID &id);

    Bond getBond(const GroupIndexID &id) const;

    template<class C>
    void addBonds(const C &bonds);
    template<class C>
    void removeBonds(const C &bonds);

    int nBonds() const;
    int nBonds(GroupID group) const;
    int nBonds(ResNum resnum) const;
    int nBonds(ResNum res0, ResNum res1) const;

    int nInterBonds() const;
    int nIntraBonds() const;

    int nIntraBonds(ResNum resnum) const;
    int nInterBonds(ResNum resnum) const;

    bool residuesBonded(ResNum res0, ResNum res1) const;

    const_iterator bonds() const;
    const_iterator bonds(ResNum resnum) const;
    const_iterator bonds(ResNum res0, ResNum res1) const;

    const_iterator intraBonds() const;
    const_iterator interBonds() const;

    const_iterator intraBonds(ResNum resnum) const;
    const_iterator interBonds(ResNum resnum) const;

    ResBondInfo residue(ResNum resnum) const;

    template<class C>
    const_iterator bonds(const C &resnums) const;
    template<class C>
    const_iterator intraBonds(const C &resnums) const;
    template<class C>
    const_iterator interBonds(const C &resnums) const;
};

/** Add a whole load of bonds to the molecule */
template<class C>
SIRE_INLINE_TEMPLATE
void MolBondInfo::addBonds(const C &bonds)
{
    this->addInternals(bonds);
}

/** Remove a whole load of bonds from the molecule */
template<class C>
SIRE_INLINE_TEMPLATE
void MolBondInfo::removeBonds(const C &bonds)
{
    this->removeInternals(bonds);
}

/** Return an iterator over all of the bonds in the residues
    whose residue numbers are in 'resnums'. The iterator is
    initially positioned on the first bond, while an invalid
    iterator is returned if there are no bonds involving these
    residues. */
template<class C>
SIRE_INLINE_TEMPLATE
MolBondInfo::const_iterator MolBondInfo::bonds(const C &resnums) const
{
    return this->internals(resnums);
}

/** Return an iterator over all of the intra-residue bonds in the residues
    whose residue numbers are in 'resnums'. The iterator is
    initially positioned on the first bond, while an invalid
    iterator is returned if there are no bonds involving these
    residues. */
template<class C>
SIRE_INLINE_TEMPLATE
MolBondInfo::const_iterator MolBondInfo::intraBonds(const C &resnums) const
{
    return this->intraInternals(resnums);
}

/** Return an iterator over all of the inter-residue bonds in the residues
    whose residue numbers are in 'resnums'. The iterator is
    initially positioned on the first bond, while an invalid
    iterator is returned if there are no bonds involving these
    residues. */
template<class C>
SIRE_INLINE_TEMPLATE
MolBondInfo::const_iterator MolBondInfo::interBonds(const C &resnums) const
{
    return this->interInternals(resnums);
}

}

Q_DECLARE_METATYPE(SireMM::MolBondInfo)

SIRE_END_HEADER

#endif
