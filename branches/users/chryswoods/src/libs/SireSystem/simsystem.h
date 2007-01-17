#ifndef SIRESYSTEM_SIMSYSTEM_H
#define SIRESYSTEM_SIMSYSTEM_H

#include <boost/noncopyable.hpp>

namespace SireCAS
{
class Function;
}

namespace SireFF
{
class FFComponent;
}

namespace SireSystem
{

class System;
class SystemID;

using SireCAS::Function;
using SireFF::FFComponent;

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
    virtual double energy(const FFComponent &component)=0;

    virtual System checkpoint()=0;

    virtual void rollback(const System &checkpoint)=0;

    virtual MoleculeGroup group(MolGroupID id)=0;

    virtual QHash<MoleculeGroupID,MoleculeGroups> groups()=0;

    virtual SystemID ID()=0;

    virtual Version version()=0;

protected:
    static System makeSystem(const SystemData &data,
                             const QHash<ForceFieldID,ForceField> &ffields);
};

}

#endif
