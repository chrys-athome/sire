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
#include "node.h"
#include "nodes.h"

#include "resources/resourcemanager.h" // CONDITIONAL_INCLUDE
#include "resources/frontend.h"        // CONDITIONAL_INCLUDE
#include "resources/backend.h"         // CONDITIONAL_INCLUDE
#include "resources/workqueue.h"       // CONDITIONAL_INCLUDE

#include "network/communicator.h"       // CONDITIONAL_INCLUDE
#include "network/message.h"            // CONDITIONAL_INCLUDE
#include "network/netresourcemanager.h" // CONDITIONAL_INCLUDE
#include "network/nodemessages.h"       // CONDITIONAL_INCLUDE

#include "Siren/mutex.h"
#include "Siren/waitcondition.h"
#include "Siren/forages.h"
#include "Siren/errors.h"

#ifdef SIRE_USE_MPI
    #include "SireCluster/mpi/mpicluster.h" // CONDITIONAL_INCLUDE
#endif

#ifdef Q_OS_UNIX
    #include <unistd.h>
#endif

#include <QDebug>

using namespace SireCluster;
using namespace SireCluster::resources;
using namespace SireCluster::network;
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
    
    /** Whether or not the cluster is currently running */
    bool cluster_is_running;
};

Q_GLOBAL_STATIC( ClusterData, clusterData );

/** Constructor for the global cluster */
ClusterData::ClusterData() 
            : datamutex( QMutex::Recursive ),
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
    
    ResourceManager::init();
    
    // start the different means of communication with
    // other nodes
    #ifdef SIRE_USE_MPI
        Communicator::init();
        SireCluster::MPI::MPICluster::start(0,0);
        
        if (SireCluster::MPI::MPICluster::count() > 1)
        {
            NetResourceManager::init();
        }
        else
        {
            //no further need for the communicator
            Communicator::end();
        }
    #endif
    
    data->cluster_is_running = true;
}

/** Return whether or not this is the initial process 
    (and should be used to initialise jobs) */
bool Cluster::isInitProcess()
{
    Cluster::start();

    #ifdef SIRE_USE_MPI
        return SireCluster::MPI::MPICluster::rank() == 0;
    #else
        return true;
    #endif
}

/** Return whether this cluster is a single local process only */
bool Cluster::isLocalOnly()
{
    Cluster::start();

    #ifdef SIRE_USE_MPI
        return SireCluster::MPI::MPICluster::count() <= 1;
    #else
        return true;
    #endif
}

/** Return the hostname of the host running this process */
QString Cluster::hostName()
{
    Cluster::start();

    #ifdef SIRE_USE_MPI
        return SireCluster::MPI::MPICluster::hostName();

    #else
        #ifdef  Q_OS_UNIX
            QByteArray buffer;
            buffer.resize(256);
    
            ::gethostname(buffer.data(), buffer.length());
    
            return QLatin1String(buffer.constData());
        #else
            return "localhost";
        #endif
    #endif
}

/** Return whether or not the cluster is running */
bool Cluster::isRunning()
{
    MutexLocker lkr( &(clusterData()->datamutex) );
    return clusterData()->cluster_is_running;
}

/** Add a new ThreadBackend to the current process */
void Cluster::addThread()
{
    Cluster::start();
    ResourceManager::registerResource( DormantBackend( new ThreadBackend() ) );
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

/** Shutdown this cluster (on this process only) */
void Cluster::shutdown()
{
    MutexLocker lkr( &(clusterData()->datamutex) );
    
    if (not clusterData()->cluster_is_running)
        return;

    #ifdef SIRE_USE_MPI
        if (SireCluster::MPI::MPICluster::count() > 1)
        {
            Communicator::end();
            NetResourceManager::end();
        }
            
        SireCluster::MPI::MPICluster::shutdown();
    #endif

    ResourceManager::end();

    clusterData()->cluster_is_running = false;
    
    MutexLocker lkr2( &(clusterData()->run_waiter_mutex) );

    //wake all threads waiting for the cluster to be shutdown
    clusterData()->run_waiter.wakeAll();
}

/** Tell the entire cluster to shutdown */
void Cluster::shutdownCluster()
{
    MutexLocker lkr( &(clusterData()->datamutex) );
    
    if (not clusterData()->cluster_is_running)
        return;

    #ifdef SIRE_USE_MPI
    if (SireCluster::MPI::MPICluster::count() > 1)
    {
        //broadcast a shutdown message
        QHash<QUuid,quint64> messages = Communicator::broadcast( network::Shutdown() );

        //wait for all of the shutdown messages to be acknowledged
        Communicator::awaitSent(messages);
    }
    #endif
    
    //now shutdown ourself

    #ifdef SIRE_USE_MPI
        if (SireCluster::MPI::MPICluster::count() > 1)
            Communicator::end();
            
        SireCluster::MPI::MPICluster::shutdown();
    #endif

    ResourceManager::end();

    clusterData()->cluster_is_running = false;
    
    MutexLocker lkr2( &(clusterData()->run_waiter_mutex) );

    //wake all threads waiting for the cluster to be shutdown
    clusterData()->run_waiter.wakeAll();
}

/** Internal function used to get the node associated with the passed reservation */
Node Cluster::getReservedLocalNode(const QUuid &reservation)
{
    if (reservation.isNull())
        return Node();

    Cluster::start();

    ActiveBackend resource = ResourceManager::collectReservation(reservation);
    
    if (not resource.isNull())
    {
        return Node( SimpleQueue().create(
                        DormantFrontend(new LocalFrontend(resource)) ) );
    }
    else
        return Node();
}

/** Internal function used to get the node associated with the passed reservations */
Nodes Cluster::getReservedLocalNodes(const QList<QUuid> &reservations)
{
    if (reservations.isEmpty())
        return Nodes();

    Cluster::start();
        
    QHash<QUuid,ActiveBackend> resources 
                     = ResourceManager::collectReservation(reservations);
                     
    if (resources.isEmpty())
        return Nodes();
        
    else
    {
        QList<DormantFrontend> frontends;
        
        foreach (ActiveBackend resource, resources)
        {
            frontends.append( DormantFrontend(new LocalFrontend(resource)) );
        }
    
        return Nodes( SimpleQueue().create(frontends) );
    }
}

/** Internal function used to get the node associated with the passed reservation */
Node Cluster::getReservedRemoteNode(const QPair<QUuid,QUuid> &reservation)
{
    if (reservation.first.isNull())
        return Node();

    DormantFrontend frontend = NetResourceManager::collectReservation(reservation);
    
    if (frontend.isNull())
        return Node();
    
    else
        return Node( SimpleQueue().create(frontend) );
}

/** Internal function used to get the node associated with the passed reservations */
Nodes Cluster::getReservedRemoteNodes(const QMultiHash<QUuid,QUuid> &reservations)
{
    if (reservations.isEmpty())
        return Nodes();
        
    return Nodes();
}

/** Return any local node - this returns a null Node if no node is available */
Node Cluster::getLocalNode()
{
    Cluster::start();

    QUuid reservation = ResourceManager::tryReserveResource(0);

    Node node = Cluster::getReservedLocalNode(reservation);

    return node;
}

/** Return any local node, but only waiting up to 'ms' milliseconds.
    If no local node is available, then this returns an empty (local-thread) Node */
Node Cluster::getLocalNode(int ms)
{
    Cluster::start();

    QTime t;
    t.start();
    
    while (for_ages())
    {
        QUuid reservation = ResourceManager::tryReserveResource(ms);
        
        Node node = Cluster::getReservedLocalNode(reservation);
        
        if (not node.isNull())
            return node;
        
        ms -= t.restart();
        
        if (ms <= 0)
            break;
    }
    
    return Node();
}

/** Return any node - this returns a null Node if no node is available */
Node Cluster::getNode()
{
    if (isLocalOnly())
        return getLocalNode();
    
    while (for_ages())
    {
        //always try to get a local node first...
        QUuid reservation = ResourceManager::tryReserveResource(0);
    
        if (not reservation.isNull())
        {
            Node node = Cluster::getReservedLocalNode(reservation);
    
            if (not node.isNull())
                return node;
        }
    
        //no local node available - try to get a remote node
        QPair<QUuid,QUuid> remote_res = NetResourceManager::reserveResource();

        if (not remote_res.first.isNull())
        {
            Node node = Cluster::getReservedRemoteNode(remote_res);
            
            if (not node.isNull())
                return node;
        }
        else
            //no nodes available (we couldn't reserve a node)
            return Node();
    }
    
    return Node();
}

/** Return any node - keep trying for up to 'timeout' milliseconds.
    This returns a null Node if no node is available */
Node Cluster::getNode(int timeout)
{
    if (isLocalOnly())
        return getLocalNode(timeout);

    return getLocalNode(timeout);
}

Node Cluster::getLocalNode(const QString &description)
{
    Cluster::start();

    QUuid reservation = ResourceManager::tryReserveResource(0, description);

    Node node = Cluster::getReservedLocalNode(reservation);
        
    return node;
}

Node Cluster::getLocalNode(const QString &description, int ms)
{
    Cluster::start();

    QTime t;
    t.start();
    
    while (for_ages())
    {
        QUuid reservation = ResourceManager::tryReserveResource(ms, description);
        
        Node node = Cluster::getReservedLocalNode(reservation);
        
        if (not node.isNull())
            return node;
        
        ms -= t.restart();
        
        if (ms <= 0)
            break;
    }
    
    return Node();
}

Node Cluster::getNode(const QString &description)
{
    if (isLocalOnly())
        return getLocalNode(description);

    return getLocalNode(description);
}

Node Cluster::getNode(const QString &description, int timeout)
{
    if (isLocalOnly())
        return getLocalNode(description, timeout);

    return getLocalNode(description, timeout);
}

Nodes Cluster::getLocalNodes(int nnodes)
{
    Cluster::start();

    QList<QUuid> reservations = ResourceManager::tryReserveResources(nnodes, 0);

    Nodes nodes = Cluster::getReservedLocalNodes(reservations);
        
    return nodes;
}

Nodes Cluster::getLocalNodes(int nnodes, int ms)
{
    Cluster::start();

    QTime t;
    t.start();
    
    Nodes nodes;
    
    while (for_ages())
    {
        QList<QUuid> reservations = ResourceManager::tryReserveResources(nnodes, ms);
        
        Nodes new_nodes = Cluster::getReservedLocalNodes(reservations);
        
        nnodes -= new_nodes.count();
        
        nodes = Nodes::merge(nodes, new_nodes);
        
        ms -= t.restart();
        
        if (ms <= 0 or nnodes <= 0)
            break;
    }
    
    return nodes;
}

Nodes Cluster::getNodes(int nnodes)
{
    if (isLocalOnly())
        return getLocalNodes(nnodes);

    return getLocalNodes(nnodes);
}

Nodes Cluster::getNodes(int nnodes, int timeout)
{
    if (isLocalOnly())
        return getLocalNodes(nnodes, timeout);

    return getLocalNodes(nnodes, timeout);
}

Nodes Cluster::getLocalNodes(const QString &description, int nnodes)
{
    Cluster::start();

    QList<QUuid> reservations = ResourceManager::tryReserveResources(description,
                                                                     nnodes, 0);

    Nodes nodes = Cluster::getReservedLocalNodes(reservations);
        
    return nodes;
}

Nodes Cluster::getLocalNodes(const QString &description, int nnodes, int ms)
{
    Cluster::start();

    QTime t;
    t.start();
    
    Nodes nodes;
    
    while (for_ages())
    {
        QList<QUuid> reservations = ResourceManager::tryReserveResources(description,
                                                                         nnodes, 
                                                                         ms);
        
        Nodes new_nodes = Cluster::getReservedLocalNodes(reservations);
        
        nnodes -= new_nodes.count();
        
        nodes = Nodes::merge(nodes, new_nodes);
        
        ms -= t.restart();
        
        if (ms <= 0 or nnodes <= 0)
            break;
    }
    
    return nodes;
}

Nodes Cluster::getNodes(const QString &description, int nnodes)
{
    if (isLocalOnly())
        return getLocalNodes(description, nnodes);

    return getLocalNodes(description, nnodes);
}

Nodes Cluster::getNodes(const QString &description, int nnodes, int timeout)
{
    if (isLocalOnly())
        return getLocalNodes(description, nnodes, timeout);

    return getLocalNodes(description, nnodes, timeout);
}
