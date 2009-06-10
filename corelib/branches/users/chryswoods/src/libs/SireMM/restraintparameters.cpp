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

#include "restraintparameters.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMM;
using namespace SireCAS;
using namespace SireStream;

/** Constructor */
RestraintSymbols::RestraintSymbols()
                 : r_("r"), theta("theta_"), phi_("phi")
{}

/** Destructor */
RestraintSymbols::~RestraintSymbols()
{}

/** Return the symbol used to represent a distance
    in a distance-based restraint */
const Symbol& RestraintSymbols::r() const
{
    return r_;
}

/** Return the symbol used to represent an angle
    in an angle-based restraint */
const Symbol& RestraintSymbols::theta() const
{
    return theta_;
}

/** Return the symbol used to represent a dihedral
    in a dihedral-based restraint */
const Symbol& RestraintSymbols::phi() const
{
    return phi_;
}
