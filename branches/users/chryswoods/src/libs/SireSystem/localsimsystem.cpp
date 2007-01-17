
#include "localsimsystem.h"

using namespace SireSystem;

/** Construct a simulation that performs the moves 'moves' on
    the system 'system' */
LocalSimSystem::LocalSimSystem(const System &system, const Moves &moves)
               : SimSystem(),
                 _system(system), _moves(moves)
{
    //initialise the moves with the system - this ensures
    //that the moves are compatible with the system, thus
    //throwing any exceptions now, rather than after a weeks
    //fruitless simulation ;-)
    _moves.initialise(_system);
}

/** Destructor */
LocalSimSystem::~LocalSimSystem()
{}

/** Return the energy of the component 'component'
    of the system */
double LocalSimSystem::energy(const Function &component)
{
    return _system.energy(component);
}

/** Return a checkpoint of this System */
System LocalSimSystem::checkpoint()
{
    return _system;
}

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
    if (_system.ID() != checkpoint.ID())
        throw SireError::incompatible_error( QObject::tr(
            "Cannot roll back to a different simulation system! "
            "(current == \"%1\" (%2), checkpoint == \"%3\" (%4))")
                .arg(_system.name()).arg(_system.ID())
                .arg(checkpoint.name()).arg(checkpoint.ID()),
                    CODELOC );

    //the checkpoint must have a lower or equal version number
    //to the current system, or that implies that the
    //checkpoint has been modified outside of this
    //simulation
    else if (_system.version() < checkpoint.version())
        throw SireError::version_error( QObject::tr(
            "The version number of the checkpoint (%1) is "
            "greater than that of the simulation (%2)! "
            "This implies that the checkpoint has been "
            "modified outside of this simulation.")
                .arg(checkpoint.version().toString(),
                     _system.version().toString()), CODELOC );

    _system = checkpoint;
}

/** Run 'nmoves' moves on the system. Return a reference to the
    resulting system after the moves. */
const System& LocalSimSystem::run(quint32 nmoves)
{
    _moves.move(system, nmoves);

    return _system;
}

/** Return the ID number of the System */
SystemID LocalSimSystem::ID()
{
    return _system.ID();
}

/** Return the version number of the System */
Version LocalSimSystem::version()
{
    return _system.version();
}
