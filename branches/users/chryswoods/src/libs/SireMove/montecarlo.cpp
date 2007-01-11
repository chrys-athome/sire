
#include "montecarlo.h"

#include "SireStream/datastream.h"

using namespace SireMove;
using namespace SireSystem;
using namespace SireStream;

static const RegisterMetaType<MonteCarlo> r_mc(MAGIC_ONLY, "SireMove::MonteCarlo");

/** Serialise to a binary data stream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, const MonteCarlo &mc)
{
    writeHeader(ds, r_mc, 1) 
          << mc.rangen << static_cast<const MoveBase&>(mc);
          
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, MonteCarlo &mc)
{
    VersionID v = readHeader(ds, r_mc);
    
    if (v == 1)
    {
        ds >> mc.rangen >> static_cast<MoveBase&>(mc);
    }
    else
        throw version_error(v, "1", r_mc, CODELOC);
    
    return ds;
}

/** Construct using the global random number generator */
MonteCarlo::MonteCarlo() : MoveBase()
{}

/** Construct using the supplied random number generator */
MonteCarlo::MonteCarlo(const RanGenerator &generator)
           : MoveBase(), rangen(generator)
{}

/** Copy constructor */
MonteCarlo::MonteCarlo(const MonteCarlo &other)
           : MoveBase(other), rangen(other.rangen)
{}

/** Destructor */
MonteCarlo::~MonteCarlo()
{}

/** Copy assignment */
MonteCarlo& MonteCarlo::operator=(const MonteCarlo &other)
{
    rangen = other.rangen;
    MoveBase::operator=(other);
    
    return *this;
}

/** Set the random number generator to use for these moves */
void MonteCarlo::setGenerator(const RanGenerator &generator)
{
    rangen = generator;
}

/** Return the random number generator used for these moves */
const RanGenerator& MonteCarlo::generator() const
{
    return rangen;
}
