
#include "exbase.h"
#include "values.h"
#include "expressionbase.h"
#include "expression.h"

#include "SireCAS/errors.h"

using namespace SireCAS;

/** Constructor */
ExBase::ExBase()
{}

/** Destructor */
ExBase::~ExBase()
{}

/** Comparison operator */
bool ExBase::operator!=(const ExBase &other) const
{
    return not operator==(other);
}

/** Return an ExpressionBase for this ExBase */
ExpressionBase ExBase::toExpressionBase() const
{
    boost::shared_ptr<const ExBase> ptr = selfptr.lock();
    
    if (not ptr) //we don't have a self pointer!
    {
        //create a clone, and give the clone a valid selfptr
        ptr.reset( this->clone() );
        
        const_cast<ExBase*>(ptr.get())->selfptr = ptr;
    }
    
    return ExpressionBase(ptr);
}
    
/** Return an Expression from this ExBase */
Expression ExBase::toExpression() const
{
    return Expression( this->toExpressionBase() );
}

/** Return the negative of this ExBase */
Expression ExBase::operator-() const
{
    return -(toExpression());
}

/** Return an expression that the differential of this ExBase
    with respect to 'symbol'. Note an exception may
    be thrown if this ExBase cannot be differentiated. 
        
    \throw SireCAS::unavailable_differential
*/
Expression ExBase::differentiate(const Symbol &symbol) const
{
    throw SireCAS::unavailable_differential(QObject::tr(
        "The differential of \"%1\" with respect to \"%2\" is not available.")
            .arg(toString(), symbol.toString()), CODELOC);
}
    
/** Return the indefinite integral of this 'ExBase' with respect to 
    symbol. This is not guaranteed to work(!) and will return an 
    expression of the form Sum( integral(exbase) + integral_constant ).
    If it doesn't work then an exception will be throw.
        
    \throw SireCAS::unavailable_integral
*/
Expression ExBase::integrate(const Symbol &symbol) const
{
    throw SireCAS::unavailable_integral(QObject::tr(
        "The integral of \"%1\" with respect to \"%2\" is not available.")
            .arg(toString(), symbol.toString()), CODELOC);
}
    
/** Return a series expansion of this expression with respect to 
    'symbol', if possible, to order
    'n'. This is not guaranteed to work, and will return this expression
    unchanged if it doesn't work. If it is expanded, then a series
    will be returned, together with an estimate of the error (e.g. O(x^2)) */
Expression ExBase::series(const Symbol&, int) const
{
    return this->toExpression();
}
    
/** Try to simplify this expression. This will try to use known mathematical
    identities to convert complex expressions down to more simple ones. 
    If SireCAS::UNSAFE_COMPLEX_SIMPLIFICATIONS is true, then identities
    that are not safe for complex math are used, e.g. z = sin(arcsin(z)). */
Expression ExBase::simplify(int) const
{
    return this->toExpression();
}
    
/** Expand this expression (if possible) */
Expression ExBase::expand() const
{
    return this->toExpression();
}

/** Collapse this expression (if possible) */
Expression ExBase::collapse() const
{
    return this->toExpression();
}

/** Return the complex conjugate of this expression */
Expression ExBase::conjugate() const
{
    return this->toExpression();
}

/** Return whether or not this is a function of the passed Symbol */
bool ExBase::isFunction(const Symbol&) const
{
    return false;
}

/** Return whether or not this is a constant expression (does not 
    depend on any symbols) */
bool ExBase::isConstant() const
{
    return true;
}

/** Return whether or not this expression contains any complex (imaginary)
    parts */
bool ExBase::isComplex() const
{
    return false;
}

/** Return whether or not this is a compound expression, and thus as such 
    requires brackets placed around it when it is printed. Examples include
    Sum, Product and Power. For most other functions it is safe to leave
    this as false. */
bool ExBase::isCompound() const
{
    return false;
}

namespace SireCAS
{

Expression SIRECAS_EXPORT operator+(const ExBase &base0, const ExBase &base1)
{
    return base0.toExpression() + base1.toExpression();
}

Expression SIRECAS_EXPORT operator-(const ExBase &base0, const ExBase &base1)
{
    return base0.toExpression() - base1.toExpression();
}

Expression SIRECAS_EXPORT operator*(const ExBase &base0, const ExBase &base1)
{
    return base0.toExpression() * base1.toExpression();
}

Expression SIRECAS_EXPORT operator/(const ExBase &base0, const ExBase &base1)
{
    return base0.toExpression() / base1.toExpression();
}

Expression SIRECAS_EXPORT operator*(const ExBase &base, double val)
{
    return base.toExpression().multiply(val);
}

Expression SIRECAS_EXPORT operator*(double val, const ExBase &base)
{
    return base.toExpression().multiply(val);
}

Expression SIRECAS_EXPORT operator*(const ExBase &base, const Complex &val)
{
    return base.toExpression().multiply(val);
}

Expression SIRECAS_EXPORT operator*(const Complex &val, const ExBase &base)
{
    return base.toExpression().multiply(val);
}

Expression SIRECAS_EXPORT operator+(const ExBase &base, const Expression &ex)
{
    return base.toExpression() + ex;
}

Expression SIRECAS_EXPORT operator-(const ExBase &base, const Expression &ex)
{
    return base.toExpression() - ex;
}

Expression SIRECAS_EXPORT operator*(const ExBase &base, const Expression &ex)
{
    return base.toExpression() * ex;
}

Expression SIRECAS_EXPORT operator/(const ExBase &base, const Expression &ex)
{
    return base.toExpression() / ex;
}

Expression SIRECAS_EXPORT operator+(const Expression &ex, const ExBase &base)
{
    return ex + base.toExpression();
}

Expression SIRECAS_EXPORT operator-(const Expression &ex, const ExBase &base)
{
    return ex - base.toExpression();
}

Expression SIRECAS_EXPORT operator*(const Expression &ex, const ExBase &base)
{
    return ex * base.toExpression();
}

Expression SIRECAS_EXPORT operator/(const Expression &ex, const ExBase &base)
{
    return ex / base.toExpression();
}

Expression SIRECAS_EXPORT pow(const ExBase &base, int n)
{
    return base.toExpression().pow(n);
}

Expression SIRECAS_EXPORT pow(const ExBase &base, const Rational &n)
{
    return base.toExpression().pow(n);
}

Expression SIRECAS_EXPORT pow(const ExBase &base, double n)
{
    return base.toExpression().pow(n);
}

Expression SIRECAS_EXPORT pow(const ExBase &base, const Complex &n)
{
    return base.toExpression().pow(n);
}

Expression SIRECAS_EXPORT pow(const ExBase &base, const Expression &n)
{
    return base.toExpression().pow(n);
}

Expression SIRECAS_EXPORT pow(const ExBase &base, const ExBase &n)
{
    return base.toExpression().pow(n.toExpression());
}

}
