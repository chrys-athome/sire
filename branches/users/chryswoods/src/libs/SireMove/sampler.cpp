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

#include "SireMol/partialmolecule.h"
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
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, 
                                        const SamplerBase &samplerbase)
{
    writeHeader(ds, r_samplerbase, 1);

    SharedDataStream sds(ds);
    sds << samplerbase.rangen
        << static_cast<const PropertyBase&>(samplerbase);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, 
                                        SamplerBase &samplerbase)
{
    VersionID v = readHeader(ds, r_samplerbase);

    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> samplerbase.rangen
            >> static_cast<PropertyBase&>(samplerbase);
    }
    else
        throw version_error(v, "1", r_samplerbase, CODELOC);

    return ds;
}

/** Construct with a specified random number generator */
SamplerBase::SamplerBase(const RanGenerator &generator)
            : PropertyBase(), rangen(generator)
{}

/** Copy constructor */
SamplerBase::SamplerBase(const SamplerBase &other) 
            : PropertyBase(other), rangen(other.rangen)
{}

/** Destructor */
SamplerBase::~SamplerBase()
{}

/** Copy assignment */
SamplerBase& SamplerBase::operator=(const SamplerBase &other)
{
    PropertyBase::operator=(other);

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

static SharedPolyPointer<SamplerBase> shared_null;

/** Return the global null sampler */
SharedPolyPointer<SamplerBase> SamplerBase::null_sampler()
{
    if (shared_null.constData() == 0)
        shared_null = new UniformSampler();
        
    return shared_null;
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

/** Null constructor */
Sampler::Sampler() : d(SamplerBase::null_sampler())
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

/** Return a random molecule from the system, together with the probability
    that this molecule had of being chosen. */
tuple<PartialMolecule,double> Sampler::sample(const QuerySystem &system)
{
    return d->sample(system);
}

/** Return the probability that the molecule 'molecule' has
    of being chosen in the System 'system' */
double Sampler::probabilityOf(const PartialMolecule &molecule,
                              const QuerySystem &system)
{
    return d->probabilityOf(molecule, system);
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
