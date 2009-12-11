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

void CASNode::stream(Stream &s)
{
    s.assertVersion<CASNode>(1);
    
    Schema schema = s.item<CASNode>();
    
    Object::stream( schema.base() );
}

/** Return the negative of this CASNode */
Expression CASNode::operator-() const
{
    return -(Expression(*this));
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
        return Expression(node0) + Expression(node1);
    }

    Expression SIRECAS_EXPORT operator+(double val, const CASNode &node)
    {
        return Expression(node).add(val);
    }

    Expression SIRECAS_EXPORT operator+(const CASNode &node, double val)
    {
        return Expression(node).add(val);
    }

    Expression SIRECAS_EXPORT operator+(const Complex &val, const CASNode &node)
    {
        return Expression(node).add(val);
    }

    Expression SIRECAS_EXPORT operator+(const CASNode &node, const Complex &val)
    {
        return Expression(node).add(val);
    }

    Expression SIRECAS_EXPORT operator-(const CASNode &node0, const CASNode &node1)
    {
        return Expression(node0) - Expression(node1);
    }

    Expression SIRECAS_EXPORT operator-(double val, const CASNode &node)
    {
        return Expression(val).subtract(node);
    }

    Expression SIRECAS_EXPORT operator-(const CASNode &node, double val)
    {
        return Expression(node).subtract(val);
    }

    Expression SIRECAS_EXPORT operator-(const Complex &val, const CASNode &node)
    {
        return Expression(val).subtract(node);
    }

    Expression SIRECAS_EXPORT operator-(const CASNode &node, const Complex &val)
    {
        return Expression(node).subtract(val);
    }

    Expression SIRECAS_EXPORT operator*(const CASNode &node0, const CASNode &node1)
    {
        return Expression(node0) * Expression(node1);
    }

    Expression SIRECAS_EXPORT operator*(double val, const CASNode &node)
    {
        return Expression(node).multiply(val);
    }

    Expression SIRECAS_EXPORT operator*(const CASNode &node, double val)
    {
        return Expression(node).multiply(val);
    }

    Expression SIRECAS_EXPORT operator*(const Complex &val, const CASNode &node)
    {
        return Expression(node).multiply(val);
    }

    Expression SIRECAS_EXPORT operator*(const CASNode &node, const Complex &val)
    {
        return Expression(node).multiply(val);
    }

    Expression SIRECAS_EXPORT operator/(const CASNode &node0, const CASNode &node1)
    {
        return Expression(node0) / Expression(node1);
    }

    Expression SIRECAS_EXPORT operator/(double val, const CASNode &node)
    {
        return Expression(val).divide(node);
    }

    Expression SIRECAS_EXPORT operator/(const CASNode &node, double val)
    {
        return Expression(node).divide(val);
    }

    Expression SIRECAS_EXPORT operator/(const Complex &val, const CASNode &node)
    {
        return Expression(val).divide(node);
    }

    Expression SIRECAS_EXPORT operator/(const CASNode &node, const Complex &val)
    {
        return Expression(node).divide(val);
    }

    Expression SIRECAS_EXPORT pow(const CASNode &node, int n)
    {
        return Expression(node).pow(n);
    }

    Expression SIRECAS_EXPORT pow(const CASNode &node, const Rational &n)
    {
        return Expression(node).pow(n);
    }

    Expression SIRECAS_EXPORT pow(const CASNode &node, double n)
    {
        return Expression(node).pow(n);
    }

    Expression SIRECAS_EXPORT pow(const CASNode &node, const Complex &n)
    {
        return Expression(node).pow(n);
    }

    Expression SIRECAS_EXPORT pow(const CASNode &node, const Expression &n)
    {
        return Expression(node).pow(n);
    }

    Expression SIRECAS_EXPORT pow(const CASNode &node, const CASNode &n)
    {
        return Expression(node).pow(Expression(n));
    }
}
