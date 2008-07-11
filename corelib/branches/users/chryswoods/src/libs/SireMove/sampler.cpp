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

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMove;
using namespace SireMol;
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
    sds << samplerbase.molgroup << samplerbase.rangen
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
        sds >> samplerbase.molgroup >> samplerbase.rangen
            >> static_cast<PropertyBase&>(samplerbase);
    }
    else
        throw version_error(v, "1", r_samplerbase, CODELOC);

    return ds;
}

/** Empty constructor */
SamplerBase::SamplerBase() : PropertyBase()
{}

/** Construct a sampler that picks molecules at random from the 
    molecule group 'molgroup' */
SamplerBase::SamplerBase(const MolGroup &moleculegroup)
            : PropertyBase(), molgroup(moleculegroup)
{}

/** Copy constructor */
SamplerBase::SamplerBase(const SamplerBase &other) 
            : PropertyBase(other), molgroup(other.molgroup), rangen(other.rangen)
{}

/** Destructor */
SamplerBase::~SamplerBase()
{}

/** Copy assignment */
SamplerBase& SamplerBase::operator=(const SamplerBase &other)
{
    PropertyBase::operator=(other);

    molgroup = other.molgroup;
    rangen = other.rangen;

    return *this;
}

/** Comparison operator */
bool SamplerBase::operator==(const SamplerBase &other) const
{
    return molgroup == other.molgroup;
}

/** Comparison operator */
bool SamplerBase::operator!=(const SamplerBase &other) const
{
    return molgroup != other.molgroup;
}

/** Set the random number generator used by this sampler */
void SamplerBase::setGenerator(const RanGenerator &generator)
{
    rangen = generator;
}

/** Set the molecule group from which random molecules will be sampled */    
void SamplerBase::setGroup(const MolGroup &moleculegroup)
{
    molgroup = moleculegroup;
}

//////////
////////// Implementation of Sampler
//////////

static const RegisterMetaType<Sampler> r_sampler;

/** Serialise a Sampler to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, const Sampler &sampler)
{
    writeHeader(ds, r_sampler, 1);
    
    SharedDataStream sds(ds);
    
    sds << static_cast<const Property&>(sampler);
    
    return ds;
}

/** Deserialise a Sampler from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, Sampler &sampler)
{
    VersionID v = readHeader(ds, r_sampler);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> static_cast<Property&>(sampler);
    }
    else
        throw version_error(v, "1", r_sampler, CODELOC);
        
    return ds;
}

static Sampler *_pvt_shared_null = 0;

const Sampler& Sampler::shared_null()
{
    if (_pvt_shared_null == 0)
        _pvt_shared_null = new Sampler( UniformSampler() );
        
    return *_pvt_shared_null;
}

/** Null constructor - constructs a simple, empty SamplerBase */
Sampler::Sampler() : Property(Sampler::shared_null())
{}

/** Construct from a passed property

    \throw SireError::invalid_cast
*/
Sampler::Sampler(const PropertyBase &property) : Property(property.asA<SamplerBase>())
{}

/** Construct from passed SamplerBase */
Sampler::Sampler(const SamplerBase &sampler) : Property(sampler)
{}

/** Copy constructor */
Sampler::Sampler(const Sampler &other) : Property(other)
{}

/** Destructor */
Sampler::~Sampler()
{}

/** Copy assignment operator from a Property object

    \throw SireError::invalid_cast
*/
Sampler& Sampler::operator=(const PropertyBase &property)
{
    Property::operator=(property.asA<SamplerBase>());
    return *this;
}

/** Copy assignment operator from another SamplerBase */
Sampler& Sampler::operator=(const SamplerBase &other)
{
    Property::operator=(other);
    return *this;
}

/** Dereference this pointer */
const SamplerBase* Sampler::operator->() const
{
    return static_cast<const SamplerBase*>(&(d()));
}

/** Dereference this pointer */
const SamplerBase& Sampler::operator*() const
{
    return static_cast<const SamplerBase&>(d());
}

/** Return a read-only reference to the contained object */
const SamplerBase& Sampler::read() const
{
    return static_cast<const SamplerBase&>(d());
}

/** Return a modifiable reference to the contained object.
    This will trigger a copy of the object if more than
    one pointer is pointing to it. */
SamplerBase& Sampler::edit()
{
    return static_cast<SamplerBase&>(d());
}
    
/** Return a raw pointer to the SamplerBase object */
const SamplerBase* Sampler::data() const
{
    return static_cast<const SamplerBase*>(&(d()));
}

/** Return a raw pointer to the SamplerBase object */
const SamplerBase* Sampler::constData() const
{
    return static_cast<const SamplerBase*>(&(d()));
}
    
/** Return a raw pointer to the SamplerBase object */
SamplerBase* Sampler::data()
{
    return static_cast<SamplerBase*>(&(d()));
}

/** Automatic casting operator */
Sampler::operator const SamplerBase&() const
{
    return static_cast<const SamplerBase&>(d());
}
