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

#include "SireMaths/errors.h"

using namespace SireMaths;
using namespace Siren;

//////////
////////// Implementation of math_error
//////////

static const RegisterObject<math_error> r_math_error;

math_error::math_error() : ImplementsException<math_error, exception>()
{}

math_error::math_error(QString err, QString place)
            : ImplementsException<math_error,exception>(err, place)
{}

math_error::math_error(const math_error &other)
            : ImplementsException<math_error,exception>(other)
{}

math_error::~math_error() throw()
{}

//////////
////////// Implementation of domain_error
//////////

static const RegisterObject<domain_error> r_domain_error;

domain_error::domain_error() : ImplementsException<domain_error, exception>()
{}

domain_error::domain_error(QString err, QString place)
            : ImplementsException<domain_error,exception>(err, place)
{}

domain_error::domain_error(const domain_error &other)
            : ImplementsException<domain_error,exception>(other)
{}

domain_error::~domain_error() throw()
{}
