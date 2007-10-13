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

#include "resid.h"
#include "residentifier.h"

#include "specify.hpp"

using namespace SireMol;
using namespace SireID;

/** Constructor */
ResID::ResID() : ID()
{}

/** Copy constructor */
ResID::ResID(const ResID &other) : ID(other)
{}

/** Destructor */
ResID::~ResID()
{}
  
/** Return a specific residue that matches this ID */
Specify<ResID> ResID::operator[](int i) const
{
    return Specify<ResID>(*this, i);
}

/** Return a specific residue that matches this ID */
Specify<ResID> ResID::operator()(int i) const
{
    return this->operator[](i);
}

/** Return a range of residues that match this ID */
Specify<ResID> ResID::operator()(int i, int j) const
{
    return Specify<ResID>(*this, i, j);
}

/** Return the atoms in the matching residues */
AtomsIn<ResID> ResID::atoms() const
{
    return AtomsIn<ResID>(*this);
}

/** Return a specific atom in the matching residues */
AtomsIn<ResID> ResID::atom(int i) const
{
    return AtomsIn<ResID>(*this, i);
}

/** Return a range of atoms in the matching residues */
AtomsIn<ResID> ResID::atoms(int i, int j) const
{
    return AtomsIn<ResID>(*this, i, j);
}

//fully instantiate Specify<ResID> and AtomsIn<ResID>
template class Specify<ResID>;
template class AtomsIn<ResID>;
