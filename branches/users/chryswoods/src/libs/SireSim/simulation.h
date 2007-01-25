/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
  *
  *  This program is free software; you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation; either version 2 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program; if not, write to the Free Software
  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  *  For full details of the license please see the COPYING file
  *  that should have come with this distribution.
  *
  *  You can contact the authors via the developer's mailing list
  *  at http://siremol.org
  *
\*********************************************/

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

