/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2008  Christopher Woods
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

///////////
/////////// Implementation of BasicRepRunner
///////////

Replicas BasicRepRunner::run(const Replicas &replicas, bool record_stats) const
{
    int nreplicas = replicas->count();
    
    if (nreplicas == 0)
        return replicas;

    Replicas new_replicas( replicas );

    //loop over all replicas in the set
    for (int i=0; i<nreplicas; ++i)
    {
        //get a copy of this replica
        Replica replica = new_replicas[i];
        
        //run the simulation
        Simulation sim = Simulation.run(replica.system(), replica.moves(),
                                        replica.nMoves(),
                                        record_stats and replica.recordStatistics());
                                        
        //wait for the simulation to finish
        sim.wait();
        
        //create a new replica, that is the copy of the old, but
        //with the system and moves from the end of the simulation
        new_replicas.setSystem( i, sim.system() );
        new_replicas.setMoves( i, sim.moves() );
    }

    return new_replicas;
}

///////////
/////////// Implementation of MPIRepRunner
///////////

Replicas MPIRepRunner::run(const Replicas &replicas, bool record_stats) const
{
    int nreplicas = replicas.count();
    
    if (nreplicas == 0)
        return;

    //get a writable handle to the list of available MPI nodes
    MPINodes available_nodes = nodes;

    QVector<Simulation> running_sims(nreplicas);
    
    //loop over all replicas in the set and set them all running
    for (int i=0; i<nreplicas; ++i)
    {
        //get a copy of this replica
        Replica replica = replicas[i];
        
        //get a free node on which to run the simulation
        MPINode node = available_nodes.getFreeNode();
        
        //run the simulation - save the handle
        running_sims[i] = Simulation::run(node, replica.system(), replica.moves(),
                                          replica.nMoves(),
                                          record_stats and replica.recordStatistics());
    }

    //now loop over the running simulations and wait for 
    //them all to finish - then update the new replicas with
    //the new results
    Replicas new_replicas(nreplicas);

    for (int i=0; i<nreplicas; ++i)
    {
        running_sims[i].wait();
        
        new_replicas.setSystem( i, running_sims[i].system() );
        new_replicas.setMoves( i, running_sims[i].moves() );
    }
    
    return new_replicas;
}
