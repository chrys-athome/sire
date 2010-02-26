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

#include "SireCluster/errors.h"

using namespace SireCluster;
using namespace Siren;

//////////
////////// Implementation of network_error
//////////

static const RegisterObject<network_error> r_network_error;

network_error::network_error() 
              : ImplementsException<network_error, exception>()
{}

network_error::network_error(QString err, QString place)
              : ImplementsException<network_error,exception>(err, place)
{}

network_error::network_error(const network_error &other)
              : ImplementsException<network_error,exception>(other)
{}

network_error::~network_error() throw()
{}