
#include "complex.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMaths;

static const RegisterMetaType<Complex> r_complex("SireMaths::Complex");

/** Serialise a Complex to a binary datastream */
QDataStream SIREMATHS_EXPORT &operator<<(QDataStream &ds, const SireMaths::Complex &z)
{
    writeHeader(ds, r_complex, 1) << z.real() << z.imag();
    
    return ds;
}

/** Deserialise a Complex from a binary datastream */
QDataStream SIREMATHS_EXPORT &operator>>(QDataStream &ds, SireMaths::Complex &z)
{
    VersionID v = readHeader(ds, r_complex);

    if (v == 1)
    {
        double r,i;
        ds >> r >> i;
        z.setRectangular(r,i);
    }
    else
        throw version_error(v, "1", r_complex, CODELOC);
    
    return ds;
}
