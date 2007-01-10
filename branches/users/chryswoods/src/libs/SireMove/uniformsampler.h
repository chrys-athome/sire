#ifndef SIREMOVE_UNIFORMSAMPLER_H
#define SIREMOVE_UNIFORMSAMPLER_H

#include "sampler.h"

#include "SireSystem/moleculegroup.h"
#include "SireMaths/maths.h"

SIRE_BEGIN_HEADER

namespace SireMove
{

using SireSystem::MoleculeGroup;

class SIREMOVE_EXPORT UniformSampler : public SamplerBase
{
public:
    UniformSampler(uint32_t seed = 47382924);

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

protected:
    uint ranint(uint nmax);

private:

    class RanGenerator
    {
    public:
        RanGenerator(uint32_t seed);
        ~RanGenerator();

        uint ranint(uint nmax);

    private:
        /** Mutex to serialise access to the generator */
        QMutex mutex;
        /** Random number generator that generates random
            numbers between 0 and 1 inclusive */
        SireMaths::UniformRand uni_rand_0_1;
    };

    /** Shared pointer to the random number generator of
        this class */

};

}

Q_DECLARE_METATYPE(SireMove::UniformSampler);

SIRE_END_HEADER

#endif
