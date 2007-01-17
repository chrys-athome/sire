
#include "localsimsystem.h"

#include "SireMol/property.h"

#include "SireError/errors.h"

using namespace SireSystem;
using namespace SireMol;

/** Construct a simulation that performs the moves 'moves' on
    the system 'system' */
LocalSimSystem::LocalSimSystem(const System &system, const Moves &moves)
               : SimSystem(), SystemData(system),
                 mvs(moves)
{
    //initialise the moves with the system - this ensures
    //that the moves are compatible with the system, thus
    //throwing any exceptions now, rather than after a weeks
    //fruitless simulation ;-)
    mvs.initialise(*this);
}

/** Destructor */
LocalSimSystem::~LocalSimSystem()
{}

/** Rollback to the supplied checkpoint. The checkpoint
    must have a lower version number than the current
    system, or else that implies that the checkpoint
    has been modified outside of the simulation.

    \throw SireError::incompatible_error
    \throw SireError::version_error
*/
void LocalSimSystem::rollback(const System &checkpoint)
{
    //the ID numbers must match - we cannot change to
    //a new system during a running simulation!
    if (this->ID() != checkpoint.ID())
        throw SireError::incompatible_error( QObject::tr(
            "Cannot roll back to a different simulation system! "
            "(current == \"%1\" (%2), checkpoint == \"%3\" (%4))")
                .arg(this->name()).arg(this->ID())
                .arg(checkpoint.name()).arg(checkpoint.ID()),
                    CODELOC );

    //the checkpoint must have a lower or equal version number
    //to the current system, or that implies that the
    //checkpoint has been modified outside of this
    //simulation
    else if (this->version() < checkpoint.version())
        throw SireError::version_error( QObject::tr(
            "The version number of the checkpoint (%1) is "
            "greater than that of the simulation (%2)! "
            "This implies that the checkpoint has been "
            "modified outside of this simulation.")
                .arg(checkpoint.version().toString(),
                     this->version().toString()), CODELOC );

    SystemData::operator=(checkpoint);
    ffields = checkpoint.forceFields();
}

/** Run 'nmoves' moves on the system. Return a copy
    of the System after the moves. */
System LocalSimSystem::run(quint32 nmoves)
{
    mvs.run(*this, nmoves);

    return this->makeSystem(*this, ffields);
}

/** Change the molecule 'molecule' in this System */
void LocalSimSystem::change(const Molecule &molecule)
{
    //should I make sure that this maintains the invariant?
    
    //change the molecule in all forcefields
    for (QHash<ForceFieldID,ForceField>::iterator it = ffields.begin();
         it != ffields.end();
         ++it)
    {
        it->change(molecule);
    }
         
    //now change the molecule in the SystemData
    SystemData::change(molecule);
}

/** Change the residue 'residue' in this System */
void LocalSimSystem::change(const Residue &residue)
{
    //should I make sure that this maintains the invariant?
    
    //change the residue in all forcefields
    for (QHash<ForceFieldID,ForceField>::iterator it = ffields.begin();
         it != ffields.end();
         ++it)
    {
        it->change(residue);
    }
         
    //now change the residue in the SystemData
    SystemData::change(residue);
}

/** Change the atom 'atom' in this System */
void LocalSimSystem::change(const NewAtom &atom)
{
    //should I make sure that this maintains the invariant?
    
    //change the atom in all forcefields
    for (QHash<ForceFieldID,ForceField>::iterator it = ffields.begin();
         it != ffields.end();
         ++it)
    {
        it->change(atom);
    }
         
    //now change the atom in the SystemData
    SystemData::change(atom);
}
