#ifndef SIRESIM_CLUSTERSIMSYSTEM_H
#define SIRESIM_CLUSTERSIMSYSTEM_H

#include "SireSystem/simsystem.h"

SIRE_BEGIN_HEADER

namespace SireSim
{

using SireSystem::SimSystem;

class SIRESIM_EXPORT ClusterSimSystem : public SimSystem
{
public:
    ClusterSimSystem(const System &system,
                     const Cluster &cluster,
                     const ClusterMap &map = ClusterMap());

    ~ClusterSimSystem();

    double energy(const Function &component);

    System checkpoint();

    void rollback(const System &system);

    MoleculeGroup group(const MoleculeGroup &group);

    QHash<MolGroupID,MoleculeGroup> groups();

private:
    /** The forcefields calculated in the main thread of the
        cluster, indexed by forcefield ID */
    QHash<ForceFieldID, ForceField> localff;

    /** The processors in the cluster running forcefields,
        indexed by ForceFieldID */
    QHash<ForceFieldID, Processor> remoteff;

    /** The metadata about the system (groups, monitors,
        properties, version etc.) */
    SystemData sysdata;

    /** The cluster that is being used to run this simulation */
    Cluster clstr;

};

}

SIRE_END_HEADER

#endif
