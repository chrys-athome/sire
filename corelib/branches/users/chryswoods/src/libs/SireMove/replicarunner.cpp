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

#include <QMutex>

#include "replicarunner.h"
#include "simulation.h"

#include "SireMPI/mpinode.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMove;
using namespace SireSystem;
using namespace SireBase;
using namespace SireStream;

///////////
/////////// Implementation of ReplicaRunner
///////////

static const RegisterMetaType<ReplicaRunner> r_runner( MAGIC_ONLY,
                                                       "SireMove::ReplicaRunner" );
                                                     
/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, 
                                        const ReplicaRunner &runner)
{
    writeHeader(ds, r_runner, 1);
    
    ds << static_cast<const Property&>(runner);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds,
                                        ReplicaRunner &runner)
{
    VersionID v = readHeader(ds, r_runner);
    
    if (v == 1)
    {
        ds >> static_cast<Property&>(runner);
    }
    else
        throw version_error( v, "1", r_runner, CODELOC );
        
    return ds;
}

/** Constructor */
ReplicaRunner::ReplicaRunner() : Property()
{}

/** Copy constructor */
ReplicaRunner::ReplicaRunner(const ReplicaRunner &other)
              : Property(other)
{}
  
/** Destructor */  
ReplicaRunner::~ReplicaRunner()
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
    
    ds << static_cast<const ReplicaRunner&>(basicrunner);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds,
                                        BasicRepRunner &basicrunner)
{
    VersionID v = readHeader(ds, r_basicrunner);
    
    if (v == 1)
    {
        ds >> static_cast<ReplicaRunner&>(basicrunner);
    }
    else
        throw version_error( v, "1", r_basicrunner, CODELOC );

    return ds;
}

/** Constructor */
BasicRepRunner::BasicRepRunner() : ConcreteProperty<BasicRepRunner,ReplicaRunner>()
{}

static SharedPolyPointer<BasicRepRunner> shared_null;

const BasicRepRunner& ReplicaRunner::null()
{
    if (shared_null.constData() == 0)
    {
        QMutexLocker lkr( SireBase::globalLock() );
        
        if (shared_null.constData() == 0)
            shared_null = new BasicRepRunner();
    }
    
    return *(shared_null.constData());
}

/** Copy constructor */
BasicRepRunner::BasicRepRunner(const BasicRepRunner &other)
               : ConcreteProperty<BasicRepRunner,ReplicaRunner>(other)
{}

/** Destructor */
BasicRepRunner::~BasicRepRunner()
{}

/** Copy assignment operator */
BasicRepRunner& BasicRepRunner::operator=(const BasicRepRunner &other)
{
    ReplicaRunner::operator=(other);
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
void BasicRepRunner::run(Replicas &replicas, bool record_stats) const
{
    int nreplicas = replicas.count();
    
    if (nreplicas == 0)
        return;

    boost::shared_ptr<Replicas> old_replicas( replicas.clone() );
    
    try
    {

        //loop over all replicas in the set
        for (int i=0; i<nreplicas; ++i)
        {
            //get a copy of this replica
            const Replica &replica = replicas[i];
        
            //run the simulation
            Simulation sim = Simulation::run(replica.system(), replica.moves(),
                                             replica.nMoves(),
                                             record_stats and replica.recordStatistics());
                                        
            //wait for the simulation to finish
            sim.wait();
        
            //create a new replica, that is the copy of the old, but
            //with the system and moves from the end of the simulation
            replicas.setSystem( i, sim.system() );
            replicas.setMoves( i, sim.moves() );
        }
    }
    catch(...)
    {
        replicas.copy( *old_replicas );
        throw;
    }
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
    ds << static_cast<const ReplicaRunner&>(mpirunner);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds,
                                        MPIRepRunner &mpirunner)
{
    VersionID v = readHeader(ds, r_mpirunner);
    
    if (v == 1)
    {
        ds >> static_cast<ReplicaRunner&>(mpirunner);
        
        //just point the nodes to the global communicator
        mpirunner.nodes = MPINodes();
    }
    else
        throw version_error( v, "1", r_mpirunner, CODELOC );
        
    return ds;
}

/** Construct to use all of the nodes available in the global communicator */
MPIRepRunner::MPIRepRunner() : ConcreteProperty<MPIRepRunner,ReplicaRunner>()
{}

/** Construct to use all of the nodes in the passed MPI communicator */
MPIRepRunner::MPIRepRunner(const MPINodes &communicator)
             : ConcreteProperty<MPIRepRunner,ReplicaRunner>(),
               nodes(communicator)
{}

/** Copy constructor */
MPIRepRunner::MPIRepRunner(const MPIRepRunner &other)
             : ConcreteProperty<MPIRepRunner,ReplicaRunner>(other),
               nodes(other.nodes)
{}

/** Destructor */
MPIRepRunner::~MPIRepRunner()
{}

/** Copy assignment operator */
MPIRepRunner& MPIRepRunner::operator=(const MPIRepRunner &other)
{
    ReplicaRunner::operator=(other);
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
void MPIRepRunner::run(Replicas &replicas, bool record_stats) const
{
    int nreplicas = replicas.count();
    
    if (nreplicas == 0)
        return;

    boost::shared_ptr<Replicas> old_replicas( replicas.clone() );

    try
    {
        //get a writable handle to the list of available MPI nodes
        MPINodes available_nodes = nodes;

        qDebug() << "Number of nodes ==" << available_nodes.count();

        QVector<Simulation> running_sims(nreplicas);
    
        //loop over all replicas in the set and set them all running
        for (int i=0; i<nreplicas; ++i)
        {
            //get a copy of this replica
            const Replica &replica = replicas[i];
        
            qDebug() << "Waiting for a free node...";
            
            //get a free node on which to run the simulation
            MPINode node = available_nodes.getFreeNode();
        
            qDebug() << "Running replica" << i << "on node" << node.rank();
        
            //run the simulation - save the handle
            running_sims[i] = Simulation::run(node, replica.system(), replica.moves(),
                                              replica.nMoves(),
                                           record_stats and replica.recordStatistics());
        }

        //now loop over the running simulations and wait for 
        //them all to finish - then update the new replicas with
        //the new results
        for (int i=0; i<nreplicas; ++i)
        {
            running_sims[i].wait();
        
            replicas.setSystem( i, running_sims[i].system() );
            replicas.setMoves( i, running_sims[i].moves() );
        }
    }
    catch(...)
    {
        replicas.copy( *old_replicas );
        throw;
    }
}
