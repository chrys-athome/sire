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

#ifndef SIREMOL_RESID_H
#define SIREMOL_RESID_H

#include "SireID/id.h"

SIRE_BEGIN_HEADER

namespace SireMol
{

class ResIdx;
class ResIdentifier;
class MoleculeInfoData;

/** This is the base class of all identifiers that are used 
    to identify a residue within a molecule

    @author Christopher Woods
*/
class SIREMOL_EXPORT ResID : public SireID::ID
{

public:

    /** The type used to index a Residue */
    typedef ResIdx Index;
    
    /** The generic type used to identify a residue */
    typedef ResIdentifier Identifier;

    ResID() : SireID::ID()
    {}

    ResID(const ResID &other) : SireID::ID(other)
    {}

    ~ResID()
    {}
    
    virtual ResID* clone() const=0;

    /** Map this ID back to the indicies of the residues in the molecule, 
        using the passed MoleculeInfo to do the mapping */
    virtual QList<ResIdx> map(const MoleculeInfoData &molinfo) const=0;
};

}

SIRE_END_HEADER

#endif

