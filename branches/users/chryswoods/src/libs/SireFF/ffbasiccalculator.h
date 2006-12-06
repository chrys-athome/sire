#ifndef SIREFF_FFBASICCALCULATOR_H
#define SIREFF_FFBASICCALCULATOR_H

#include "ffcalculator.h"
#include "forcefield.h"

SIRE_BEGIN_HEADER

namespace SireFF
{

class SIREFF_EXPORT FFBasicCalculator : public FFCalculator
{
public:
    FFBasicCalculator();
    FFBasicCalculator(const ForceField &forcefield);

    ~FFBasicCalculator();

    double getEnergies(Values &values);

    const Molecule& molecule(MoleculeID molid) const;

    void calculateEnergy();

    bool move(const Molecule &molecule);

    bool move(const Residue &residue);

    bool setForceField(const ForceField &forcefield);

    ForceField forcefield() const;

private:
    /** The forcefield being evaluated */
    ForceField ffield;

    /** The total energy of the forcefield at the last calculation */
    double total_nrg;

    /** The values of the energy components at the last evaluation */
    Values nrg_components;
};

}

SIRE_END_HEADER

#endif
