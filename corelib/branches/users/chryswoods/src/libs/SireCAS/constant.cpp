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

#include "constant.h"
#include "expression.h"
#include "symbol.h"
#include "values.h"
#include "complexvalues.h"
#include "identities.h"
#include "factor.h"

#include "SireMaths/complex.h"

#include "Siren/stream.h"

using namespace Siren;
using namespace SireMaths;
using namespace SireCAS;

static const RegisterObject<Constant> r_constant;

/** Construct a constant */
Constant::Constant() : Implements<Constant,CASNode>()
{}

/** Copy constructor */
Constant::Constant(const Constant &other) : Implements<Constant,CASNode>(other)
{}

/** Destructor */
Constant::~Constant()
{}

Constant& Constant::operator=(const Constant &other)
{
    CASNode::operator=(other);
    return *this;
}

bool Constant::operator==(const Constant&) const
{
    return true;
}

bool Constant::operator!=(const Constant&) const
{
    return false;
}

uint Constant::hashCode() const
{
    return qHash( Constant::typeName() );
}

void Constant::stream(Stream &s)
{
    s.assertVersion<Constant>(1);
    
    Schema schema = s.item<Constant>();
    
    CASNode::stream( schema.base() );
}

/** Return a string representation of this constant (actually an empty string!) */
QString Constant::toString() const
{
    return QString("1.0");
}

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
    return Expression(*this);
}

/** No symbols in a constant */
QSet<Symbol> Constant::symbols() const
{
    return QSet<Symbol>();
}

/** No children in a constant */
QList<Expression> Constant::children() const
{
    return QList<Expression>();
}

QList<Factor> Constant::expand(const Symbol &symbol) const
{
    QList<Factor> ret;
    ret.append( Factor( symbol, 1, 0 ) );
    return ret;
}
