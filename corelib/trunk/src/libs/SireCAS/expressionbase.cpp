
#include "expressionbase.h"
#include "exbase.h"
#include "constant.h"
#include "factory.h"
#include "identities.h"
#include "symbols.h"
#include "functions.h"
#include "expressions.h"
#include "complexvalues.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireCAS;

/** This is the null ExpressionBase - it equals '1.0' */
static const ExpressionBase one = Constant();

static const RegisterMetaType<ExpressionBase> r_exbase("SireCAS::ExpressionBase");

/** Serialise an ExpressionBase to a binary datastream */
QDataStream SIRECAS_EXPORT &operator<<(QDataStream &ds, const ExpressionBase &ex)
{
    writeHeader(ds, r_exbase, 1);

    return SireCAS::Factory::dump(ds,*(ex.d));
}

/** Deserialise an ExpressionBase from a binary datastream */
QDataStream SIRECAS_EXPORT &operator>>(QDataStream &ds, ExpressionBase &ex)
{
    VersionID v = readHeader(ds, r_exbase);
    
    if (v == 1)
    {
        ex = SireCAS::Factory::load(ds);
    }
    else
        throw version_error(v, "1", r_exbase, CODELOC);
    
    return ds;
}

/** Construct a default ExpressionBase (equals the number 1) */
ExpressionBase::ExpressionBase() : d( one.d )
{}

/** Construct an expression from an ExBase */
ExpressionBase::ExpressionBase(const ExBase &ex) : d( ex.toExpressionBase().d )
{
    BOOST_ASSERT( d.get() != 0 );
}

/** Construct from a shared_ptr */
ExpressionBase::ExpressionBase(const boost::shared_ptr<const ExBase> &ptr) : d(ptr)
{
    BOOST_ASSERT( d.get() != 0 );
}

/** Destructor */
ExpressionBase::~ExpressionBase()
{}

/** Comparison operator */
bool ExpressionBase::operator==(const ExpressionBase &other) const
{
    return d == other.d or *d == *(other.d);
}
    
/** Comparison operator */
bool ExpressionBase::operator!=(const ExpressionBase &other) const
{
    return d != other.d and *d != *(other.d);
}
    
/** Assignment operator */
ExpressionBase& ExpressionBase::operator=(const ExpressionBase &other)
{
    d = other.d;
    return *this;
}

/** Return an expression that the differential of this expression
    with respect to 'symbol'. Note an exception may
    be thrown if this cannot be differentiated. 
        
    \throw SireCAS::unavailable_differential
*/
Expression ExpressionBase::differentiate(const Symbol &symbol) const
{
    return d->differentiate(symbol);
}

/** Return the indefinite integral of this expression with respect to 
    symbol. This is not guaranteed to work(!) and will return an 
    expression of the form Sum( integral(exbase) + integral_constant ).
    If it doesn't work then an exception will be throw.
        
    \throw SireCAS::unavailable_integral
*/
Expression ExpressionBase::integrate(const Symbol &symbol) const
{
    return d->integrate(symbol);
}
    
/** Return a series expansion of this expression with respect to 
    'symbol', if possible, to order
    'n'. This is not guaranteed to work, and will return this expression
    unchanged if it doesn't work. If it is expanded, then a series
    will be returned, together with an estimate of the error (e.g. O(x^2)) */
Expression ExpressionBase::series(const Symbol &symbol, int n) const
{
    return d->series(symbol,n);
}
    
/** Try to simplify this expression. This will try to use known mathematical
    identities to convert complex expressions down to more simple ones. 
    If SireCAS::UNSAFE_COMPLEX_SIMPLIFICATIONS is true, then identities
    that are not safe for complex math are used, e.g. z = sin(arcsin(z)). */
Expression ExpressionBase::simplify(int options) const
{
    return d->simplify(options);
}
    
/** Expand this expression (if possible) */
Expression ExpressionBase::expand() const
{
    return d->expand();
}

/** Collapse this expression (if possible) */
Expression ExpressionBase::collapse() const
{
    return d->collapse();
}

/** Return the complex conjugate of this expression */
Expression ExpressionBase::conjugate() const
{
    return d->conjugate();
}
    
/** Return whether or not this is a function of the passed Symbol */
bool ExpressionBase::isFunction(const Symbol &symbol) const
{
    return d->isFunction(symbol);
}

/** Return whether or not this is a constant expression (does not 
    depend on any symbols) */
bool ExpressionBase::isConstant() const
{
    return d->isConstant();
}

/** Return whether or not this expression contains any complex (imaginary)
    parts */
bool ExpressionBase::isComplex() const
{
    return d->isComplex();
}

/** Return whether or not this is a compound expression, and thus as such 
    requires brackets placed around it when it is printed. Examples include
    Sum, Product and Power. For most other functions it is safe to leave
    this as false. */
bool ExpressionBase::isCompound() const
{
    return d->isCompound();
}

/** Return a hash of this object - return a combination of the 
    identifying magic for the class and a hash for its contents. */
uint ExpressionBase::hash() const
{
    return d->hash();
}

/** Return the name of the type of this ExBase object */
QString ExpressionBase::what() const
{
    return d->what();
}

/** Return a string representation of this object */
QString ExpressionBase::toString() const
{
    if (d.get() != 0)
        return d->toString();
    else
        return "WARNING: NULL EXPRESSIONBASE";
}

/** Evaluate this ExBase using values 'values'. Any 
    missing symbols are assumed to equal zero. 
        
    Note that an exception will be thrown if the result of the
    evaluation of this, or one of its children, is complex.
        
    \throw SireMaths::math_error
    \throw SireMaths::domain_error
*/
double ExpressionBase::evaluate(const Values &values) const
{
    return d->evaluate(values);
}

/** Evaluate this ExBase using the complex values 'values'. 
    Any missing symbols are assumed to equal zero. 
    
    \throw SireMaths::math_error
*/
Complex ExpressionBase::evaluate(const ComplexValues &values) const
{
    return d->evaluate(values);
}
    
/** Return an expression that has the identities in 'identities'
    substituted into this expression */
Expression ExpressionBase::substitute(const Identities &identities) const
{
    return d->substitute(identities);
}
    
/** Return the set of Symbols that appear in this ExBase */
Symbols ExpressionBase::symbols() const
{
    return d->symbols();
}

/** Return the set of Functions that appear in this ExBase */
Functions ExpressionBase::functions() const
{
    return d->functions();
}

/** Return the child expressions of this Expression */
Expressions ExpressionBase::children() const
{
    return d->children();
}
