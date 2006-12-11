
#include "SireCAS/qhash_sirecas.h"

#include "ffcalculator.h"

using namespace SireFF;

/////////
///////// Implementation of FFCalculatorBase
/////////

/** Constructor */
FFCalculatorBase::FFCalculatorBase()
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
        total_nrg = nrg_components.value( ffield.total() );
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

/** Return the molecule with ID == molid

    \throw SireMol::missing_molecule
*/
const Molecule& FFCalculator::molecule(MoleculeID molid) const
{
    return ffield.molecule(molid);
}

/** Recalculate the energy of the forcefield */
void FFCalculator::calculateEnergy()
{
    nrg_components = ffield.energies();
    total_nrg = nrg_components.value(ffield.total());
}

/** Move the molecule 'molecule' and return whether the forcefield
    energy needs recalculating */
bool FFCalculator::move(const Molecule &molecule)
{
    return ffield.move(molecule);
}

/** Move the residue 'residue' and return whether the forcefield
    energy needs recalculating */
bool FFCalculator::move(const Residue &residue)
{
    return ffield.move(residue);
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
        total_nrg = nrg_components.value(ffield.total());
        return false;
    }
}

/** Return a copy of the forcefield that is being evaluated */
ForceField FFCalculator::forcefield() const
{
    return ffield;
}
