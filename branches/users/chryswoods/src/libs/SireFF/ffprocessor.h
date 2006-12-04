#ifndef SIREFF_FFPROCESSOR_H
#define SIREFF_FFPROCESSOR_H

#include "SireCluster/processor.h"

#include "forcefield.h"

SIRE_BEGIN_HEADER

namespace SireFF
{

class SIREFF_EXPORT FFProcessor : public SireCluster::Processor
{
public:
    FFProcessor();
    FFProcessor(const QString &name);
    
    ~FFProcessor();
    
    ForceField forcefield() const;

    double energy();
    double energy(const Function &component);
    
    Values energies();

    const Molecule& molecule(MoleculeID molid);
    
    void move(const Molecule &molecule);
    void move(const Residue &residue);

protected:
    const ForceField& ff() const;
    ForceField& ff();

    virtual void recalculateEnergy()=0; 

private:
    /** The forcefield being processed on this processor */
    ForceField ffield;

};

}

SIRE_END_HEADER

#endif

