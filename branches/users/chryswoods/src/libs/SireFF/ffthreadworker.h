#ifndef SIREFF_FFTHREADWORKER_H
#define SIREFF_FFTHREADWORKER_H

#include "ffworker.h"
#include "SireCluster/threadworker.h"

SIRE_BEGIN_HEADER

namespace SireFF
{

class FFCalculator;

/** This class provides a worker that can calculate the energy and
    forces of a ForceField in a background thread (via the passed
    FFCalculator)

    @author Christopher Woods
*/
class SIREFF_EXPORT FFThreadWorker : public FFWorkerBase,
                                     public SireCluster::ThreadWorker
{
public:
    FFThreadWorker(FFCalculator *ffcalculator);

    ~FFThreadWorker();

    ForceField forcefield() const;

    Molecule molecule(MoleculeID molid) const;

protected:
    bool _pvt_setForceField(const ForceField &forcefield);

    bool _pvt_move(const Molecule &molecule);
    bool _pvt_move(const Residue &residue);

    void _pvt_recalculateEnergy();
    void _pvt_recalculateEnergyFG();

    void _pvt_waitUntilReady();

    double _pvt_getEnergies(Values &nrg_components);

private:
    void calculate();

    /** Pointer to the calculator that is used to calculate the
        forcefield energies and forces */
    std::auto_ptr<FFCalculator> ffcalculator;
};

}

SIRE_END_HEADER

#endif
