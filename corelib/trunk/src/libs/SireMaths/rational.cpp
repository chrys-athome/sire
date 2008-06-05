
#include "rational.h"
#include "maths.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMaths;

static const RegisterMetaType<Rational> r_rational("SireMaths::Rational");

/** Serialise a rational number to a binary datastream */
QDataStream SIREMATHS_EXPORT &operator<<(QDataStream &ds, const Rational &val)
{
    writeHeader(ds, r_rational, 1) << val.numerator() << val.denominator();
    return ds;
}

/** Deserialise a rational number from a binary datastream */
QDataStream SIREMATHS_EXPORT &operator>>(QDataStream &ds, Rational &val)
{
    VersionID v = readHeader(ds, r_rational);
    
    if (v == 1)
    {
        qint32 num,denom;
        ds >> num >> denom;

        val = Rational(num,denom);
    }
    else
        throw version_error(v, "1", r_rational, CODELOC);

    return ds;
}
