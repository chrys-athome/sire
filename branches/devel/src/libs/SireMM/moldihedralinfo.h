#ifndef SIREMM_MOLDIHEDRALINFO_H
#define SIREMM_MOLDIHEDRALINFO_H

#include "internaldatatypes.h"
#include "molinternalinfo.hpp"

#include "internalgenerator.hpp"

SIRE_BEGIN_HEADER

namespace SireMM
{
class MolDihedralInfo;
}

QDataStream& operator<<(QDataStream&, const SireMM::MolDihedralInfo&);
QDataStream& operator>>(QDataStream&, SireMM::MolDihedralInfo&);

namespace SireMM
{

class ResDihedralInfo;

/**
This is the Dihedral version of MolBondInfo - namely it holds all of the metadata about the dihedrals that are contained in a molecule, and assigns to each dihedral a unique GroupIndexIF value (and provides the functionality necessary to map between GroupIndexID values and dihedrals).

@author Christopher Woods
*/
class SIREMM_EXPORT MolDihedralInfo : public MolInternalInfo<Dihedral>
{
public:
    MolDihedralInfo();
    MolDihedralInfo(const MoleculeInfo &molinfo);
    MolDihedralInfo(const Molecule &mol,
                    const InternalGenerator<MolDihedralInfo> &dihedralgenerator);

    MolDihedralInfo(const MolDihedralInfo &other);

    ~MolDihedralInfo();

    GroupIndexID addDihedral(const Dihedral &dihedral);
    void removeDihedral(const Dihedral &dihedral);
    void removeDihedral(const GroupIndexID &id);

    template<class C>
    void addDihedrals(const C &dihedrals);
    template<class C>
    void removeDihedrals(const C &dihedrals);

    int nDihedrals() const;
    int nDihedrals(GroupID group) const;
    int nDihedrals(ResNum resnum) const;
    int nDihedrals(ResNum res0, ResNum res1) const;
    int nDihedrals(ResNum res0, ResNum res1, ResNum res2) const;
    int nDihedrals(ResNum res0, ResNum res1,
                   ResNum res2, ResNum res3) const;

    int nInterDihedrals() const;
    int nIntraDihedrals() const;

    int nIntraDihedrals(ResNum resnum) const;
    int nInterDihedrals(ResNum resnum) const;

    bool residuesDihedraled(ResNum res0, ResNum res1) const;
    bool residuesDihedraled(ResNum res0, ResNum res1, ResNum res2) const;
    bool residuesDihedraled(ResNum res0, ResNum res1,
                            ResNum res2, ResNum res3) const;

    const_iterator dihedrals() const;
    const_iterator dihedrals(ResNum resnum) const;
    const_iterator dihedrals(ResNum res0, ResNum res1) const;
    const_iterator dihedrals(ResNum res0, ResNum res1, ResNum res2) const;
    const_iterator dihedrals(ResNum res0, ResNum res1,
                             ResNum res2, ResNum res3) const;

    const_iterator intraDihedrals() const;
    const_iterator interDihedrals() const;

    const_iterator intraDihedrals(ResNum resnum) const;
    const_iterator interDihedrals(ResNum resnum) const;

    ResDihedralInfo residue(ResNum resnum) const;

    template<class C>
    const_iterator dihedrals(const C &resnums) const;
    template<class C>
    const_iterator intraDihedrals(const C &resnums) const;
    template<class C>
    const_iterator interDihedrals(const C &resnums) const;

};

/** Add a whole load of dihedrals to the molecule */
template<class C>
SIRE_OUTOFLINE_TEMPLATE
void MolDihedralInfo::addDihedrals(const C &dihedrals)
{
    for (typename C::const_iterator it = dihedrals.begin();
         it != dihedrals.end();
         ++it)
    {
        this->addInternal( *it );
    }
}

/** Remove a whole load of dihedrals from the molecule. */
template<class C>
SIRE_OUTOFLINE_TEMPLATE
void MolDihedralInfo::removeDihedrals(const C &dihedrals)
{
    for (typename C::const_iterator it = dihedrals.begin();
         it != dihedrals.end();
         ++it)
    {
        this->removeInternal( *it );
    }
}

/** Return an iterator over the dihedrals in the residues whose residue
    numbers are in 'resnums'.

    This returns an iterator pointing to the first dihedral, or an
    invalid iterator if there are no dihedrals to iterate over.
*/
template<class C>
SIRE_INLINE_TEMPLATE
MolDihedralInfo::const_iterator MolDihedralInfo::dihedrals(const C &resnums) const
{
    return this->internals(resnums);
}

/** Return an iterator over the intra-reside dihedrals in the residues
    whose residue numbers are in 'resnums'.

    This returns an iterator pointing to the first dihedral, or an
    invalid iterator if there are no dihedrals to iterate over.
*/
template<class C>
SIRE_INLINE_TEMPLATE
MolDihedralInfo::const_iterator MolDihedralInfo::intraDihedrals(const C &resnums) const
{
    return this->intraInternals(resnums);
}

/** Return an iterator over the inter-reside dihedrals in the residues
    whose residue numbers are in 'resnums'.

    This returns an iterator pointing to the first dihedral, or an
    invalid iterator if there are no dihedrals to iterate over.
*/
template<class C>
SIRE_INLINE_TEMPLATE
MolDihedralInfo::const_iterator MolDihedralInfo::interDihedrals(const C &resnums) const
{
    return this->interInternals(resnums);
}

}

Q_DECLARE_METATYPE(SireMM::MolDihedralInfo)

SIRE_END_HEADER

#endif
