#ifndef SIRESIM_SIMULATION_H
#define SIRESIM_SIMULATION_H

#include "clustersimsystem.h"

#include <boost/shared_ptr.hpp>

SIRE_BEGIN_HEADER

namespace SireSim
{

class SIRESIM_EXPORT Simulation
{
public:
    Simulation();

    Simulation(const Simulation &other);

    ~Simulation();


    static Simulation run(const Moves &moves,
                          const System &system,
                          const Cluster &cluster,
                          const ClusterMap &map = ClusterMap());

    static Simulation run(const SimSnapshot &snapshot,
                          const Cluster &cluster,
                          const ClusterMap &map = ClusterMap());

    void wait();
    bool wait(unsigned long time);

    void abort();

    SimSnapshot snapshot();

private:
    /** Shared pointer to the implementation of this
        simulation */
    boost::shared_ptr<SimulationPvt> d;
};

}

SIRE_END_HEADER

#endif

