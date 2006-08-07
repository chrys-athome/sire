
#include "integrationconstant.h"
#include "registerexpression.h"

#include "SireCAS/errors.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireCAS;

static const RegisterMetaType<IntegrationConstant> r_intconst("SireCAS::IntegrationConstant");

/** Serialise to a binary datastream */
QDataStream SIRECAS_EXPORT &operator<<(QDataStream &ds, const IntegrationConstant &ic)
{
    writeHeader(ds, r_intconst, 1) << static_cast<const Symbol&>(ic);
    
    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIRECAS_EXPORT &operator>>(QDataStream &ds, IntegrationConstant &ic)
{
    VersionID v = readHeader(ds, r_intconst);
    
    if (v == 1)
    {
        ds >> static_cast<Symbol&>(ic);
    }
    else
        throw version_error(v, "1", r_intconst, CODELOC);
    
    return ds;
}

/** Register this expression */
static RegisterExpression<IntegrationConstant> RegisterIntegrationConstant;

/** Constructor */
IntegrationConstant::IntegrationConstant()
                    : Symbol("C")
{}

/** Copy constructor */
IntegrationConstant::IntegrationConstant( const IntegrationConstant &other )
                    : Symbol(other)
{}

/** Destructor */
IntegrationConstant::~IntegrationConstant()
{}

/** Comparison operator */
bool IntegrationConstant::operator==(const ExBase &other) const
{
    const IntegrationConstant *other_ic = dynamic_cast<const IntegrationConstant*>(&other);
    
    return other_ic != 0 and typeid(other).name() == typeid(*this).name();
}
  
/** Return a hash for this object */
uint IntegrationConstant::hash() const
{
    return ( r_intconst.magicID() << 16 ) | ( r_intconst.magicID() << 16 );
}

/** Cannot integrate an expression containing an integration constant. This 
    is to prevent integration constants from multiple integrations from
    appearing in the expression. */
Expression IntegrationConstant::integrate(const Symbol&) const
{
    throw SireCAS::unavailable_integral(QObject::tr(
        "Cannot integrate an IntegrationConstant. You must remove all "
        "integration constants from an expression before you can integrate "
        "it again (e.g. via ex.substitute(IntegrationConstant() == 0))."), CODELOC);

    return Expression();
}
