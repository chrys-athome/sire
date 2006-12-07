#ifndef SIREFF_FFWORKER_H
#define SIREFF_FFWORKER_H

#include "SireCluster/processor.h"
#include "SireCAS/values.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class Molecule;
class Residue;
class MoleculeID;
}

namespace SireCAS
{
class Function;
}

namespace SireFF
{

class ForceField;
class FFCalculator;

using SireMol::Molecule;
using SireMol::Residue;
using SireMol::MoleculeID;

using SireCAS::Function;
using SireCAS::Values;

/** This is the base class of all FFWorkers - an FFWorker is a class
    that evaluates a ForceField on a (possibly) remote thread or processor.

    @author Christopher Woods
*/
class SIREFF_EXPORT FFWorkerBase
{
public:
    virtual ~FFWorkerBase();

    virtual ForceField forcefield() const=0;
    void setForceField(const ForceField &forcefield);

    virtual Molecule molecule(MoleculeID molid) const=0;

    void recalculateEnergy();
    void waitUntilReady();

    double energy();
    double energy(const Function &component);

    Values energies();

    void move(const Molecule &molecule);
    void move(const Residue &residue);

    void assertContains(const Function &component) const;

protected:
    FFWorkerBase();

    virtual bool _pvt_setForceField(const ForceField &forcefield)=0;

    virtual bool _pvt_move(const Molecule &molecule)=0;
    virtual bool _pvt_move(const Residue &residue)=0;

    virtual void _pvt_recalculateEnergy()=0;
    virtual void _pvt_recalculateEnergyFG()=0;

    virtual void _pvt_waitUntilReady()=0;

    virtual double _pvt_getEnergies(Values &components)=0;

private:
    void checkEnergiesUpToDate();

    /** The total energy of the forcefield */
    double total_nrg;

    /** The values of the energy components */
    Values nrg_components;

    /** The different states of this processor */
    enum FF_STATE{ IDLE  = 0x00000000,  ///< The processor is idle
                   CALCULATING_ENERGY = 0x00000001,  ///< Energy recalculation is underway
                   CALCULATING_FORCE = 0x00000002   ///< Force recalculation is underway
                 };

    /** The current status of this processor */
    FF_STATE current_state;

    /** Whether or not the energy needs recalculating */
    bool needs_energy_recalculation;
};

/** This is a test class which calculates the forcefield energy in the main thread.

    @author Christopher Woods
*/
class SIREFF_EXPORT FFWorker : public FFWorkerBase, public SireCluster::WorkerBase
{
public:
    FFWorker(FFCalculator *ffcalculator);

    ~FFWorker();

    ForceField forcefield() const;

    Molecule molecule(MoleculeID molid) const;

protected:
    bool _pvt_setForceField(const ForceField &forcefield);

    bool _pvt_move(const Molecule &molecule);
    bool _pvt_move(const Residue &residue);

    void _pvt_recalculateEnergy();
    void _pvt_recalculateEnergyFG();

    void _pvt_waitUntilReady();

    double _pvt_getEnergies(Values &components);

private:
    /** The calculator used to evaluate the forcefield */
    std::auto_ptr<FFCalculator> ffcalculator;
};

}

SIRE_END_HEADER

#endif
