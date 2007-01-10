
#include "sampler.h"
#include "uniformsampler.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMove;
using namespace SireSystem;
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

/** Copy constructor */
SamplerBase::SamplerBase(const SamplerBase&) : QSharedData()
{}

/** Destructor */
SamplerBase::~SamplerBase()
{}

/** Copy assignment */
SamplerBase& SamplerBase::operator=(const SamplerBase&)
{
    return *this;
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
