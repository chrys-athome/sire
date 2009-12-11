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

#include "exp.h"
#include "values.h"
#include "complexvalues.h"

#include "SireMaths/complex.h"

#include "Siren/stream.h"

using namespace Siren;
using namespace SireMaths;
using namespace SireCAS;

/////////////
///////////// Implementation of Exp
/////////////

static const RegisterObject<Exp> r_exp;

/** Construct an empty Exp (e^0) */
Exp::Exp() : Implements<Exp,PowerFunction>()
{}

/** Construct e^power */
Exp::Exp(const Expression &power) : Implements<Exp,PowerFunction>(), pwr(power)
{}

/** Copy constructor */
Exp::Exp(const Exp &other) : Implements<Exp,PowerFunction>(other), pwr(other.pwr)
{}

/** Destructor */
Exp::~Exp()
{}

Exp& Exp::operator=(const Exp &other)
{
    if (this != &other)
    {
        pwr = other.pwr;
        PowerFunction::operator=(other);
    }
    
    return *this;
}

bool Exp::operator==(const Exp &other) const
{
    return pwr == other.pwr and PowerFunction::operator==(other);
}

bool Exp::operator!=(const Exp &other) const
{
    return not Exp::operator==(other);
}

uint Exp::hashCode() const
{
   return qHash(Exp::typeName()) + pwr.hashCode(); 
}

/** Return a string representation of this function */
QString Exp::toString() const
{
    if (pwr.isZero())
        return "1";
    else
        return QObject::tr("exp( %1 )").arg(pwr.toString());
}

void Exp::stream(Stream &s)
{
    s.assertVersion<Exp>(1);
    
    Schema schema = s.item<Exp>();
    
    schema.data("power") & pwr;
    
    PowerFunction::stream( schema.base() );
}

/** Evaluate this expression at 'values' */
double Exp::evaluate(const Values &values) const
{
    return std::exp( pwr.evaluate(values) );
}

/** Complex evaluate */
Complex Exp::evaluate(const ComplexValues &values) const
{
    return SireMaths::exp( pwr.evaluate(values) );
}

/** Differentiatial of e^x is e^x */
Expression Exp::differentiate(const Symbol &symbol) const
{
    // d e^f(x) / dx = f'(x) * e^f(x)
    return pwr.differentiate(symbol) * Expression(*this);
}

/** Integral of e^x is e^x + c */
Expression Exp::integrate(const Symbol &symbol) const
{
    return Exp(pwr) / pwr.diff(symbol);
}

/** Return the core of this power */
Expression Exp::core() const
{
    return Expression( SireMaths::e );
}

/** Return the power of this power */
Expression Exp::power() const
{
    return pwr;
}

/////////////
///////////// Implementation of Ln
/////////////

static const RegisterObject<Ln> r_ln;

/** Construct an empty ln (ln(0)) */
Ln::Ln() : Implements<Ln,SingleFunc>()
{}

/** Construct ln(expression) */
Ln::Ln(const Expression &expression) : Implements<Ln,SingleFunc>(expression)
{}

/** Copy constructor */
Ln::Ln(const Ln &other) : Implements<Ln,SingleFunc>(other)
{}

/** Destructor */
Ln::~Ln()
{}

Ln& Ln::operator=(const Ln &other)
{
    SingleFunc::operator=(other);
    return *this;
}

bool Ln::operator==(const Ln &other) const
{
    return Ln::operator==(other);
}

bool Ln::operator!=(const Ln &other) const
{
    return Ln::operator!=(other);
}

void Ln::stream(Stream &s)
{
    s.assertVersion<Ln>(1);
    
    Schema schema = s.item<Ln>();
    
    SingleFunc::stream( schema.base() );
}

/** Evaluate this function */
double Ln::evaluate(const Values &values) const
{
    return std::log( x().evaluate(values) );
}

/** Complex evaluation */
Complex Ln::evaluate(const ComplexValues &values) const
{
    return SireMaths::log( x().evaluate(values) );
}

Expression Ln::functionOf(const Expression &arg) const
{
    if (arg == argument())
        return *this;
    else
        return Ln(arg);
}

QString Ln::stringRep() const
{
    return QObject::tr("ln");
}

/** differential of ln(x) = 1/x */
Expression Ln::diff() const
{
    // d ln(f(x)) / dx = f'(x) / f(x)
    return 1 / x();
}

/** Integral of ln(x) = x ln(x) - x + C */
Expression Ln::integ() const
{
    return x()*Ln(x()) - x();
}
