/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2009  Christopher Woods
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

#include "casnode.h"
#include "symbol.h"
#include "expression.h"
#include "identities.h"
#include "values.h"
#include "complexvalues.h"
#include "factor.h"

#include "SireCAS/errors.h"

#include "SireMaths/complex.h"
#include "SireMaths/rational.h"

#include "Siren/stream.h"
#include "Siren/errors.h"

using namespace SireCAS;
using namespace SireMaths;
using namespace Siren;

static const RegisterObject<CASNode> r_casnode( VIRTUAL_CLASS );

/** Constructor */
CASNode::CASNode() : Extends<CASNode,Object>()
{}

/** Copy constructor */
CASNode::CASNode(const CASNode &other) : Extends<CASNode,Object>(other)
{}

/** Destructor */
CASNode::~CASNode()
{}

CASNode& CASNode::operator=(const CASNode &other)
{
    Object::operator=(other);
    return *this;
}

bool CASNode::operator==(const CASNode&) const
{
    return true;
}

bool CASNode::operator!=(const CASNode&) const
{
    return false;
}

QString CASNode::typeName()
{
    return "SireCAS::CASNode";
}

double CASNode::operator()(const Values &values) const
{
    return this->evaluate(values);
}

SireMaths::Complex CASNode::operator()(const ComplexValues &values) const
{
    return this->evaluate(values);
}

void CASNode::stream(Stream &s)
{
    s.assertVersion<CASNode>(1);
    
    Schema schema = s.item<CASNode>();
    
    Object::stream( schema.base() );
}

/** Return this expression raised to the power 'n' */
Expression CASNode::pow(int n) const
{
    return Expression(*this).pow(n);
}

/** Return this expression raised to the rational power 'n' */
Expression CASNode::pow(const Rational &n) const
{
    return Expression(*this).pow(n);
}

/** Return this expression raised to a real number power */
Expression CASNode::pow(double n) const
{
    return Expression(*this).pow(n);
}

/** Return this expresssion raised to a complex power */
Expression CASNode::pow(const Complex &n) const
{
    return Expression(*this).pow(n);
}

/** Return this expression raised to a function */
Expression CASNode::pow(const CASNode &node) const
{
    return Expression(*this).pow(node);
}

/** Return the square root of this expression */
Expression CASNode::sqrt() const
{
    return Expression(*this).sqrt();
}

/** Return the cube root of this expression */
Expression CASNode::cbrt() const
{
    return Expression(*this).cbrt();
}

/** Return the nth root of this expression */
Expression CASNode::root(int n) const
{
    return Expression(*this).root(n);
}

/** Return 1 / expression */
Expression CASNode::invert() const
{
    return Expression(*this).invert();
}

/** Return the square of this expression */
Expression CASNode::squared() const
{
    return Expression(*this).squared();
}

/** Return the cube of this expression */
Expression CASNode::cubed() const
{
    return Expression(*this).cubed();
}

/** Return the negative of this expression */
Expression CASNode::negate() const
{
    return Expression(*this).negate();
}

/** Negation operator */
Expression CASNode::operator-() const
{
    return this->negate();
}

/** Return this expression added to 'ex' */
Expression CASNode::add(const CASNode &node) const
{
    return Expression(*this).add(node);
}

/** Return this expression added to 'val' */
Expression CASNode::add(double val) const
{
    return Expression(*this).add(val);
}

/** Return this expression added to 'val' */
Expression CASNode::add(const Complex &val) const
{
    return Expression(*this).add(val);
}

/** Return an expression that is this - ex */
Expression CASNode::subtract(const CASNode &node) const
{
    return Expression(*this).subtract(node);
}

/** Return an expression that is this - val */
Expression CASNode::subtract(double val) const
{
    return Expression(*this).subtract(val);
}

/** Return an expression that is this - val */
Expression CASNode::subtract(const Complex &val) const
{
    return Expression(*this).subtract(val);
}

/** Return an expression that is this multipled by 'val' */
Expression CASNode::multiply(double val) const
{
    return Expression(*this).multiply(val);
}

/** Return an expression that is this multiplied by the complex value z */
Expression CASNode::multiply(const Complex &z) const
{
    return Expression(*this).multiply(z);
}

/** Return an expression that is this multiplied by 'ex' */
Expression CASNode::multiply(const CASNode &node) const
{
    return Expression(*this).multiply(node);
}

/** Return an expression that is this divided by 'val' */
Expression CASNode::divide(double val) const
{
    return Expression(*this).divide(val);
}

/** Return an expression that is divided by the complex number z */
Expression CASNode::divide(const Complex &z) const
{
    return Expression(*this).divide(z);
}

/** Return an expression that is this / ex */
Expression CASNode::divide(const CASNode &node) const
{
    return Expression(*this).divide(node);
}

/** Return an expression that the differential of this CASNode
    with respect to 'symbol'. Note an exception may
    be thrown if this CASNode cannot be differentiated.

    \throw SireCAS::unavailable_differential
*/
Expression CASNode::differentiate(const Symbol &symbol) const
{
    throw SireCAS::unavailable_differential(QObject::tr(
        "The differential of \"%1\" with respect to \"%2\" is not available.")
            .arg(toString(), symbol.toString()), CODELOC);
}

/** Return an expression that the differential of this CASNode
    with respect to 'symbol'. Note an exception may
    be thrown if this CASNode cannot be differentiated.

    \throw SireCAS::unavailable_differential
*/
Expression CASNode::differentiate(const Symbol &symbol, int level) const
{
    if (level == 0)
    {
        return *this;
    }
    else if (level < 0)
    {
        //we want integration
        Expression ret(*this);
        
        for (int i=0; i>level; --i)
        {
            ret = ret.integrate(symbol);
        }
        
        return ret;
    }
    else
    {
        //we want differentiation
        Expression ret(*this);
        
        for (int i=0; i<level; ++i)
        {
            ret = ret.differentiate(symbol);
        }
        
        return ret;
    }
}

/** Return the indefinite integral of this 'CASNode' with respect to
    symbol. This is not guaranteed to work(!) and will return an
    expression of the form Sum( integral(exbase) + integral_constant ).
    If it doesn't work then an exception will be throw.

    \throw SireCAS::unavailable_integral
*/
Expression CASNode::integrate(const Symbol &symbol) const
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
Expression CASNode::series(const Symbol&, int) const
{
    return Expression(*this);
}

/** Try to simplify this expression. This will try to use known mathematical
    identities to convert complex expressions down to more simple ones.
    If SireCAS::UNSAFE_COMPLEX_SIMPLIFICATIONS is true, then identities
    that are not safe for complex math are used, e.g. z = sin(arcsin(z)). */
Expression CASNode::simplify(int) const
{
    return Expression(*this);
}

/** Return the complex conjugate of this expression */
Expression CASNode::conjugate() const
{
    return Expression(*this);
}

/** Return whether or not this is a function of the passed Symbol */
bool CASNode::isFunction(const Symbol&) const
{
    return false;
}

/** Return whether or not this is a constant expression (does not
    depend on any symbols) */
bool CASNode::isConstant() const
{
    return true;
}

/** Return whether or not this expression contains any complex (imaginary)
    parts */
bool CASNode::isComplex() const
{
    return false;
}

/** Return whether or not this is a compound expression, and thus as such
    requires brackets placed around it when it is printed. Examples include
    Sum, Product and Power. For most other functions it is safe to leave
    this as false. */
bool CASNode::isCompound() const
{
    return false;
}

namespace SireCAS
{
    Expression SIRECAS_EXPORT operator+(const CASNode &node0, const CASNode &node1)
    {
        return node0.add(node1);
    }

    Expression SIRECAS_EXPORT operator+(double val, const CASNode &node)
    {
        return node.add(val);
    }

    Expression SIRECAS_EXPORT operator+(const CASNode &node, double val)
    {
        return node.add(val);
    }

    Expression SIRECAS_EXPORT operator+(const Complex &val, const CASNode &node)
    {
        return node.add(val);
    }

    Expression SIRECAS_EXPORT operator+(const CASNode &node, const Complex &val)
    {
        return node.add(val);
    }

    Expression SIRECAS_EXPORT operator-(const CASNode &node0, const CASNode &node1)
    {
        return node0.subtract(node1);
    }

    Expression SIRECAS_EXPORT operator-(double val, const CASNode &node)
    {
        return Expression(val).subtract(node);
    }

    Expression SIRECAS_EXPORT operator-(const CASNode &node, double val)
    {
        return node.subtract(val);
    }

    Expression SIRECAS_EXPORT operator-(const Complex &val, const CASNode &node)
    {
        return Expression(val).subtract(node);
    }

    Expression SIRECAS_EXPORT operator-(const CASNode &node, const Complex &val)
    {
        return node.subtract(val);
    }

    Expression SIRECAS_EXPORT operator*(const CASNode &node0, const CASNode &node1)
    {
        return node0.multiply(node1);
    }

    Expression SIRECAS_EXPORT operator*(double val, const CASNode &node)
    {
        return node.multiply(val);
    }

    Expression SIRECAS_EXPORT operator*(const CASNode &node, double val)
    {
        return node.multiply(val);
    }

    Expression SIRECAS_EXPORT operator*(const Complex &val, const CASNode &node)
    {
        return node.multiply(val);
    }

    Expression SIRECAS_EXPORT operator*(const CASNode &node, const Complex &val)
    {
        return node.multiply(val);
    }

    Expression SIRECAS_EXPORT operator/(const CASNode &node0, const CASNode &node1)
    {
        return node0.divide(node1);
    }

    Expression SIRECAS_EXPORT operator/(double val, const CASNode &node)
    {
        return Expression(val).divide(node);
    }

    Expression SIRECAS_EXPORT operator/(const CASNode &node, double val)
    {
        return node.divide(val);
    }

    Expression SIRECAS_EXPORT operator/(const Complex &val, const CASNode &node)
    {
        return Expression(val).divide(node);
    }

    Expression SIRECAS_EXPORT operator/(const CASNode &node, const Complex &val)
    {
        return node.divide(val);
    }

    Expression SIRECAS_EXPORT sqrt(const CASNode &node)
    {
        return node.sqrt();
    }

    Expression SIRECAS_EXPORT cbrt(const CASNode &node)
    {
        return node.cbrt();
    }

    Expression SIRECAS_EXPORT root(const CASNode &node, int n)
    {
        return node.root(n);
    }

    Expression SIRECAS_EXPORT pow(const CASNode &node, int n)
    {
        return node.pow(n);
    }

    Expression SIRECAS_EXPORT pow(const CASNode &node, const Rational &n)
    {
        return node.pow(n);
    }

    Expression SIRECAS_EXPORT pow(const CASNode &node, double n)
    {
        return node.pow(n);
    }

    Expression SIRECAS_EXPORT pow(const CASNode &node, const Complex &n)
    {
        return node.pow(n);
    }

    Expression SIRECAS_EXPORT pow(const CASNode &node, const Expression &n)
    {
        return node.pow(n);
    }

    Expression SIRECAS_EXPORT pow(const CASNode &node, const CASNode &n)
    {
        return Expression(node).pow(Expression(n));
    }
}
