
#include "constant.h"
#include "expression.h"
#include "expressions.h"
#include "symbol.h"
#include "symbols.h"
#include "functions.h"
#include "registerexpression.h"
#include "integrationconstant.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireCAS;

static const RegisterMetaType<Constant> r_constant("SireCAS::Constant");

/** Serialise a constant to a binary datastream */
QDataStream SIRECAS_EXPORT &operator<<(QDataStream &ds, const Constant&)
{
    writeHeader(ds, r_constant, 0);
    return ds;
}

/** Deserialise a constant from a binary datastream */
QDataStream SIRECAS_EXPORT &operator>>(QDataStream &ds, Constant&)
{
    VersionID v = readHeader(ds, r_constant);
    
    if (v != 0)
        throw version_error(v, "0", r_constant, CODELOC);
    
    return ds;
}

/** Register the 'Constant' class */
static RegisterExpression<Constant> RegisterConstant;

/** Construct a constant */
Constant::Constant() : ExBase()
{}

/** Copy constructor */
Constant::Constant(const Constant&) : ExBase()
{}

/** Destructor */
Constant::~Constant()
{}

/** Differential of a constant is zero */
Expression Constant::differentiate(const Symbol&) const
{
    return 0;
}

/** Integral of a constant is = constant*symbol + C */
Expression Constant::integrate(const Symbol &symbol) const
{
    return *this * symbol;
}
        
/** Comparison operator */
bool Constant::operator==(const ExBase &other) const
{
    return typeid(other).name() == typeid(*this).name();
}

/** Hash a constant */
uint Constant::hash() const
{
    return (r_constant.magicID() << 16) | (r_constant.magicID() & 0x0000FFFF);
}

/** Return a string representation of this constant (actually an empty string!) */
QString Constant::toString() const
{
    return QString("1.0");
}

/** Evaluation of a constant is 1 */
double Constant::evaluate(const Values&) const
{
    return 1;
}

/** Evaluation of a constant is 1 */
Complex Constant::evaluate(const ComplexValues&) const
{
    return Complex(1,0);
}
    
/** Can't substitute into a constant */
Expression Constant::substitute(const Identities&) const
{
    return this->toExpression();
}
    
/** No symbols in a constant */
Symbols Constant::symbols() const
{
    return Symbols();
}

/** No functions in a constant */
Functions Constant::functions() const
{
    return Functions();
}

/** No children in a constant */
Expressions Constant::children() const
{
    return Expressions();
}
