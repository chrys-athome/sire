#ifndef SIREMOVE_UNIFORMSAMPLER_H
#define SIREMOVE_UNIFORMSAMPLER_H

#include "sampler.h"

#include "SireSystem/moleculegroup.h"

SIRE_BEGIN_HEADER

namespace SireMove
{

using SireSystem::MoleculeGroup;

/** This class is used to pick contained Molecules, Residues
    or Atoms at random (uniformly distributed).
    
    @author Christopher Woods
*/
class SIREMOVE_EXPORT UniformSampler : public SamplerBase
{
public:
    UniformSampler();
    UniformSampler(const RanGenerator &rangenerator);

    UniformSampler(const UniformSampler &other);

    ~UniformSampler();

    UniformSampler& operator=(const UniformSampler &other);

    static const char* typeName()
    {
        return "SireMove::UniformSampler";
    }

    const char* what() const
    {
        return UniformSampler::typeName();
    }

    UniformSampler* clone() const
    {
        return new UniformSampler(*this);
    }

    tuple<Molecule,double> randomMolecule(const MoleculeGroup &group);
    tuple<Residue,double> randomResidue(const MoleculeGroup &group);
    tuple<NewAtom,double> randomAtom(const MoleculeGroup &group);

    double probability(const MoleculeGroup &group, const Molecule &molecule);
    double probability(const MoleculeGroup &group, const Residue &residue);
    double probability(const MoleculeGroup &group, const NewAtom &atom);

private:
    tuple<Molecule,double> _pvt_randomMolecule(const MoleculeGroup &group, 
                                               uint nmols);
};

}

Q_DECLARE_METATYPE(SireMove::UniformSampler);

SIRE_END_HEADER

#endif
