/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007  Christopher Woods
  *
  *  This program is free software; you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation; either version 2 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program; if not, write to the Free Software
  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  *  For full details of the license please see the COPYING file
  *  that should have come with this distribution.
  *
  *  You can contact the authors via the developer's mailing list
  *  at http://siremol.org
  *
\*********************************************/

#include "sampler.h"
#include "uniformsampler.h"

#include "SireMol/molecule.h"
#include "SireMol/residue.h"
#include "SireMol/newatom.h"
#include "SireMol/moleculegroup.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMove;
using namespace SireBase;
using namespace SireStream;

//////////
////////// Implementation of SamplerBase
//////////

static const RegisterMetaType<SamplerBase> r_samplerbase(MAGIC_ONLY,
                                                         "SireMove::SamplerBase");

/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, const SamplerBase&)
{
    writeHeader(ds, r_samplerbase, 0);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, SamplerBase&)
{
    VersionID v = readHeader(ds, r_samplerbase);

    if (v != 0)
        throw version_error(v, "0", r_samplerbase, CODELOC);

    return ds;
}

/** Constructor */
SamplerBase::SamplerBase() : QSharedData()
{}

/** Construct with a specified random number generator */
SamplerBase::SamplerBase(const RanGenerator &generator)
            : QSharedData(), rangen(generator)
{}

/** Copy constructor */
SamplerBase::SamplerBase(const SamplerBase &other) 
            : QSharedData(), rangen(other.rangen)
{}

/** Destructor */
SamplerBase::~SamplerBase()
{}

/** Copy assignment */
SamplerBase& SamplerBase::operator=(const SamplerBase &other)
{
    rangen = other.rangen;

    return *this;
}

/** Set the random number generator used by this sampler */
void SamplerBase::setGenerator(const RanGenerator &generator)
{
    rangen = generator;
}

/** Return a const reference to the random number generator 
    used by this sampler */
const RanGenerator& SamplerBase::generator() const
{
    return rangen;
}

//////////
////////// Implementation of Sampler
//////////

static const RegisterMetaType<Sampler> r_sampler;

/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, const Sampler &sampler)
{
    writeHeader(ds, r_sampler, 1);

    SharedDataStream(ds) << sampler.d;

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, Sampler &sampler)
{
    VersionID v = readHeader(ds, r_sampler);

    if (v == 1)
    {
        SharedDataStream(ds) >> sampler.d;
    }
    else
        throw version_error(v, "1", r_sampler, CODELOC);

    return ds;
}

static SharedPolyPointer<SamplerBase> shared_null( new UniformSampler() );

/** Null constructor */
Sampler::Sampler() : d(shared_null)
{}

/** Construct from the passed sampler */
Sampler::Sampler(const SamplerBase &sampler)
        : d(sampler)
{}

/** Copy constructor */
Sampler::Sampler(const Sampler &other)
        : d(other.d)
{}

/** Destructor */
Sampler::~Sampler()
{}

/** Copy assignment */
Sampler& Sampler::operator=(const Sampler &other)
{
    d = other.d;
    return *this;
}

/** Return a random molecule from the group, together with the probability
    that this molecule had of being chosen. */
tuple<Molecule,double> Sampler::randomMolecule(const MoleculeGroup &group)
{
    return d->randomMolecule(group);
}

/** Return a random residue from the group, together with the probability
    that this residue had of being chosen. */
tuple<Residue,double> Sampler::randomResidue(const MoleculeGroup &group)
{
    return d->randomResidue(group);
}

/** Return a random atom from the group, together with the probability
    that this atom had of being chosen. */
tuple<NewAtom,double> Sampler::randomAtom(const MoleculeGroup &group)
{
    return d->randomAtom(group);
}

/** Return the probability that the molecule 'molecule' has
    of being chosen in the System 'system' */
double Sampler::probability(const MoleculeGroup &group,
                            const Molecule &molecule)
{
    return d->probability(group, molecule);
}

/** Return the probability that the residue 'residue' has
    of being chosen in the System 'system' */
double Sampler::probability(const MoleculeGroup &group,
                            const Residue &residue)
{
    return d->probability(group, residue);
}

/** Return the probability that the atom 'atom' has
    of being chosen in the System 'system' */
double Sampler::probability(const MoleculeGroup &group,
                            const NewAtom &atom)
{
    return d->probability(group, atom);
}

/** Set the random number generator to be used by this sampler */
void Sampler::setGenerator(const RanGenerator &generator)
{
    d->setGenerator(generator);
}

/** Return the random number generator that is used by this sampler */
const RanGenerator& Sampler::generator() const
{
    return d->generator();
}
