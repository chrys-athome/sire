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

#include "abs.h"
#include "values.h"
#include "complexvalues.h"

#include "SireMaths/complex.h"

#include "Siren/stream.h"

#include <cmath>

using namespace SireCAS;
using namespace Siren;

static const RegisterObject<Abs> r_abs;

/** Construct an empty Abs(0) */
Abs::Abs() : Implements<Abs,SingleFunc>()
{}

/** Construct abs(expression) */
Abs::Abs(const Expression &expression) : Implements<Abs,SingleFunc>(expression)
{}

/** Copy constructor */
Abs::Abs(const Abs &other) : Implements<Abs,SingleFunc>(other)
{}

/** Destructor */
Abs::~Abs()
{}

Abs& Abs::operator=(const Abs &other)
{
    SingleFunc::operator=(other);
    return *this;
}

bool Abs::operator==(const Abs &other) const
{
    return SingleFunc::operator==(other);
}

bool Abs::operator!=(const Abs &other) const
{
    return SingleFunc::operator!=(other);
}

void Abs::stream(Stream &s)
{
    s.assertVersion<Abs>(1);
    
    Schema schema = s.item<Abs>();
    
    SingleFunc::stream( schema.base() );
}

/** Evaluate this function */
double Abs::evaluate(const Values &values) const
{
    return std::abs( x().evaluate(values) );
}

/** Complex evaluation */
Complex Abs::evaluate(const ComplexValues &values) const
{
    Complex arg = x().evaluate(values);
    
    return Complex( std::abs(arg.real()), std::abs(arg.imag()) );
}

Expression Abs::functionOf(const Expression &arg) const
{
    if (arg == argument())
        return *this;
    else
        return Abs(arg);
}

QString Abs::stringRep() const
{
    return QObject::tr("abs");
}

/** d |x| / dx = x / |x|  (signum function) */
Expression Abs::diff() const
{
    return x() / *this;
}

/** Integral of |x| = 0.5( x |x| ) + C */
Expression Abs::integ() const
{
    return 0.5 * (x() * *this);
}
