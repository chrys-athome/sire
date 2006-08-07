
#include "cljparameter.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMM;

static const RegisterMetaType<CLJParameter> r_cljparam("SireMM::CLJParameter");

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const CLJParameter &cljparam)
{
    writeHeader(ds, r_cljparam, 1) 
          << static_cast<const ChargeParameter&>(cljparam)
          << static_cast<const LJParameter&>(cljparam);
          
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, CLJParameter &cljparam)
{
    VersionID v = readHeader(ds, r_cljparam);
    
    if (v == 1)
    {
        ds >> static_cast<ChargeParameter&>(cljparam)
           >> static_cast<LJParameter&>(cljparam);
    }
    else
        throw version_error(v, "1", r_cljparam, CODELOC);
    
    return ds;
}

/** Create a a dummy parameter. */
CLJParameter::CLJParameter() : ChargeParameter(), LJParameter()
{}

/** Copy constructor */
CLJParameter::CLJParameter(const CLJParameter &other)
                             : ChargeParameter(other), LJParameter(other)
{}

/** Construct a CLJParameter with the supplied values of the charge,
    sigma and epsilon */
CLJParameter::CLJParameter(const ChargeParameter &charge, double sigma, double epsilon)
                             : ChargeParameter(charge), LJParameter(sigma, epsilon)
{}

/** Construct a CLJParameter from the supplied charge and LJ parameter */
CLJParameter::CLJParameter(const ChargeParameter &charge, const LJParameter &lj)
                             : ChargeParameter(charge), LJParameter(lj)
{}

/** Destructor */
CLJParameter::~CLJParameter()
{}

/** Comparison operator */
bool CLJParameter::operator==(const CLJParameter &other) const
{
    return ChargeParameter::operator==(other) and LJParameter::operator==(other);
}

/** Comparison operator */
bool CLJParameter::operator!=(const CLJParameter &other) const
{
    return ChargeParameter::operator!=(other) or LJParameter::operator!=(other);
}

/** Return a dummy CLJParameter */
CLJParameter CLJParameter::dummy()
{
    return CLJParameter(0.0,0.0,0.0);
}

/** Return a string representation of the CLJ parameter */
QString CLJParameter::toString() const
{
    return QString("CLJ[ %1 %2 ]")
            .arg(ChargeParameter::toString(), LJParameter::toString());
}
