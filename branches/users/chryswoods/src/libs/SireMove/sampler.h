#ifndef SIREMOVE_SAMPLER_H
#define SIREMOVE_SAMPLER_H

#include <boost/tuple/tuple.hpp>

#include "SireBase/sharedpolypointer.hpp"

SIRE_BEGIN_HEADER

namespace SireMove
{
class Sampler;
class SamplerBase;
}

QDataStream& operator<<(QDataStream&, const SireMove::SamplerBase&);
QDataStream& operator>>(QDataStream&, SireMove::SamplerBase&);

QDataStream& operator<<(QDataStream&, const SireMove::Sampler&);
QDataStream& operator>>(QDataStream&, SireMove::Sampler&);

namespace SireSystem
{
class SimSystem;
}

namespace SireMol
{
class Molecule;
class MoleculeID;
}

namespace SireMove
{

using boost::tuple;

using SireSystem::SimSystem;

using SireMol::Molecule;
using SireMol::MoleculeID;

/** This is the base class of all Samplers. A Sampler is used
    to pick a random molecule from a MoleculeGroup

    @author Christopher Woods
*/
class SIREMOVE_EXPORT SamplerBase : public QSharedData
{

friend QDataStream& ::operator<<(QDataStream&, const SamplerBase&);
friend QDataStream& ::operator>>(QDataStream&, SamplerBase&);

public:
    SamplerBase();

    SamplerBase(const SamplerBase &other);

    virtual ~SamplerBase();

    SamplerBase& operator=(const SamplerBase &other);

    const char* typeName()
    {
        return "SireMove::SamplerBase";
    }

    virtual const char* what() const=0;
    virtual SamplerBase* clone() const=0;

    virtual tuple<Molecule,double> randomMolecule(const SimSystem &system)=0;

    virtual double probability(const SimSystem &system,
                               MoleculeID molid)=0;
};

/** This is a convienient holder for a SharedPolyPointer<SamplerBase>

    @author Christopher Woods
*/
class Sampler
{

friend QDataStream& ::operator<<(QDataStream&, const Sampler&);
friend QDataStream& ::operator>>(QDataStream&, Sampler&);

public:
    Sampler();

    Sampler(const SamplerBase &sampler);

    Sampler(const Sampler &other);

    ~Sampler();

    Sampler& operator=(const Sampler &other);

    tuple<Molecule,double> randomMolecule(const SimSystem &system);

    double probability(const SimSystem &system, MoleculeID molid);

private:
    /** Implicitly shared pointer to the sampler implementation */
    SharedPolyPointer<SamplerBase> d;
};

/** Return a random molecule from the group, together with the probability
    that this molecule had of being chosen. */
tuple<Molecule,double> Sampler::randomMolecule(const SimSystem &system)
{
    return d->randomMolecule(system);
}

/** Return the probability that the molecule with ID == molid has
    of being chosen in the System 'system' */
double Sampler::probability(const SimSystem &system,
                            MoleculeID molid)
{
    return d->probability(system);
}

}

Q_DECLARE_METATYPE(SireMove::Sampler);

SIRE_END_HEADER

#endif
