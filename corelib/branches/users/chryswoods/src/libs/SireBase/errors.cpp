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

#include "SireBase/errors.h"

using namespace SireBase;
using namespace Siren;

//////////
////////// Implementation of missing_property
//////////

static const RegisterObject<missing_property> r_missing_property;

missing_property::missing_property() : ImplementsException<missing_property, exception>()
{}

missing_property::missing_property(QString err, QString place)
                 : ImplementsException<missing_property,exception>(err, place)
{}

missing_property::missing_property(const missing_property &other)
                 : ImplementsException<missing_property,exception>(other)
{}

missing_property::~missing_property() throw()
{}

//////////
////////// Implementation of duplicate_property
//////////

static const RegisterObject<duplicate_property> r_duplicate_property;

duplicate_property::duplicate_property() 
                   : ImplementsException<duplicate_property, exception>()
{}

duplicate_property::duplicate_property(QString err, QString place)
                   : ImplementsException<duplicate_property,exception>(err, place)
{}

duplicate_property::duplicate_property(const duplicate_property &other)
                   : ImplementsException<duplicate_property,exception>(other)
{}

duplicate_property::~duplicate_property() throw()
{}
