
#include "simsystem.h"
#include "system.h"
#include "moleculegroup.h"
#include "moves.h"
#include "move.h"

#include "SireFF/forcefield.h"

using namespace SireSystem;
using namespace SireMol;
using namespace SireFF;

//////////
////////// Implementation of SimSystem
//////////

/** Constructor */
SimSystem::SimSystem() : boost::noncopyable()
{}

/** Destructor */
SimSystem::~SimSystem()
{}

/** Internal function used by classes derived from SimSystem to 
    construct a System object from SystemData and a set of forcefields,
    without calling any code to check that the forcefields are compatible
    with the SystemData - you must use this function responsibly! */
System SimSystem::makeSystem(const SystemData &data,
                             const QHash<ForceFieldID,ForceField> &ffields)
{
    return System(data, ffields);
}
