/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2008  Christopher Woods
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

#include "minmax.h"
#include "identities.h"
#include "expression.h"
#include "complexvalues.h"

#include "SireMaths/complex.h"
#include "SireMaths/errors.h"

#include "Siren/stream.h"

using namespace SireMaths;
using namespace Siren;
using namespace SireCAS;

////////
//////// Implementation of Min
////////

static const RegisterObject<Min> r_min;

/** Constructor */
Min::Min() : Implements<Min,DoubleFunc>()
{}

/** Construct min(x(), y()) */
Min::Min(const Expression &x, const Expression &y) : Implements<Min,DoubleFunc>(x,y)
{}

/** Copy constructor */
Min::Min(const Min &other) : Implements<Min,DoubleFunc>(other)
{}

/** Destructor */
Min::~Min()
{}

Min& Min::operator=(const Min &other)
{
    DoubleFunc::operator=(other);
    return *this;
}

bool Min::operator==(const Min &other) const
{
    return DoubleFunc::operator==(other);
}

bool Min::operator!=(const Min &other) const
{
    return DoubleFunc::operator!=(other);
}

void Min::stream(Stream &s)
{
    s.assertVersion<Min>(1);
    
    Schema schema = s.item<Min>();
    
    DoubleFunc::stream( schema.base() );
}

/** Evaluate this function */
double Min::evaluate(const Values &values) const
{
    return qMin( x().evaluate(values), y().evaluate(values) );
}

/** Complex evaluation */
Complex Min::evaluate(const ComplexValues &values) const
{
    Complex xval = x().evaluate(values);
    Complex yval = y().evaluate(values);

    if (xval == yval)
        return xval;
        
    else if (xval.isReal() and yval.isReal())
        return Complex( qMin(xval.real(),yval.real()), 0 );
        
    else    
        throw SireMaths::domain_error( QObject::tr(
            "It is not possible to order the two complex numbers %1 and %2. "
            "It is thus not possible to find the minimum of %3.")
                .arg(xval.toString(), yval.toString(), this->toString()), CODELOC );

    return Complex(0,0);
}

/** Return this as a function of x and y */
Expression Min::functionOf(const Expression &ex, const Expression &ey) const
{
     if (ex == x() and ey == y())
     {
        return Expression(*this);
     }
     else
        return Expression(Min(ex, ey));
}

QString Min::stringRep() const
{
    return QObject::tr("min");
}

////////
//////// Implementation of Max
////////

static const RegisterObject<Max> r_max;

/** Constructor */
Max::Max() : Implements<Max,DoubleFunc>()
{}

/** Construct min(x(), y()) */
Max::Max(const Expression &x, const Expression &y) : Implements<Max,DoubleFunc>(x,y)
{}

/** Copy constructor */
Max::Max(const Max &other) : Implements<Max,DoubleFunc>(other)
{}

/** Destructor */
Max::~Max()
{}

Max& Max::operator=(const Max &other)
{
    DoubleFunc::operator=(other);
    return *this;
}

bool Max::operator==(const Max &other) const
{
    return DoubleFunc::operator==(other);
}

bool Max::operator!=(const Max &other) const
{
    return DoubleFunc::operator!=(other);
}

void Max::stream(Stream &s)
{
    s.assertVersion<Max>(1);
    
    Schema schema = s.item<Max>();
    
    DoubleFunc::stream( schema.base() );
}

/** Evaluate this function */
double Max::evaluate(const Values &values) const
{
    return qMax( x().evaluate(values), y().evaluate(values) );
}

/** Complex evaluation */
Complex Max::evaluate(const ComplexValues &values) const
{
    Complex xval = x().evaluate(values);
    Complex yval = y().evaluate(values);

    if (xval == yval)
        return xval;
        
    else if (xval.isReal() and yval.isReal())
        return Complex( qMax(xval.real(),yval.real()), 0 );
        
    else    
        throw SireMaths::domain_error( QObject::tr(
            "It is not possible to order the two complex numbers %1 and %2. "
            "It is thus not possible to find the minimum of %3.")
                .arg(xval.toString(), yval.toString(), this->toString()), CODELOC );

    return Complex(0,0);
}

/** Return this as a function of x and y */
Expression Max::functionOf(const Expression &ex, const Expression &ey) const
{
     if (ex == x() and ey == y())
     {
        return Expression(*this);
     }
     else
        return Expression(Max(ex, ey));
}

QString Max::stringRep() const
{
    return QObject::tr("max");
}
