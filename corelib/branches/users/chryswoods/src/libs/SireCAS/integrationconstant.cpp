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

#include "integrationconstant.h"
#include "expression.h"

#include "SireMaths/complex.h"

#include "SireCAS/errors.h"

#include "Siren/stream.h"

using namespace SireMaths;
using namespace Siren;
using namespace SireCAS;

static const RegisterObject<IntegrationConstant> r_intconst;

/** Constructor */
IntegrationConstant::IntegrationConstant()
                    : Implements<IntegrationConstant,Symbol>("C")
{}

/** Copy constructor */
IntegrationConstant::IntegrationConstant( const IntegrationConstant &other )
                    : Implements<IntegrationConstant,Symbol>(other)
{}

/** Destructor */
IntegrationConstant::~IntegrationConstant()
{}

IntegrationConstant& IntegrationConstant::operator=(const IntegrationConstant &other)
{
    Symbol::operator=(other);
    return *this;
}

bool IntegrationConstant::operator==(const IntegrationConstant &other) const
{
    return Symbol::operator==(other);
}

bool IntegrationConstant::operator!=(const IntegrationConstant &other) const
{
    return Symbol::operator!=(other);
}

void IntegrationConstant::stream(Stream &s)
{
    s.assertVersion<IntegrationConstant>(1);
    
    Schema schema = s.item<IntegrationConstant>();
    
    Symbol::stream( schema.base() );
}

/** Cannot integrate an expression containing an integration constant. This
    is to prevent integration constants from multiple integrations from
    appearing in the expression. */
Expression IntegrationConstant::integrate(const Symbol&) const
{
    throw SireCAS::unavailable_integral(QObject::tr(
        "Cannot integrate an IntegrationConstant. You must remove all "
        "integration constants from an expression before you can integrate "
        "it again (e.g. via ex.substitute(IntegrationConstant() == 0))."), CODELOC);

    return Expression();
}
