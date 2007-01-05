
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

bool FFCalculator::add(const Molecule &molecule, const ParameterMap &map)
{
    return ffield.add(molecule, map);
}

bool FFCalculator::add(const Residue &residue, const ParameterMap &map)
{
    return ffield.add(residue, map);
}

bool FFCalculator::change(const Molecule &molecule)
{
    return ffield.change(molecule);
}

bool FFCalculator::change(const Residue &residue)
{
    return ffield.change(residue);
}

bool FFCalculator::remove(const Molecule &molecule)
{
    return ffield.remove(molecule);
}

bool FFCalculator::remove(const Residue &residue)
{
    return ffield.remove(residue);
}

bool FFCalculator::replace(const Molecule &oldmol, const Molecule &newmol,
                           const ParameterMap &map)
{
    return ffield.replace(oldmol, newmol, map);
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
