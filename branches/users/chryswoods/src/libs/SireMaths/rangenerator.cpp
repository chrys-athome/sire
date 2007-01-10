
#include "rangenerator.h"

#include <boost/random.hpp>

using namespace SireMaths;
using namespace SireMaths::detail;

using namespace boost;

namespace SireMaths
{

namespace detail
{

class RanGeneratorPvt
{

friend QDataStream& ::operator<<(QDataStream&, const RanGeneratorPvt&);
friend QDataStream& ::operator>>(QDataStream&, RanGeneratorPvt&);

public:


private:
    /** Mutex to serialise access to the generator */
    QMutex mutex;

    /** The actual generator */
    lagged_fibonacci607 rand_generator;
};

}

}

/** Serialise to a binary datastream */
QDataStream SIREMATHS_EXPORT &operator<<(QDataStream &ds, const )
{
    writeHeader(ds, , 1)

    //serialise to ostream, compress resulting bytearray,
    //serialise bytearray to datastream!

    //do this as large datastream!

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMATHS_EXPORT &operator>>(QDataStream &ds, )
{
    VersionID v = readHeader(ds, );

    if (v == 1)
    {
        ds >>
    }
    else
        throw version_error(v, "1", , CODELOC);

    return ds;
}
