#ifndef SIREFF_FFCALCULATOR_H
#define SIREFF_FFCALCULATOR_H

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class Molecule;
class Residue;
class MoleculeID;
}

namespace SireCAS
{
class Values;
}

namespace SireFF
{

class ForceField;

using SireCAS::Values;
using SireMol::Molecule;
using SireMol::Residue;
using SireMol::MoleculeID;

/** This is the pure virtual base class of all of the FFCalculators. These are
    simple classes that provide an interface to a ForceField that allows their
    use by the FFProcessor classes.

    @author Christopher Woods
*/
class SIREFF_EXPORT FFCalculator
{
public:
    FFCalculator();

    virtual ~FFCalculator();

    virtual double getEnergies(Values &values)=0;

    virtual const Molecule& molecule(MoleculeID molid) const=0;

    virtual void calculateEnergy()=0;

    virtual bool move(const Molecule &molecule)=0;

    virtual bool move(const Residue &residue)=0;

    virtual bool setForceField(const ForceField &forcefield)=0;

    virtual ForceField forcefield() const=0;
};

}

SIRE_END_HEADER

#endif
