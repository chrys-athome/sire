#ifndef SIRESYSTEM_SIMSYSTEM_H
#define SIRESYSTEM_SIMSYSTEM_H

#include "system.h"

#include <boost/noncopyable.hpp>

namespace SireSystem
{

/** This pure-virtual class provides the interface to a simulation
    system that is being simulated. This allows the
    System to be simulated both in the local process
    and also on a cluster.

    While System is an implicitly-shared class,
    SimSystem is designed to be a non-copyable
    class (as a Simulation is occuring only once).

    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT SimSystem : public boost::noncopyable
{
public:
    SimSystem();

    virtual ~SimSystem();

    virtual double energy(const Function &component)=0;

    virtual System checkpoint()=0;

    virtual void rollback(const System &checkpoint)=0;

    virtual MoleculeGroup group(MolGroupID id)=0;

    virtual QHash<MoleculeGroupID,MoleculeGroups> groups()=0;
    
    virtual SystemID ID()=0;
    
    virtual Version version()=0;
};

/** This class provides a SimSystem that is used to perform
    the entire simulation in the local process. This class
    is designed to allow the user to quickly run local simulations
    without the hassle of setting up processors/cluster and using
    SireSim

    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT LocalSimSystem : public SimSystem
{
public:
    LocalSimSystem(const System &system, const Moves &moves);

    const System& run(quint32 nmoves);

    double energy(const Function &component);

    System checkpoint();

    SystemID ID();
    
    Version version();

private:
    /** The system that is being simulated */
    System _system;

    /** The moves that will be performed on this system */
    Moves _moves;
};

}

#endif
