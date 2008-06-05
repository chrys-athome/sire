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

#ifndef SIREMOL_SPLITMOLECULE_H
#define SIREMOL_SPLITMOLECULE_H

#include <boost/tuple/tuple.hpp>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireMol
{

class AtomIDGroup;
class Bond;
class Angle;
class Dihedral;
class Improper;

class ResidueBonds;
class MoleculeBonds;

typedef boost::tuple<AtomIDGroup,AtomIDGroup> AtomIDGroupPair;

/**
This file contains functions that may be used to split a molecule into groups, based on 
the connectivity of the molecule represented via MoleculeBonds/ResidueBonds. The molecule is split
into two groups, represented via the AtomIDGroup class.

@author Christopher Woods
*/

AtomIDGroupPair splitResidue(const Bond &bnd, const ResidueBonds &connectivity);
AtomIDGroupPair splitResidue(const SireMol::Angle &ang, const ResidueBonds &connectivity);
AtomIDGroupPair splitResidue(const Dihedral &dih, const ResidueBonds &connectivity);
AtomIDGroupPair splitResidue(const Improper &improper, const ResidueBonds &connectivity);

AtomIDGroupPair splitMolecule(const Bond &bnd, const MoleculeBonds &connectivity);
AtomIDGroupPair splitMolecule(const SireMol::Angle &ang, const MoleculeBonds &connectivity);
AtomIDGroupPair splitMolecule(const Dihedral &dih, const MoleculeBonds &connectivity);
AtomIDGroupPair splitMolecule(const Improper &improper, const MoleculeBonds &connectivity);

}

SIRE_END_HEADER

#endif
