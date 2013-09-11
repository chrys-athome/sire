/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2008  Christopher Woods
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

#include "latticecharges.h"

#include "SireID/index.h"

#include "SireBase/quickcopy.hpp"

using namespace Squire;
using namespace SireID;
using namespace SireBase;

////////
//////// Implementation of LatticeCharge
////////

/** Create a null charge at the origin */
LatticeCharge::LatticeCharge()
{
    for (int i=0; i<4; ++i)
    {
        d[i] = 0;
    }
}

/** Create a point charge at the specified location with the specified
    value */
LatticeCharge::LatticeCharge(double x, double y, double z, double charge)
{
    d[0] = x;
    d[1] = y;
    d[2] = z;
    d[3] = charge;
}

/** Create a point charge at the specified location with the specified charge */
LatticeCharge::LatticeCharge(const Vector &point, double charge)
{
    d[0] = point.x();
    d[1] = point.y();
    d[2] = point.z();
    d[3] = charge;
}

/** Copy constructor */
LatticeCharge::LatticeCharge(const LatticeCharge &other)
{
    quickCopy<double>(d, other.d, 4);
}

/** Destructor */
LatticeCharge::~LatticeCharge()
{}

/** Copy assignment operator */
LatticeCharge& LatticeCharge::operator=(const LatticeCharge &other)
{
    if (this != &other)
    {
        quickCopy<double>(d, other.d, 4);
    }

    return *this;
}

/** Comparison operator */
bool LatticeCharge::operator==(const LatticeCharge &other) const
{
    return d[0] == other.d[0] and d[1] == other.d[1] and
           d[2] == other.d[2] and d[3] == other.d[3];
}

/** Comparison operator */
bool LatticeCharge::operator!=(const LatticeCharge &other) const
{
    return d[0] != other.d[0] or d[1] != other.d[1] or
           d[2] != other.d[2] or d[3] != other.d[3];
}

////////
//////// Implementation of LatticeCharges
////////

/** Construct an empty set of charges */
LatticeCharges::LatticeCharges()
{}

/** Copy constructor */
LatticeCharges::LatticeCharges(const LatticeCharges &other)
               : lattice_charges(other.lattice_charges)
{}

/** Destructor */
LatticeCharges::~LatticeCharges()
{}

/** Reserve space for 'n' lattice charges - this is useful to speed
    things up by preventing memory reallocations if you already have 
    a good idea of how many lattice charges you need */
void LatticeCharges::reserve(int n)
{
    lattice_charges.reserve(n);
}

/** Add the lattice charge 'charge' */
void LatticeCharges::add(const LatticeCharge &charge)
{
    lattice_charges.append(charge);
}

/** Return whether or not there are no charges (this set is empty) */
bool LatticeCharges::isEmpty() const
{
    return lattice_charges.isEmpty();
}

/** Return the number of charges in this set */
int LatticeCharges::nCharges() const
{
    return lattice_charges.count();
}

/** Return the number of charges in this set */
int LatticeCharges::count() const
{
    return this->nCharges();
}

/** Return the 'ith' lattice charge

    \throw SireError::invalid_index
*/
const LatticeCharge& LatticeCharges::operator[](int i) const
{
    return lattice_charges.constData()[ Index(i).map(lattice_charges.count()) ];
}

/** Return a raw pointer to the array of lattice charges */
const LatticeCharge* LatticeCharges::constData() const
{
    return lattice_charges.constData();
}
