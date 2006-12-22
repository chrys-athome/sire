#ifndef SIREFF_FFCALCULATOR_H
#define SIREFF_FFCALCULATOR_H

#include "forcefield.h"

#include <boost/noncopyable.hpp>

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

using SireCAS::Values;
using SireMol::Molecule;
using SireMol::Residue;
using SireMol::MoleculeID;

/** This is the pure virtual base class of all of the FFCalculatorBases. These are
    simple classes that provide an interface to a ForceField that allows their
    use by the FFProcessor classes.

    FFWorker classes are non-copyable!

    @author Christopher Woods
*/
class SIREFF_EXPORT FFCalculatorBase : public boost::noncopyable
{
public:
    FFCalculatorBase();

    virtual ~FFCalculatorBase();

    virtual double getEnergies(Values &values)=0;

    virtual const Molecule& molecule(MoleculeID molid) const=0;

    virtual void calculateEnergy()=0;

    virtual bool move(const Molecule &molecule)=0;

    virtual bool move(const Residue &residue)=0;

    virtual bool setForceField(const ForceField &forcefield)=0;

    virtual ForceField forcefield() const=0;
};

/** This is the default, and most simple, FFCalculator. This will
    work with any forcefield that can be used from the command line
    (pretty much all of them). The only exceptions are forcefields
    which require special calculators, e.g. MolproFF.

    @author Christopher Woods
*/
class SIREFF_EXPORT FFCalculator : public FFCalculatorBase
{
public:
    FFCalculator();
    FFCalculator(const ForceField &forcefield);

    ~FFCalculator();

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
