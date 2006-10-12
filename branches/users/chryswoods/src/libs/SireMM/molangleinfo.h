#ifndef SIREMM_MOLANGLEINFO_H
#define SIREMM_MOLANGLEINFO_H

#include "internaldatatypes.h"
#include "molinternalinfo.hpp"

#include "internalgenerator.hpp"

SIRE_BEGIN_HEADER

namespace SireMM
{
class MolAngleInfo;
}

QDataStream& operator<<(QDataStream&, const SireMM::MolAngleInfo&);
QDataStream& operator>>(QDataStream&, SireMM::MolAngleInfo&);

namespace SireMM
{

class ResAngleInfo;

/**
This is the Angle version of MolBondInfo - namely it holds all of the metadata about the angles that are contained in a molecule, and assigns to each angle a unique GroupIndexID value (and provides the functionality necessary to map between GroupIndexID values and angles).

@author Christopher Woods
*/
class SIREMM_EXPORT MolAngleInfo : public MolInternalInfo<Angle>
{
public:
    MolAngleInfo();
    MolAngleInfo(const MoleculeInfo &molinfo);
    MolAngleInfo(const Molecule &mol, 
                 const InternalGenerator<MolAngleInfo> &anglegenerator);

    MolAngleInfo(const MolAngleInfo &other);

    ~MolAngleInfo();

    GroupIndexID addAngle(const Angle &angle);
    void removeAngle(const Angle &angle);
    void removeAngle(const GroupIndexID &id);

    template<class C>
    void addAngles(const C &angles);
    template<class C>
    void removeAngles(const C &angles);

    int nAngles() const;
    int nAngles(GroupID group) const;
    int nAngles(ResNum resnum) const;
    int nAngles(ResNum res0, ResNum res1) const;
    int nAngles(ResNum res0, ResNum res1, ResNum res2) const;
    
    int nInterAngles() const;
    int nIntraAngles() const;
    
    int nIntraAngles(ResNum resnum) const;
    int nInterAngles(ResNum resnum) const;

    bool residuesAngled(ResNum res0, ResNum res1) const;
    bool residuesAngled(ResNum res0, ResNum res1, ResNum res2) const;

    const_iterator angles() const;
    const_iterator angles(ResNum resnum) const;
    const_iterator angles(ResNum res0, ResNum res1) const;
    const_iterator angles(ResNum res0, ResNum res1, ResNum res2) const;
    
    const_iterator intraAngles() const;
    const_iterator interAngles() const;
    
    const_iterator intraAngles(ResNum resnum) const;
    const_iterator interAngles(ResNum resnum) const;

    ResAngleInfo residue(ResNum resnum) const;

    template<class C>
    const_iterator angles(const C &resnums) const;
    template<class C>
    const_iterator intraAngles(const C &resnums) const;
    template<class C>
    const_iterator interAngles(const C &resnums) const;

};

/** Add a whole load of angles to the molecule */
template<class C>
SIRE_OUTOFLINE_TEMPLATE
void MolAngleInfo::addAngles(const C &angles)
{
    for (typename C::const_iterator it = angles.begin();
         it != angles.end();
         ++it)
    {
        this->addInternal( *it );
    }
}

/** Remove a whole load of angles from the molecule. */
template<class C>
SIRE_OUTOFLINE_TEMPLATE
void MolAngleInfo::removeAngles(const C &angles)
{
    for (typename C::const_iterator it = angles.begin();
         it != angles.end();
         ++it)
    {
        this->removeInternal( *it );
    }
}

/** Return an iterator over the angles in the residues whose residue 
    numbers are in 'resnums'.
    
    This returns an iterator pointing to the first angle, or an 
    invalid iterator if there are no angles to iterate over.
*/
template<class C>
SIRE_INLINE_TEMPLATE
MolAngleInfo::const_iterator MolAngleInfo::angles(const C &resnums) const
{
    return this->internals(resnums);
}

/** Return an iterator over the intra-reside angles in the residues 
    whose residue numbers are in 'resnums'.
    
    This returns an iterator pointing to the first angle, or an 
    invalid iterator if there are no angles to iterate over.
*/
template<class C>
SIRE_INLINE_TEMPLATE
MolAngleInfo::const_iterator MolAngleInfo::intraAngles(const C &resnums) const
{
    return this->intraInternals(resnums);
}

/** Return an iterator over the inter-reside angles in the residues 
    whose residue numbers are in 'resnums'.
    
    This returns an iterator pointing to the first angle, or an 
    invalid iterator if there are no angles to iterate over.
*/
template<class C>
SIRE_INLINE_TEMPLATE
MolAngleInfo::const_iterator MolAngleInfo::interAngles(const C &resnums) const
{
    return this->interInternals(resnums);
}

}

Q_DECLARE_METATYPE(SireMM::MolAngleInfo)

SIRE_END_HEADER

#endif
