/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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

#include "moleculerep.h"
#include "moldisplaydata.h"

#include "SireMol/editmol.h"

using namespace Spier;
using namespace SireMol;
using namespace SireMaths;
using namespace SireStream;

/** Construct the molecule representation from the data held in moldata */
MoleculeRep::MoleculeRep(const MolDisplayDataPtr &moldata) 
            : ViewObj(), prnt(moldata)
{
    if (moldata.get() != 0 and not moldata->isNull())
        renderatms = moldata->molecule().atoms();
}

/** Destructor */
MoleculeRep::~MoleculeRep()
{}

/** Return the molecule being represented */
EditMol MoleculeRep::molecule() const
{
    if (prnt.expired())
        return EditMol();
    else
        return prnt.lock()->molecule();
}

/** Return whether this is a null molecule rep - a null rep is one that 
    does not have a valid parent */
bool MoleculeRep::isNull() const
{
    return (prnt.expired() or prnt.lock()->isNull());
}
