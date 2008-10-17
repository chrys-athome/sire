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

#include "replicarunner.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMove;
using namespace SireSystem;
using namespace SireBase;
using namespace SireStream;

///////////
/////////// Implementation of ReplicaRunnerBase
///////////

static const RegisterMetaType<ReplicaRunnerBase> r_runnerbase( MAGIC_ONLY,
                                                     "SireMove::ReplicaRunnerBase" );
                                                     
/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, 
                                        const ReplicaRunnerBase &runnerbase)
{
    writeHeader(ds, r_runnerbase, 1);
    
    ds << static_cast<const PropertyBase&>(runnerbase);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds,
                                        ReplicaRunnerBase &runnerbase)
{
    VersionID v = readHeader(ds, r_runnerbase);
    
    if (v == 1)
    {
        ds >> static_cast<PropertyBase&>(runnerbase);
    }
    else
        throw version_error( v, "1", r_runnerbase, CODELOC );
        
    return ds;
}

/** Constructor */
ReplicaRunnerBase::ReplicaRunnerBase() : PropertyBase()
{}

/** Copy constructor */
ReplicaRunnerBase::ReplicaRunnerBase(const ReplicaRunnerBase &other)
                  : PropertyBase(other)
{}
  
/** Destructor */  
ReplicaRunnerBase::~ReplicaRunnerBase()
{}

///////////
/////////// Implementation of BasicRepRunner
///////////

static const RegisterMetaType<BasicRepRunner> r_basicrunner;

/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, 
                                        const BasicRepRunner &basicrunner)
{
    writeHeader(ds, r_basicrunner, 1);
    
    ds << static_cast<const ReplicaRunnerBase&>(basicrunner);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds,
                                        BasicRepRunner &basicrunner)
{
    VersionID v = readHeader(ds, r_basicrunner);
    
    if (v == 1)
    {
        ds >> static_cast<ReplicaRunnerBase&>(basicrunner);
    }
    else
        throw version_error( v, "1", r_basicrunner, CODELOC );

    return ds;
}

/** Constructor */
BasicRepRunner::BasicRepRunner() : ConcreteProperty<BasicRepRunner,ReplicaRunnerBase>()
{}

/** Copy constructor */
BasicRepRunner::BasicRepRunner(const BasicRepRunner &other)
               : ConcreteProperty<BasicRepRunner,ReplicaRunnerBase>(other)
{}

/** Destructor */
BasicRepRunner::~BasicRepRunner()
{}

/** Copy assignment operator */
BasicRepRunner& BasicRepRunner::operator=(const BasicRepRunner &other)
{
    ReplicaRunnerBase::operator=(other);
    return *this;
}

/** Comparison operator */
bool BasicRepRunner::operator==(const BasicRepRunner &other) const
{
    return true;
}

/** Comparison operator */
bool BasicRepRunner::operator!=(const BasicRepRunner &other) const
{
    return false;
}

/** Perform a simulation run on each of the replicas in 'replicas', optionally
    recording statistics if 'record_stats' is true. This returns a copy
    of the replicas after the sampling is complete. This runs each of the 
    simulations sequentially in the current thread. */
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

static const RegisterMetaType<MPIRepRunner> r_mpirunner;

/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, 
                                        const MPIRepRunner &mpirunner)
{
    writeHeader(ds, r_mpirunner, 1);
    
    //can't save the actual nodes, as we don't know if they will be
    //available when (and where!) we are next loaded
    ds << static_cast<const ReplicaRunnerBase&>(mpirunner);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds,
                                        MPIRepRunner &mpirunner)
{
    VersionID v = readHeader(ds, r_mpirunner);
    
    if (v == 1)
    {
        ds >> static_cast<ReplicaRunnerBase&>(mpirunner);
        
        //just point the nodes to the global communicator
        mpirunner.nodes = MPINodes();
    }
    else
        throw version_error( v, "1", r_mpirunner, CODELOC );
        
    return ds;
}

/** Construct to use all of the nodes available in the global communicator */
MPIRepRunner::MPIRepRunner() : ConcreteProperty<MPIRepRunner,ReplicaRunnerBase>()
{}

/** Construct to use all of the nodes in the passed MPI communicator */
MPIRepRunner::MPIRepRunner(const MPINodes &communicator)
             : ConcreteProperty<MPIRepRunner,ReplicaRunnerBase>(),
               nodes(communicator)
{}

/** Copy constructor */
MPIRepRunner::MPIRepRunner(const MPIRepRunner &other)
             : ConcreteProperty<MPIRepRunner,ReplicaRunnerBase>(other),
               nodes(other.nodes)
{}

/** Destructor */
MPIRepRunner::~MPIRepRunner()
{}

/** Copy assignment operator */
MPIRepRunner& MPIRepRunner::operator=(const MPIRepRunner &other)
{
    ReplicaRunnerBase::operator=(other);
    nodes = other.nodes;
    return *this;
}

/** Comparison operator */
bool MPIRepRunner::operator==(const MPIRepRunner &other) const
{
    return nodes == other.nodes;
}

/** Comparison operator */
bool MPIRepRunner::operator!=(const MPIRepRunner &other) const
{
    return nodes != other.nodes;
}

/** Perform a simulation run on each of the replicas in 'replicas', optionally
    recording statistics if 'record_stats' is true. This returns a copy
    of the replicas after the sampling is complete. This runs each of the 
    simulations on the MPI nodes associated with this runner */
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
