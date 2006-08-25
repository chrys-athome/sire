#ifndef SIREFF_FFIELD_H
#define SIREFF_FFIELD_H

#include "SireBase/dynamicsharedptr.hpp"

SIRE_BEGIN_HEADER

namespace SireFF
{
class FField;
}

QDataStream& operator<<(QDataStream&, const SireFF::FField&);
QDataStream& operator>>(QDataStream&, SireFF::FField&);

namespace SireFF
{

class FFWorker;

/**
This class holds a dynamic shared pointer to an FFWorker, and provides the "worker" interface for the forcefield classes that is used by the internal code.

This class is called FField as it provides the 'little' interface to ForceField.

@author Christopher Woods
*/
class FField
{

friend QDataStream& ::operator<<(QDataStream&, const FField&);
friend QDataStream& ::operator>>(QDataStream&, FField&);

public:
    FField();
    FField(const FFWorker &ffworker);
    FField(const FField &other);
    
    ~FField();

    FField& operator=(const FField &other);
    FField& operator=(const FFWorker &ffworker);

    const char* what() const;
    
    bool isDirty() const;
    
    void moved(const Molecule &molecule);
    void moved(const Residue &residue);
    
    void changed(const ChangedMols &changedmols);
    void moved(const MovedMols &movedmols);
    
    Energy energy();
    Energy energy(const Symbol &symbol);
    
    void force(ForceTable &forces);
    void force(const Molecule &mol, ForceTable &forces);
    void force(const Residue &res, ForceTable &forces);
    void force(const MoleculeIDSet &mols, ForceTable &forces);
    
    void addForce(ForceTable &forces);
    void addForce(const Molecule &mol, ForceTable &forces, double fac=1);
    void addForce(const Residue &res, ForceTable &forces, double fac=1);
    void addForce(const MoleculeIDSet &mols, ForceTable &forces, double fac=1);
    
    void subtractForce(ForceTable &forces);
    void subtractForce(const Molecule &mol, ForceTable &forces, double fac=1);
    void subtractForce(const Residue &res, ForceTable &forces, double fac=1);
    void subtractForce(const MoleculeIDSet &mols, ForceTable &forces, double fac=1);
    
private:
    /** Dynamic shared pointer to the FFWorker instance */
    SireBase::DynamicSharedPtr<FFWorker> d;
};

}

SIRE_END_HEADER

#endif
