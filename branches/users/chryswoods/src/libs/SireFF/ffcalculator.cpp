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

#include "SireCAS/qhash_sirecas.h"

#include "SireMol/molecule.h"

#include "ffcalculator.h"

#include "SireError/errors.h"

using namespace SireFF;
using namespace SireMol;

/////////
///////// Implementation of FFCalculatorBase
/////////

/** Constructor */
FFCalculatorBase::FFCalculatorBase() : boost::noncopyable()
{}

/** Destructor */
FFCalculatorBase::~FFCalculatorBase()
{}

/////////
///////// Implementation of FFCalculator
/////////

/** Constructor, using a null forcefield */
FFCalculator::FFCalculator()
             : FFCalculatorBase(), total_nrg(0)
{}

/** Constructor, specifying the forcefield */
FFCalculator::FFCalculator(const ForceField &forcefield)
             : FFCalculatorBase(), ffield(forcefield), total_nrg(0)
{
    if (not ffield.isDirty())
    {
        nrg_components = ffield.energies();
        total_nrg = nrg_components.value( ffield.components().total() );
    }
}

/** Destructor */
FFCalculator::~FFCalculator()
{}

/** Return the total energy of the forcefield, and populate
    'components' with all of the energy components. */
double FFCalculator::getEnergies(Values &components)
{
    components = nrg_components;

    return total_nrg;
}

/** Recalculate the energy of the forcefield */
void FFCalculator::calculateEnergy()
{
    nrg_components = ffield.energies();
    total_nrg = nrg_components.value(ffield.components().total());
}

bool FFCalculator::add(const PartialMolecule &molecule, const ParameterMap &map)
{
    return ffield.add(molecule, map);
}

bool FFCalculator::change(const PartialMolecule &molecule)
{
    return ffield.change(molecule);
}

bool FFCalculator::remove(const PartialMolecule &molecule)
{
    return ffield.remove(molecule);
}
             
/** Set the forcefield to be calculated and return whether its
    energy needs to be reevaluated. */
bool FFCalculator::setForceField(const ForceField &forcefield)
{
    ffield = forcefield;

    if (ffield.isDirty())
        return true;
    else
    {
        nrg_components = ffield.energies();
        total_nrg = nrg_components.value(ffield.components().total());
        return false;
    }
}

/** Return a copy of the forcefield that is being evaluated */
ForceField FFCalculator::forcefield() const
{
    return ffield;
}
