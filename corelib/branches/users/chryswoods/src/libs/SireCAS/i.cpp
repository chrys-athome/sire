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

#include "i.h"
#include "expression.h"
#include "complexvalues.h"

#include "SireMaths/complex.h"
#include "SireMaths/errors.h"

#include "Siren/stream.h"

using namespace Siren;
using namespace SireMaths;
using namespace SireCAS;

static const RegisterObject<SireCAS::I> r_i;

/** Constructor */
I::I() : Implements<I,Constant>()
{}

/** Copy constructor */
I::I(const I &other) : Implements<I,Constant>(other)
{}

/** Destructor */
I::~I()
{}

I& I::operator=(const I &other)
{
    Constant::operator=(other);
    return *this;
}

bool I::operator==(const I &other) const
{
    return true;
}

bool I::operator!=(const I &other) const
{
    return false;
}

uint I::hashCode() const
{
    return qHash( I::typeName() );
}

void I::stream(Stream &s)
{
    s.assertVersion<I>(1);
    
    Schema schema = s.item<I>();
    
    Constant::stream( schema.base() );
}

/** Return a string representation */
QString I::toString() const
{
    return "i";
}

/** Cannot evaluate 'i' as a real number, so throw a domain error */
double I::evaluate(const Values&) const
{
    throw SireMaths::domain_error(QObject::tr(
        "Cannot evaluate 'i' as a real number"), CODELOC);
}

/** Evaluate this as a complex number - return 'i' */
Complex I::evaluate(const ComplexValues&) const
{
    return Complex(0,1);
}

/** Return the complex conjugate of 'i' (-i) */
Expression I::conjugate() const
{
    return -1 * I();
}

/** I is definitely complex :-) */
bool I::isComplex() const
{
    return true;
}
