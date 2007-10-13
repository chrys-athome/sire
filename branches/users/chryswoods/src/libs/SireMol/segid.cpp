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

#include "segid.h"
#include "segidentifier.h"
#include "specify.hpp"

using namespace SireMol;
using namespace SireID;

/** Constructor */
SegID::SegID() : ID()
{}

/** Copy constructor */
SegID::SegID(const SegID &other) : ID(other)
{}

/** Destructor */
SegID::~SegID()
{}
  
/** Return a specific atom that matches this ID */
Specify<SegID> SegID::operator[](int i) const
{
    return Specify<SegID>(*this, i);
}

/** Return a specific atom that matches this ID */
Specify<SegID> SegID::operator()(int i) const
{
    return this->operator[](i);
}

/** Return a range of atoms that match this ID */
Specify<SegID> SegID::operator()(int i, int j) const
{
    return Specify<SegID>(*this, i, j);
}

/** Return the atoms in the matching residues */
AtomsIn<SegID> SegID::atoms() const
{
    return AtomsIn<SegID>(*this);
}

/** Return a specific atom in the matching residues */
AtomsIn<SegID> SegID::atom(int i) const
{
    return AtomsIn<SegID>(*this, i);
}

/** Return a range of atoms in the matching residues */
AtomsIn<SegID> SegID::atoms(int i, int j) const
{
    return AtomsIn<SegID>(*this, i, j);
}

//fully instantiate template classes
template class Specify<SegID>;
template class AtomsIn<SegID>;
