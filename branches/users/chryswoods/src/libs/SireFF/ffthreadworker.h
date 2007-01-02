#ifndef SIREFF_FFTHREADWORKER_H
#define SIREFF_FFTHREADWORKER_H

#include "ffworker.h"
#include "SireCluster/threadworker.h"

SIRE_BEGIN_HEADER

namespace SireFF
{

class FFCalculatorBase;

/** This class provides a worker that can calculate the energy and
    forces of a ForceField in a background thread (via the passed
    FFCalculator)

    @author Christopher Woods
*/
class SIREFF_EXPORT FFThreadWorker : public FFWorkerBase,
                                     public SireCluster::ThreadWorker
{
public:
    FFThreadWorker(FFCalculatorBase *ffcalculator);

    ~FFThreadWorker();

    ForceField forcefield() const;

protected:
    bool _pvt_setForceField(const ForceField &forcefield);

    bool _pvt_add(const Molecule &molecule, const ParameterMap &map);
    bool _pvt_add(const Residue &residue, const ParameterMap &map);

    bool _pvt_change(const Molecule &molecule);
    bool _pvt_change(const Residue &residue);

    bool _pvt_remove(const Molecule &molecule);
    bool _pvt_remove(const Residue &residue);
    
    bool _pvt_replace(const Molecule &oldmol,
                      const Molecule &newmol, const ParameterMap &map);

    void _pvt_recalculateEnergy();
    void _pvt_recalculateEnergyFG();

    void _pvt_waitUntilReady();

    double _pvt_getEnergies(Values &nrg_components);

private:
    void calculate();

    /** Pointer to the calculator that is used to calculate the
        forcefield energies and forces */
    std::auto_ptr<FFCalculatorBase> ffcalculator;
};

}

SIRE_END_HEADER

#endif
