/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007  Christopher Woods
  *
  *  This program is free software; you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation; either version 2 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program; if not, write to the Free Software
  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  *  For full details of the license please see the COPYING file
  *  that should have come with this distribution.
  *
  *  You can contact the authors via the developer's mailing list
  *  at http://siremol.org
  *
\*********************************************/

#include "expression.h"
#include "sum.h"
#include "product.h"
#include "symbol.h"
#include "power.h"
#include "powerconstant.h"
#include "identities.h"
#include "values.h"
#include "complexvalues.h"
#include "i.h"
#include "factor.h"
#include "integrationconstant.h"

#include "SireMaths/maths.h"
#include "SireMaths/complex.h"

#include "Siren/stream.h"

using namespace Siren;
using namespace SireCAS;
using namespace SireMaths;

static const RegisterObject<Expression> r_expression;

static CASNodePtr constant_ptr;

static const CASNode& getConstant()
{
    if (constant_ptr.constData() == 0)
    {
        QMutexLocker lkr( &Siren::globalRegistrationLock() );
        
        if (constant_ptr.constData() == 0)
            constant_ptr = new Constant();
    }
    
    return *(constant_ptr.constData());
}

/** Construct '0' */
Expression::Expression() : Implements<Expression,CASNode>(),
                           casnode( getConstant() ), fac(0)
{}

/** Construct 'constant' */
Expression::Expression(int constant) : Implements<Expression,CASNode>(),
                                       casnode( getConstant() ), fac(constant)
{}

/** Construct 'constant' */
Expression::Expression(double constant) : Implements<Expression,CASNode>(),
                                          casnode( getConstant() ), fac(constant)
{}

/** Construct 'constant' */
Expression::Expression(const SireMaths::Rational &constant)
           : Implements<Expression,CASNode>(),
             casnode( getConstant() ), fac(constant)
{}

/** Construct 'constant' */
Expression::Expression(const Complex &constant) : Implements<Expression,CASNode>(),
                                                  casnode( getConstant() )
{
    if (constant.isZero())
        fac = 0.0;
    else if (constant.isReal())
        fac = constant.real();
    else if (SireMaths::isZero(constant.real()))
    {
        fac = constant.imag();
        casnode = I();
    }
    else
    {
        fac = constant.imag();
        casnode = Sum((constant.real() / fac), I());
    }
}

/** Construct from the passed node */
Expression::Expression(const CASNode &node) 
           : Implements<Expression,CASNode>(), casnode(node), fac(1)
{
    if (node.isA<Expression>())
    {
        casnode = node.asA<Expression>().casnode;
        fac = node.asA<Expression>().fac;
    }
    else if (node.isConstant())
    {
        if (node.isComplex())
        {
            Complex val = node.evaluate( ComplexValues() );
            
            this->operator=( Expression(val) );
        }
        else
        {
            double val = node.evaluate(Values());
            
            this->operator=( Expression(val) );
        }
    }
}

/** Copy constructor */
Expression::Expression(const Expression &other)
           : Implements<Expression,CASNode>(other),
             casnode(other.casnode), fac(other.fac)
{}

/** Destructor */
Expression::~Expression()
{}

/** Copy assignment operator */
Expression& Expression::operator=(const Expression &other)
{
    if (this != &other)
    {
        fac = other.fac;
        casnode = other.casnode;
        CASNode::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool Expression::operator==(const Expression &other) const
{
    return fac == other.fac and casnode.equals(other.casnode);
}

bool Expression::operator!=(const Expression &other) const
{
    return not Expression::operator==(other);
}

/** Return the factor by which the base node of this expression is multiplied */
double Expression::factor() const
{
    return fac;
}

/** Return the base node of this expression */
const CASNode& Expression::node() const
{
    return *casnode;
}

QString Expression::toString() const
{
    if (isConstant())
        return evaluate(ComplexValues()).toString();
    else
    {
        QString basestr = casnode->toString();

        if ( SireMaths::areEqual(fac,1.0) )
            return basestr;
        else if ( SireMaths::areEqual(fac,-1.0) )
        {
            if (this->isCompound())
                return QString("-[%1]").arg(basestr);
            else
                return QString("-%1").arg(basestr);
        }
        else if (this->isCompound())
            return QString("%1 [%2]").arg(fac).arg(basestr);
        else
            return QString("%1 %2").arg(fac).arg(basestr);
    }
}

uint Expression::hashCode() const
{
    return qHash(Expression::typeName()) + qHash(fac) + qHash(node());
}

void Expression::stream(Siren::Stream &s)
{
    s.assertVersion<Expression>(1);
    
    Schema schema = s.item<Expression>();
    
    schema.data("factor") & fac;
    schema.data("node") & casnode;
    
    CASNode::stream( schema.base() );
}

Expression Expression::operator-() const
{
    Expression ret( *this );
    ret.fac *= -1;
    
    return ret;
}

/** Differentiate this expression with respect to 'symbol' and return
    the resulting expression.

    \throw SireCAS::unavailable_differential
*/
Expression Expression::differentiate(const Symbol &symbol, int level) const
{
    if (level <= 0 or this->isZero())
        return *this;
    else if (level > 0 and this->isConstant())
        return Expression(0);
    else if (not casnode->isFunction(symbol))
        // d f(y) / dx = 0
        return Expression(0);
    else
    {
        //calculate the differential of the base expression with respect
        //to symbol
        Expression diff = fac * casnode->differentiate(symbol);

        if (not diff.isZero())
        {
            if (level == 1)
                return diff;
            else
                return diff.differentiate(symbol, level-1);
        }
        else
            return diff;
    }
}

/** Differentiate this expression with respect to 'symbol' and return
    the resulting expression.

    \throw SireCAS::unavailable_differential
*/
Expression Expression::differentiate(const Symbol &symbol) const
{
    return this->differentiate(symbol, 1);
}

/** Integrate this expression with respect to 'symbol' and return the
    resulting expression.

    \throw SireCAS::unavailable_integral
*/
Expression Expression::integrate(const Symbol &symbol) const
{
    if (this->isZero())
        return Expression(0);
    else if (not casnode->isFunction(symbol))
        //exbase is constant with respect to 'symbol' - return symbol*exbase + C
        return (fac * *casnode * symbol) + IntegrationConstant();
    else
        //calculate the integral with respect to the symbol (add integration constant)
        return (fac * casnode->integrate(symbol)) + IntegrationConstant();
}

/** Return the complex conjugate of this expression */
Expression Expression::conjugate() const
{
    return fac * casnode->conjugate();
}

/** Return a series expansion of this function with respect to 'symbol'
    up to order 'n'. If an expansion is not possible, then this just
    returns this expression */
Expression Expression::series(const Symbol &symbol, int n) const
{
    return fac * casnode->series(symbol,n);
}

/** Try to simplify this expression by using built-in identities. If
    SireCAS::UNSAFE_COMPLEX_SIMPLIFICATIONS is passed, then allow the use
    of identities that are not necessarily true in the complex domain,
    e.g. z = sin(arcsin(z)) */
Expression Expression::simplify(int options) const
{ 
    return fac * casnode->simplify(options);
}

/** Return whether or not this expression is '0' for all values */
bool Expression::isZero() const
{
    return SireMaths::isZero(fac);
}

/** Return whether or not this expression is constant for all values */
bool Expression::isConstant() const
{
    return casnode->isConstant();
}

/** Return whether or not this is a function of 'symbol' */
bool Expression::isFunction(const Symbol &symbol) const
{
    return casnode->isFunction(symbol);
}

/** Return whether or not this is a compound expression (contains more
    than a single expression, e.g. Sum, Product or PowerFunction) */
bool Expression::isCompound() const
{
    return casnode->isCompound();
}

/** Return whether or not this expression has complex parts */
bool Expression::isComplex() const
{
    return casnode->isComplex();
}

/** Return this expression added to 'ex' */
Expression Expression::add(const CASNode &node) const
{
    return Sum(*this, Expression(node)).reduce();
}

/** Return this expression added to 'val' */
Expression Expression::add(double val) const
{
    return Sum(*this, Expression(val)).reduce();
}

/** Return this expression added to 'val' */
Expression Expression::add(const Complex &val) const
{
    return Sum(*this, Expression(val)).reduce();
}

/** Return an expression that is this - ex */
Expression Expression::subtract(const CASNode &node) const
{
    return Sum(*this, node.negate()).reduce();
}

/** Return an expression that is this - val */
Expression Expression::subtract(double val) const
{
    return Sum(*this, -val).reduce();
}

/** Return an expression that is this - val */
Expression Expression::subtract(const SireMaths::Complex &val) const
{
    return Sum(*this, -val).reduce();
}

/** Return an expression that is this multiplied by 'ex' */
Expression Expression::multiply(const CASNode &node) const
{
    if (this->isConstant())
        return node.multiply( this->evaluate(ComplexValues()) );
    else if (node.isConstant())
        return this->multiply( node.evaluate(ComplexValues()) );
    else
        return Product(*this, Expression(node)).reduce();
}

/** Return an expression that is this multipled by 'val' */
Expression Expression::multiply(double val) const
{
    if (casnode->isA<IntegrationConstant>())
    {
        return *this;
    }
    else
    {
        double newfactor = fac * val;

        if (SireMaths::isZero(newfactor))
            return Expression(0);
        else
        {
            Expression ret(*this);
            ret.fac = newfactor;

            return ret;
        }
    }
}

/** Return an expression that is this multiplied by the complex value z */
Expression Expression::multiply(const SireMaths::Complex &z) const
{
    if (z.isReal())
        return this->multiply(z.real());
    else if (SireMaths::isZero(z.real()))
        return Product( *this, z.imag() * I() ).reduce();
    else
        return Product( *this, z.real() + z.imag()*I() ).reduce();
}

/** Return an expression that is this / node */
Expression Expression::divide(const CASNode &node) const
{
    if (node.isConstant())
        return divide( node.evaluate(ComplexValues()) );
    else
        return multiply(node.invert());
}

/** Return an expression that is this divided by 'val' */
Expression Expression::divide(double val) const
{
    if ( SireMaths::areEqual(val,1.0) )
        return *this;
    else
        return multiply( double(1.0) / val );
}

/** Return an expression that is divided by the complex number z */
Expression Expression::divide(const SireMaths::Complex &val) const
{
    return multiply( val.inverse() );
}

/** Return the negative of this expression */
Expression Expression::negate() const
{
    Expression ret(*this);
    ret.fac *= -1;
    return ret;
}

/** Return 1 / expression */
Expression Expression::invert() const
{
    return this->pow(-1);
}

/** Return this expression raised to the power 'n' */
Expression Expression::pow(int n) const
{
    if (n == 1)
        return *this;
    else if (n == 0)
        return Expression(1);
    else if (this->isConstant())
        return SireMaths::pow(evaluate(ComplexValues()), n);
    else
        return IntegerPower( *this, n ).reduce();
}

/** Return the square of this expression */
Expression Expression::squared() const
{
    return this->pow(2);
}

/** Return the cube of this expression */
Expression Expression::cubed() const
{
    return this->pow(3);
}

/** Return the nth root of this expression */
Expression Expression::root(int n) const
{
    return this->pow( Rational(1,n) );
}

/** Return the square root of this expression */
Expression Expression::sqrt() const
{
    return this->root(2);
}

/** Return the cube root of this expression */
Expression Expression::cbrt() const
{
    return this->root(3);
}

/** Return this expression raised to the rational power 'n' */
Expression Expression::pow(const Rational &n) const
{
    if (n.denominator() == 1)
        return this->pow(n.numerator());
    else if (this->isConstant())
        return SireMaths::pow(evaluate(ComplexValues()), n);
    else
        return RationalPower( *this, n ).reduce();
}

/** Return this expression raised to a real number power */
Expression Expression::pow(double n) const
{
    if (SireMaths::areEqual(n,1.0))
        return *this;
    else if (this->isConstant())
        return SireMaths::pow(evaluate(ComplexValues()), n);
    else
        return RealPower( *this, n ).reduce();
}

/** Return this expresssion raised to a complex power */
Expression Expression::pow(const Complex &n) const
{
    if (n.isReal())
        return pow(n.real());
    else if (this->isConstant())
        return SireMaths::pow(evaluate(ComplexValues()), n);
    else
        return ComplexPower( *this, n ).reduce();
}

/** Return this expression raised to a function */
Expression Expression::pow(const CASNode &n) const
{
    if (n.isConstant())
        return pow(n.evaluate(ComplexValues()));
    else
        return Power( *this, n ).reduce();
}

/** Evaluate the numerical value of this expression, using the values
    supplied in 'values'. Any unidentified symbols or functions are
    assumed to be equal to zero. Note that this only performs real-arithmetic,
    so an exception will be thrown if any part of this expression generates
    a complex result.

    \throw SireMaths::domain_error
*/
double Expression::evaluate(const Values &values) const
{
    return fac * casnode->evaluate(values);
}

/** Evaluate the numerical value of this expression using complex
    arithmetic. Any unidentified symbols or functions are assumed
    to be equal to zero.
*/
Complex Expression::evaluate(const ComplexValues &values) const
{
    return fac * casnode->evaluate(values);
}

/** Return an expression whereby the identities in 'identities' have
    been substituted into this expression */
Expression Expression::substitute(const Identities &identities) const
{
    Expression ret = casnode->substitute(identities);
    ret.fac *= fac;
    return ret;
}

/** Return the child expressions that make up this expression */
QList<Expression> Expression::children() const
{
    return casnode->children();
}

/** Return all of the symbols used in this expression */
QSet<Symbol> Expression::symbols() const
{
    return casnode->symbols();
}

/** Return the factors and powers for the symbol 'symbol', given the values of the 
    other symbols in 'values'. This attempts to rearrange this equation
    so that it is of the form 'm * symbol^i + n * symbol^j ... + constant', 
    and it returns the values of 'm,i', 'n,j' etc.. 
    Note that this will fail if such a rearrangement is not possible
    
    \throw SireCAS::rearrangement_error
*/
QList<Factor> Expression::expand(const Symbol &symbol) const
{
    QList<Factor> factors = casnode->expand(symbol);
    
    if (fac != 1)
    {
        for (QList<Factor>::iterator it = factors.begin();
             it != factors.end();
             ++it)
        {
            *it = Factor( it->symbol(), fac * it->factor(), it->power() );
        }
    }
    
    return factors;
}
