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

#include "residentifier.h"
#include "moleculeinfodata.h"

#include "specify.hpp"
#include "atomsin.hpp"

#include "SireStream/datastream.h"

using namespace SireMol;
using namespace SireID;

static const RegisterMetaType<ResIdentifier> r_resid;

///////
/////// Implementation of ResName
///////

QList<ResIdx> ResName::map(const MoleculeInfoData &molinfo) const
{
    return molinfo.map(*this);
}

///////
/////// Implementation of ResNum
///////

QList<ResIdx> ResNum::map(const MoleculeInfoData &molinfo) const
{
    return molinfo.map(*this);
}

///////
/////// Implementation of ResIdentifier
///////

/** Null constructor */
ResIdentifier::ResIdentifier() : Identifier_T_<ResIdentifier,ResID>()
{}

/** Construct from the passed ID */
ResIdentifier::ResIdentifier(const ResID &resid)
              : Identifier_T_<ResIdentifier,ResID>(resid)
{}
         
/** Copy constructor */
ResIdentifier::ResIdentifier(const ResIdentifier &other)
              : Identifier_T_<ResIdentifier,ResID>(other)
{}
  
/** Destructor */  
ResIdentifier::~ResIdentifier()
{}

/** Return a specific matching residue */
Specify<ResIdentifier> ResIdentifier::operator[](int i) const
{
    return Specify<ResIdentifier>(*this, i);
}

/** Return a specific matching residue */
Specify<ResIdentifier> ResIdentifier::operator()(int i) const
{
    return this->operator[](i);
}

/** Return a specific matching residue */
Specify<ResIdentifier> ResIdentifier::operator()(int i, int j) const
{
    return Specify<ResIdentifier>(*this, i, j);
}
    
/** Return the ID for the ith atom in the matching residue(s) */
AtomsIn<ResIdentifier> ResIdentifier::atom(int i) const
{
    return AtomsIn<ResIdentifier>(*this, i);
}

/** Return the IDs for all of the atoms in the matching residue(s) */
AtomsIn<ResIdentifier> ResIdentifier::atoms() const
{
    return AtomsIn<ResIdentifier>(*this, 0, -1);
}

/** Return the specified range of atoms in the matching residue(s) */
AtomsIn<ResIdentifier> ResIdentifier::atoms(int i, int j) const
{
    return AtomsIn<ResIdentifier>(*this, i, j);
}
   
/** Return the indicies of all of the residues that match this ID 

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
QList<ResIdx> ResIdentifier::map(const MoleculeInfoData &molinfo) const
{
    if (this->isNull())
        return molinfo.getResidues();
    else
        return this->asA<ResID>().map(molinfo);
}
