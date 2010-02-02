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

#include "cluster.h"
#include "resourcemanager.h"
#include "node.h"
#include "nodes.h"
#include "frontend.h"
#include "backend.h"
#include "workqueue.h"

#include "Siren/mutex.h"
#include "Siren/waitcondition.h"
#include "Siren/forages.h"
#include "Siren/errors.h"

#ifdef SIRE_USE_MPI
    #include "SireCluster/mpi/mpicluster.h" // CONDITIONAL_INCLUDE
#endif

#include <QDebug>

using namespace SireCluster;
using namespace Siren;

class ClusterData
{
public:
    ClusterData();
    ~ClusterData();
    
    /** Mutex to protect access to the data of the cluster */
    Mutex datamutex;
    
    /** Waiter used to wait until the cluster is shut down */
    WaitCondition run_waiter;
    
    /** Mutex to be used with run_waiter (mutex must not be recursive) */
    Mutex run_waiter_mutex;
    
    /** The UIDs of all known local resources */
    QSet<QUuid> local_resources;
    
    /** The UIDs of all known MPI resources */
    QSet<QUuid> mpi_resources;
    
    /** The version number of the known local resources 
        (this changes if resources are added or removed) */
    int local_resource_version;
    
    /** The version number of known mpi resources
        (this changes if resources are added or removed) */
    int mpi_resource_version;
    
    /** Whether or not the cluster is currently running */
    bool cluster_is_running;
};

Q_GLOBAL_STATIC( ClusterData, clusterData );

/** Constructor for the global cluster */
ClusterData::ClusterData() 
            : datamutex( QMutex::Recursive ),
              local_resource_version(0), 
              mpi_resource_version(0),
              cluster_is_running(false)
{}

/** Destructor */
ClusterData::~ClusterData()
{}

/** Start the cluster - you need to call this function once at the
    start of your program to initialise the cluster */
void Cluster::start()
{
    ClusterData *data = clusterData();

    MutexLocker lkr( &(data->datamutex) );
    
    if (data->cluster_is_running)
        //this has already been started
        return;
    
    // start the different means of communication with
    // other nodes
    #ifdef SIRE_USE_MPI
        SireCluster::MPI::MPICluster::start();
    #endif
    
    data->cluster_is_running = true;
}

/** Return whether or not the cluster is running */
bool Cluster::isRunning()
{
    MutexLocker lkr( &(clusterData()->datamutex) );
    return clusterData()->cluster_is_running;
}

/** Wait for the global cluster to stop running */
void Cluster::wait()
{
    MutexLocker lkr( &(clusterData()->datamutex) );
    
    if (clusterData()->cluster_is_running)
    {
        MutexLocker lkr2( &(clusterData()->run_waiter_mutex) );
        lkr.unlock();
        clusterData()->run_waiter.wait( &(clusterData()->run_waiter_mutex) );
    }
}

/** Shutdown this cluster */
void Cluster::shutdown()
{
    MutexLocker lkr( &(clusterData()->datamutex) );
    
    if (not clusterData()->cluster_is_running)
        return;

    #ifdef SIRE_USE_MPI
        SireCluster::MPI::MPICluster::shutdown();
    #endif

    clusterData()->cluster_is_running = false;
    
    MutexLocker lkr2( &(clusterData()->run_waiter_mutex) );

    //wake all threads waiting for the cluster to be shutdown
    clusterData()->run_waiter.wakeAll();
}

/** Return the UIDs of local resources */
QList<QUuid> Cluster::localUIDs()
{
    MutexLocker lkr( &(clusterData()->datamutex) );
    
    int version = ResourceManager::resourceListVersion();
    
    if (clusterData()->local_resource_version != version)
    {
        QHash<QUuid,QString> resources = ResourceManager::availableResources();
        clusterData()->local_resources = resources.keys().toSet();
        clusterData()->local_resource_version = version;
    }

    return clusterData()->local_resources.toList();
}

/** Return the UIDs of all resources */
QList<QUuid> Cluster::UIDs()
{
    MutexLocker lkr( &(clusterData()->datamutex) );

    QList<QUuid> resources;
    
    #ifdef SIRE_USE_MPI
    {
        int version = SireCluster::MPI::MPICluster::resourceListVersion()
    
        if (clusterData()->mpi_resource_version != version)
        {
            QHash<QUuid,QString> resources = 
                SireCluster::MPI::MPICluster::availableResources();
            
            QList<QUuid> mpi_resources = resources.keys();
        
            QList<QUuid> local_resources = Cluster::localUIDs();
        
            foreach (QUuid local_resource, local_resources)
            {
                mpi_resources.removeAll(local_resource);
            }
        
            clusterData()->mpi_resources = mpi_resources;
            clusterData()->mpi_resource_version = version;
        }
        
        resources += clusterData()->mpi_resources;
    }
    #endif // SIRE_USE_MPI
    
    return clusterData()->local_resources.toList() + resources;
}

/** Return the descriptions of the local resources in the cluster */
QHash<QUuid,QString> Cluster::localDescriptions()
{
    return ResourceManager::availableResources();
}

/** Return the descriptions of all of the resources available in the cluster */
QHash<QUuid,QString> Cluster::descriptions()
{
    QHash<QUuid,QString> descs = ResourceManager::availableResources();

    #ifdef SIRE_USE_MPI
    {
        QHash<QUuid,QString> mpi = SireCluster::MPI::MPICluster::availableResources();
    
        for (QHash<QUuid,QString>::const_iterator it = mpi.constBegin();
             it != mpi.constEnd();
             ++it)
        {
            if (not descs.contains(it.key()))
                descs.insert(it.key(), it.value());
        }
    }
    #endif // SIRE_USE_MPI

    return descs;
}

/** Return whether or not the resource identified with UID 'uid'
    is local to this process */
bool Cluster::isLocal(const QUuid &uid)
{
    return Cluster::localUIDs().contains(uid);
}

/** Return any local node - this blocks until a local node is available */
Node Cluster::getLocalNode()
{
    while (for_ages())
    {
        QUuid reservation = ResourceManager::reserveResource();
    
        if (not reservation.isNull())
        {
            ActiveBackend resource = ResourceManager::collectReservation(reservation);
            
            if (not resource.isNull())
            {
                return Node( SimpleQueue().create( 
                                DormantFrontend(new LocalFrontend(resource)) ) );
            }
        }
    }

    return Node();
}

/** Return any local node, but only waiting up to 'ms' milliseconds.
    If no local node is available, then this returns an empty (local-thread) Node */
Node Cluster::getLocalNode(int ms)
{
    QTime t;
    t.start();
    
    while (for_ages())
    {
        QUuid reservation = ResourceManager::reserveResource(ms);
        
        if (not reservation.isNull())
        {
            ActiveBackend resource = ResourceManager::collectReservation(reservation);
            
            if (not resource.isNull())
            {
                return Node( SimpleQueue().create(
                                DormantFrontend(new LocalFrontend(resource)) ) );
            }
        }
        
        ms -= t.restart();
        
        if (ms <= 0)
            break;
    }
    
    return Node();
}
