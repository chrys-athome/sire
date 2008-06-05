#ifndef SIREMOL_ATOMSET_H
#define SIREMOL_ATOMSET_H

#include "atom.h"

#include "SireBase/set.hpp"

using SireMaths::Vector;

SIRE_BEGIN_HEADER

namespace SireMol
{

typedef SireBase::Set<AtomIndex,Atom> AtomSet;

/** This function returns the 'n' nearest atoms in 'atoms' to coordinates 'point' */
AtomSet getNearest(const Vector &point, const AtomSet &atoms, int n);

/** This function removes atoms from 'atoms' such that the remaining atoms do not
    contain any pairs that are closer than 'dist' A from one another */
void removeNearAtoms(AtomSet &atoms, double dist);

/** This function removes atoms from 'atoms' such that no three atoms in the set
    lie along a straight line (have an angle between then of 180 +/- delta or 0 +/- delta) */
void removeLinearAtoms(AtomSet &atoms, const SireMaths::Angle &delta);

}

SIRE_END_HEADER

#endif
