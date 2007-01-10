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
class MoleculeGroup;
}

namespace SireMol
{
class Molecule;
class Residue;
class NewAtom;
}

namespace SireMove
{

using boost::tuple;

using SireSystem::MoleculeGroup;

using SireMol::Molecule;
using SireMol::Residue;
using SireMol::NewAtom;

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

    virtual tuple<Molecule,double> randomMolecule(const MoleculeGroup &group)=0;
    virtual tuple<Residue,double> randomResidue(const MoleculeGroup &group)=0;
    virtual tuple<NewAtom,double> randomAtom(const MoleculeGroup &group)=0;

    virtual double probability(const MoleculeGroup &group,
                               const Molecule &molecule)=0;

    virtual double probability(const MoleculeGroup &group,
                               const Residue &residue)=0;

    virtual double probability(const MoleculeGroup &group,
                               const NewAtom &atom)=0;
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

    tuple<Molecule,double> randomMolecule(const MoleculeGroup &group);
    tuple<Residue,double> randomResidue(const MoleculeGroup &group);
    tuple<NewAtom,double> randomAtom(const MoleculeGroup &group);

    double probability(const MoleculeGroup &group, const Molecule &molecule);
    double probability(const MoleculeGroup &group, const Residue &residue);
    double probability(const MoleculeGroup &group, const NewAtom &atom);

private:
    /** Implicitly shared pointer to the sampler implementation */
    SharedPolyPointer<SamplerBase> d;
};

/** Return a random molecule from the group, together with the probability
    that this molecule had of being chosen. */
inline tuple<Molecule,double> Sampler::randomMolecule(const MoleculeGroup &group)
{
    return d->randomMolecule(group);
}

/** Return a random residue from the group, together with the probability
    that this residue had of being chosen. */
inline tuple<Residue,double> Sampler::randomResidue(const MoleculeGroup &group)
{
    return d->randomResidue(group);
}

/** Return a random atom from the group, together with the probability
    that this atom had of being chosen. */
inline tuple<NewAtom,double> Sampler::randomAtom(const MoleculeGroup &group)
{
    return d->randomAtom(group);
}

/** Return the probability that the molecule 'molecule' has
    of being chosen in the System 'system' */
inline double Sampler::probability(const MoleculeGroup &group,
                                   const Molecule &molecule)
{
    return d->probability(group, molecule);
}

/** Return the probability that the residue 'residue' has
    of being chosen in the System 'system' */
inline double Sampler::probability(const MoleculeGroup &group,
                                   const Residue &residue)
{
    return d->probability(group, residue);
}

/** Return the probability that the atom 'atom' has
    of being chosen in the System 'system' */
inline double Sampler::probability(const MoleculeGroup &group,
                                   const NewAtom &atom)
{
    return d->probability(group, atom);
}

}

Q_DECLARE_METATYPE(SireMove::Sampler);

SIRE_END_HEADER

#endif
