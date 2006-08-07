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
