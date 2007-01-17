#ifndef SIRESYSTEM_LOCALSIMSYSTEM_H
#define SIRESYSTEM_LOCALSIMSYSTEM_H

#include "simsystem.h"
#include "system.h"

SIRE_BEGIN_HEADER

namespace SireSystem
{

/** This class provides a SimSystem that is used to perform
    the entire simulation in the local process. This class
    is designed to allow the user to quickly run local simulations
    without the hassle of setting up processors/cluster and using
    SireSim

    @author Christopher Woods
*/
class LocalSimSystem : public SystemData, public SimSystem
{
public:
    LocalSimSystem(const System &system, const Moves &moves);

    const System& run(quint32 nmoves);

    double energy(const Function &component);

    System checkpoint();

    SystemID ID();

    Version version();

private:
    /** The moves that will be performed on this system */
    Moves _moves;
};

}

SIRE_END_HEADER

#endif
