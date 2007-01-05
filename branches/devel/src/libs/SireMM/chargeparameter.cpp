
#include "chargeparameter.h"

#include "SireMaths/maths.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMM;

/** Register this ChargeParameter */
static const RegisterMetaType<ChargeParameter> r_chargeparam;

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const ChargeParameter &charge)
{
    writeHeader(ds, r_chargeparam, 1) << charge.charge();

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, ChargeParameter &charge)
{
    VersionID v = readHeader(ds, r_chargeparam);

    if (v == 1)
    {
        double chg;
        ds >> chg;
        charge = chg;
    }
    else
        throw version_error( v, "1", r_chargeparam, CODELOC );

    return ds;
}

/** Construct a zero charge */
ChargeParameter::ChargeParameter() : chg(0)
{}

/** Construct a partial charge with value 'charge' */
ChargeParameter::ChargeParameter(double charge) : chg(charge)
{
    //check for zero parameters
    if ( SireMaths::isZero(chg) )
        chg = 0.0;
}

/** Copy constructor */
ChargeParameter::ChargeParameter(const ChargeParameter &other) : chg(other.chg)
{}

/** Destructor */
ChargeParameter::~ChargeParameter()
{}

/** Assignment operator */
ChargeParameter& ChargeParameter::operator=(const ChargeParameter &other)
{
    chg = other.chg;
    return *this;
}

/** Comparison operator */
bool ChargeParameter::operator==(const ChargeParameter &other) const
{
    return chg == other.chg;
}

/** Comparison operator */
bool ChargeParameter::operator!=(const ChargeParameter &other) const
{
    return chg != other.chg;
}

/** Return a string representation of the charge */
QString ChargeParameter::toString() const
{
    return QString("Charge( %1 |e| )").arg(chg);
}

/** Return a dummy charge parameter (zero charge) */
ChargeParameter ChargeParameter::dummy()
{
    return ChargeParameter();
}
