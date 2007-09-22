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

#ifndef SIREMOL_ATOMIDCOMBO_H
#define SIREMOL_ATOMIDCOMBO_H

#include "atomid.h"

SIRE_BEGIN_HEADER

namespace SireMol
{

/** This class represents a combination of ID types that
    are all combined so that we can try to identify some
    atoms.
    
    @author Christopher Woods
*/
class AtomIDCombo : public AtomID
{
public:


private:
    /** The identifier for the segment */
    SegIdentifier segid;

    /** The identifier for the chain and residue */
    ResIDCombo resid;
    
    /** The identifier for the CutGroup */
    CGIdentifier cgid;
    
    /** The identifiers for the atom */
    AtomIdentifier atmid;
};

}

SIRE_END_HEADER

#endif
