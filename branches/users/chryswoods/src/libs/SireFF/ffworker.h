#ifndef SireFF_FFWORKER_H
#define SireFF_FFWORKER_H

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireFF
{
class FFWorker;
}

QDataStream& operator<<(QDataStream&, const SireFF::FFWorker&);
QDataStream& operator>>(QDataStream&, SireFF::FFWorker&);

namespace SireFF
{

/**
This class is the base class of all of the forcefield worker classes. Each forcefield has two interfaces;

The first interface is the "worker" interface. This is used by the internal code to evaluate energies and forces. The worker interface has no editing functions and provides a limited interface that is used by the internal code.

The second interface is the "user" interface. This is the view of the forcefield that is seen by the user of the code. This interface provides nice editing functions, allowing the user to manipulate the forcefield however they wish.

The reason for the split is to prevent the editing of a forcefield during a running simulation. If editing is prevented, then the forcefield knows that it can cache values between calls, and do other clever tricks that may speed up the energy evaluation.

This is the base class of the "worker" interface, and as such provides the worker interface. This class can then be taken by FField, which holds this polymorphic class via a DynamicSharedPtr.

@author Christopher Woods
*/
class SIREFF_EXPORT FFWorker
{

friend QDataStream& ::operator<<(QDataStream&, const ForceField&);
friend QDataStream& ::operator>>(QDataStream&, ForceField&);

public:
    FFWorker(const QString &name = QString::null);
    virtual ~FFWorker();

    /** Return the class name of the forcefield */
    virtual const char* what() const=0;
    
    /** Return a clone of this forcefield */
    virtual FFWorker* clone() const=0;
    
    bool isDirty() const;
    
    /** Return the total energy of this forcefield. This
        will trigger an energy calculation if any
        molecules in this forcefield have moved or changed since
        the last call to 'energy()' (if 'isDirty()' returns 
        true) */
    Energy energy();
    
    /** Return the energy component corresponding to the symbol 'symbol' 
         - this throws an exception if there is no such component in
         this forcefield */
    Energy energy(const Symbol &symbol);
    
    /** Return the complete set of energy components of this forcefield */
    QHash<Symbol, Energy> energyComponents() const;
    
    /** Inform the forcefield that the listed molecules / residues have moved */
    void move(const MoveMols &movemols);

protected:
    /** Rename the component-symbol 'symbol' to have name 'newname' */
    void rename(const Symbol &symbol, const QString &newname);

    /** Set the energy value of the component 'comp' */
    void setComponent(const Symbol &comp, double nrg);

    /** Virtual function used to trigger a recalculation of the total energy
        and of all of the component energies */
    virtual void recalculateEnergy()=0;

private:
    /** The name of this forcefield - this may be used to give a unique
        name to all of the component-symbols in this forcefield. */
    QString ffname;

    /** The cached value of the total energy of this forcefield */
    Energy totalnrg;

    /** All of the energy components in this forcefield, indexed 
        by their symbol ID number */
    QHash<SymbolID, Energy> comps;
    
    /** Set of MoleculeID numbers of the molecules that are in this forcefield */
    QSet<MoleculeID> mols_in_ff;
    
    /** Set of MolResNumID numbers of residues that are in this forcefield that
        are here without their parent molecules */
    QSet<MolResNumID> res_in_ff;
    
    /** List of molecules/residues that have moved, changed or been
        added or removed since the last time the energy was calculated.
        This list contains only changes that will affect this forcefield. */
    ChangedMols changedmols;

};

}

SIRE_END_HEADER

#endif
