#ifndef SIREFF_FFIELD_H
#define SIREFF_FFIELD_H

#include "SireBase/dynamicsharedptr.hpp"

#include "ffworker.h"

SIRE_BEGIN_HEADER

namespace SireFF
{
class FField;
}

QDataStream& operator<<(QDataStream&, const SireFF::FField&);
QDataStream& operator>>(QDataStream&, SireFF::FField&);

namespace SireFF
{

/**
This class holds a dynamic shared pointer to an FFWorker, and provides the "worker" interface for the forcefield classes that is used by the internal code.

This class is called FField as it provides the 'little' interface to ForceField.

@author Christopher Woods
*/
class SIREFF_EXPORT FField
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

    bool operator==(const FField &other) const;
    bool operator!=(const FField &other) const;

    const char* what() const;
    
    const QString& name() const;
    
    void move(const Molecule &molecule);
    void move(const Residue &residue);
    
    void change(const Molecule &molecule, const ParameterTable &newparams);
    void change(const Residue &residue, const ParameterTable &newparams);
    
    void change(const ChangedMols &changedmols);
    void move(const MovedMols &movedmols);
    
    const QHash<Function,QString>& components() const;
    
    double energy();
    double energy(const Function &component);
    Values energies();
    
    const Function& total() const;
    
private:
    /** Dynamic shared pointer to the FFWorker instance */
    SireBase::DynamicSharedPtr<FFWorker> d;
};

/** Return the type of this forcefield */
inline const char* FField::what() const
{
    return d->what();
}

/** Return the name of this forcefield */
inline const QString& FField::name() const
{
    return d->name();
}
        
/** Tell the forcefield that the molecule 'molecule' 
    has moved (and thereby also supplying a copy of the moved
    molecule)
    
    \throw SireError::version_error
*/
inline void FField::move(const Molecule &molecule)
{
    d->move(molecule);
}

/** Tell the forcefield that the residue 'residue' 
    has moved (and thereby also supplying a copy of the
    moved residue)
    
    \throw SireError::version_error
*/
inline void FField::move(const Residue &residue)
{
    d->move(residue);
}
    
/** Tell the forcefield that the molecule 'molecule' has
    changed, and that its parameters are now in 'newparams'.
    
    \throw SireError::incompatible_error
    \throw SireDB::missing_parameter
*/
inline void FField::change(const Molecule &molecule, 
                           const ParameterTable &newparams)
{
    d->change(molecule,newparams);
}

/** Tell the forcefield that the residue 'residue' has
    changed, and that its parameters are now in 'newparams'
    
    \throw SireError::incompatible_error
    \throw SireDB::missing_parameter
*/
inline void FField::change(const Residue &residue, 
                           const ParameterTable &newparams)
{
    d->change(residue,newparams);
}

/** Tell the forcefield about all of the changed molecules
    in 'changedmols'
    
    \throw SireError::version_error
    \throw SireError::incompatible_error
    \throw SireDB::missing_parameter
*/
inline void FField::change(const ChangedMols &changedmols)
{
    d->change(changedmols);
}

/** Tell the forcefield about all of the moved molecules
    in 'movedmols'
    
    \throw SireError::version_error
*/
inline void FField::move(const MovedMols &movedmols)
{
    d->move(movedmols);
}

/** Return the functions that represent the components of this
    forcefield, together with descriptions of what all of the 
    components represent. */
inline const QHash<Function,QString>& FField::components() const
{
    return d->components();
}

/** Return the total energy of this forcefield. This may trigger
    a recalculation of the total energy if the state of this forcefield
    has changed since that last time the energy was evaluated. */
inline double FField::energy()
{
    return d->energy();
}

/** Return the energy of the component represented by the function
    'component' - this may trigger a recalculation of the total
    energy
    
    \throw SireFF::missing_component
*/
inline double FField::energy(const Function &component)
{
    return d->energy(component);
}

/** Return the values of the energy of all of the energy
    components of this forcefield. This includes the component
    representing the total energy of this forcefield */
inline Values FField::energies()
{
    return d->energies();
}

/** Return the function representing the total energy of 
    this forcefield */
inline const Function& FField::total() const
{
    return d->total();
}
    

}

Q_DECLARE_METATYPE(SireFF::FField)

SIRE_END_HEADER

#endif
