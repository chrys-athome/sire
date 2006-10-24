#ifndef SireFF_FFWORKER_H
#define SireFF_FFWORKER_H

#include "SireCAS/function.h"
#include "SireCAS/values.h"

#include "SireMol/moleculeid.h"
#include "SireMol/molresnumid.h"

#include "changedmols.h"

SIRE_BEGIN_HEADER

namespace SireFF
{
class FFWorker;
}

QDataStream& operator<<(QDataStream&, const SireFF::FFWorker&);
QDataStream& operator>>(QDataStream&, SireFF::FFWorker&);

namespace SireMol
{
class Molecule;
class Residue;
}

namespace SireDB
{
class ParameterTable;
}

namespace SireFF
{

class MovedMols;

using SireCAS::Function;
using SireCAS::Values;

using SireMol::MoleculeID;
using SireMol::MolResNumID;
using SireMol::Molecule;
using SireMol::Residue;

using SireDB::ParameterTable;

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
    FFWorker();
    FFWorker(const QString &name);
    
    virtual ~FFWorker();

    /** Return the class name of the forcefield */
    virtual const char* what() const=0;
    
    /** Return a clone of this forcefield */
    virtual FFWorker* clone() const=0;
    
    const QString& name() const;
    
    double energy();
    double energy(const Function &component);
    
    Values energies() const;
    
    const Function& total() const;

    const Function& component(const QString &name) const;
    const QHash<Function,QString>& components() const;
    
    void move(const MovedMols &movemols);

    void move(const Molecule &mol);
    void move(const Residue &res);

    void change(const ChangedMols &changemols);

    void change(const Molecule &mol, const ParameterTable &params);
    void change(const Residue &res, const ParameterTable &params);

protected:
    void setComponent(const Function &comp, double nrg);

    void addToRegister(const Molecule &molecule);
    void addToRegister(const Residue &residue);

    const ChangedMols& changedMolecules() const;

    /** Virtual function used to trigger a recalculation of the total energy
        and of all of the component energies */
    virtual void recalculateEnergy()=0;

private:
    /** The name of this forcefield - this may be used to give a unique
        name to all of the component-symbols in this forcefield. */
    QString ffname;

    /** All of the cached energy components in this forcefield, indexed 
        by their symbol ID number (includes the total energy) */
    Values nrg_components;
    
    /** The symbols (actually, functions) representing each of the energy
        components, together with a description of what each component
        represents (includes the total energy) */
    QHash< Function, QString > component_descriptions;
    
    /** The function representing the total energy of this forcefield */
    Function etotal;
    
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

/** Set the energy value of the component 'comp' */
inline void FFWorker::setComponent(const Function &comp, double nrg)
{
    nrg_components.set(comp,nrg);
}

/** Return the set of molecules that have moved or changed
    in this forcefield since the last energy or force evaluation */
inline const ChangedMols& FFWorker::changedMolecules() const
{
    return changedmols;
}

}

SIRE_END_HEADER

#endif
