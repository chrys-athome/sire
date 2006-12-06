
#include "ffbasiccalculator.h"

#include "SireMol/molecule.h"
#include "SireMol/moleculeid.h"
#include "SireMol/residue.h"

#include "SireCAS/values.h"

using namespace SireFF;
using namespace SireMol;
using namespace SireCAS;

/** Constructor, using a null forcefield */
FFBasicCalculator::FFBasicCalculator()
                  : FFCalculator(), total_nrg(0)
{}

/** Constructor, specifying the forcefield */
FFBasicCalculator::FFBasicCalculator(const ForceField &forcefield)
                  : FFCalculator(), ffield(forcefield), total_nrg(0)
{
    if (not ffield.isDirty())
    {
        nrg_components = ffield.energies();
        total_nrg = nrg_components.value( ffield.total() );
    }
}

/** Destructor */
FFBasicCalculator::~FFBasicCalculator()
{}

/** Return the total energy of the forcefield, and populate
    'components' with all of the energy components. */
double FFBasicCalculator::getEnergies(Values &components)
{
    components = nrg_components;

    return total_nrg;
}

/** Return the molecule with ID == molid

    \throw SireMol::missing_molecule
*/
const Molecule& FFBasicCalculator::molecule(MoleculeID molid) const
{
    return ffield.molecule(molid);
}

/** Recalculate the energy of the forcefield */
void FFBasicCalculator::calculateEnergy()
{
    nrg_components = ffield.energies();
    total_nrg = nrg_components.value(ffield.total());
}

/** Move the molecule 'molecule' and return whether the forcefield
    energy needs recalculating */
bool FFBasicCalculator::move(const Molecule &molecule)
{
    return ffield.move(molecule);
}

/** Move the residue 'residue' and return whether the forcefield
    energy needs recalculating */
bool FFBasicCalculator::move(const Residue &residue)
{
    return ffield.move(residue);
}

/** Set the forcefield to be calculated and return whether its
    energy needs to be reevaluated. */
bool FFBasicCalculator::setForceField(const ForceField &forcefield)
{
    ffield = forcefield;

    return ffield.isDirty();
}

/** Return a copy of the forcefield that is being evaluated */
ForceField FFBasicCalculator::forcefield() const
{
    return ffield;
}
