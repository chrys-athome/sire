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

#ifdef __SIRE_USE_MPI__
#include <mpi.h>             // CONDITIONAL_INCLUDE
#include "mpi/mpicluster.h"  // CONDITIONAL_INCLUDE
#endif

#include <QHash>
#include <QMutex>
#include <QWaitCondition>

#include "cluster.h"
#include "backend.h"
#include "frontend.h"

#include "SireError/printerror.h"

#include <QDebug>

using namespace SireCluster;

using boost::shared_ptr;

/** Private implementation of Cluster */
class ClusterPvt
{
public:
    ClusterPvt();
    
    ~ClusterPvt();
    
    /** Mutex to protect access to the registry */
    QMutex datamutex;
    
    /** Waitcondition used to wait until the cluster has been
        shut down - this allows the 'exec' function to work */
    QWaitCondition execwaiter;
    
    /** The registry of all backends that are local to this 
        address space */
    QHash<QUuid,Backend> local_backends;
};

using namespace SireCluster::detail;

/** Return whether or not there is any point using MPI
    (only worth it if we have more than one MPI process!) */
static bool usingMPI()
{
    #ifdef __SIRE_USE_MPI__
        if (not ::MPI::Is_initialized())
        {
            int argc = 0;
            char **argv = 0;
            ::MPI::Init(argc,argv);
        }
        
        return ::MPI::COMM_WORLD.Get_size() > 1;
    #else
        return false;
    #endif
}

Q_GLOBAL_STATIC( QMutex, globalMutex );
Q_GLOBAL_STATIC( QMutex, execMutex );

static ClusterPvt *global_cluster(0);
static bool global_cluster_is_running = false;

ClusterPvt* globalCluster()
{
    QMutexLocker lkr( globalMutex() );
    
    if (global_cluster == 0)
    {
        qDebug() << "Creating the global cluster in thread" 
                 << SireError::getPIDString();

        global_cluster = new ClusterPvt();
        
        lkr.unlock();
        
        execMutex()->lock();
        global_cluster_is_running = true;
        execMutex()->unlock();
        
        #ifdef __SIRE_USE_MPI__
            if (::usingMPI())
                SireCluster::MPI::MPICluster::start();
        #endif
        
        //create a new backend
        Backend::create();
    }
    
    return global_cluster;
}

/** Constructor for the global cluster */
ClusterPvt::ClusterPvt()
{}

/** Destructor */
ClusterPvt::~ClusterPvt()
{
    execwaiter.wakeAll();
}

/** Start the cluster - this is like exec, but it doesn't
    block until the cluster has been shutdown */
void Cluster::start()
{
    globalCluster();
}

/** Return whether or not the cluster is running */
bool Cluster::isRunning()
{
    QMutexLocker lkr( execMutex() );
    return global_cluster_is_running;
}

/** Wait for the global cluster to stop running */
void Cluster::wait()
{
    QMutexLocker lkr( execMutex() );
    
    if (global_cluster_is_running)
        globalCluster()->execwaiter.wait( execMutex() );
}

/** Start the cluster, and block until the cluster is shutdown */
void Cluster::exec()
{
    Cluster::start();
    Cluster::wait();
}

/** Return whether or not this cluster supports MPI
    (this is true if MPI is available, and there is more than
     one MPI process) */
bool Cluster::supportsMPI()
{
    return ::usingMPI();
}

/** Return the rank of the process - this is either the 
    rank of this process in the MPI group, or it is 0 */
int Cluster::getRank()
{
    #ifdef __SIRE_USE_MPI__
        if (::usingMPI())
            return SireCluster::MPI::MPICluster::getRank();
        else
            return 0;
    #else
        return 0;
    #endif
}

/** Return the number of processes - this is either the 
    size of the MPI group, or it is 1 */
int Cluster::getCount()
{
    #ifdef __SIRE_USE_MPI__
        if (::usingMPI())
            return SireCluster::MPI::MPICluster::getCount();
        else
            return 1;
    #else
        return 1;
    #endif
}

/** Register the backend 'backend' with the cluster */
void Cluster::registerBackend(const Backend &backend)
{
    if (backend.isNull())
        return;

    QMutexLocker lkr( &(globalCluster()->datamutex) );

    if (not globalCluster()->local_backends.contains(backend.UID()))
    {
        globalCluster()->local_backends.insert( backend.UID(), backend );
        
        #ifdef __SIRE_USE_MPI__
            //now inform the MPI connected nodes that a backend with this
            //UID is available on this node
            if (::usingMPI())
            {
                SireCluster::MPI::MPICluster::registerBackend(backend);
            }
        #endif
    }
}

/** Return a Frontend that will allow us to communicate with the 
    Backend with UID 'uid'. A null Frontend is returned if
    no Backend with this UID exists */
Frontend Cluster::getFrontend(const QUuid &uid)
{
    if (uid.isNull())
        return Frontend();

    QMutexLocker lkr( &(globalCluster()->datamutex) );
    
    if (globalCluster()->local_backends.contains(uid))
    {
        //return a local frontend for this local backend
        return Frontend( globalCluster()->local_backends.value(uid) );
    }
    else
    {
        lkr.unlock();
        
        #ifdef __SIRE_USE_MPI__
            if (::usingMPI())
                //see if this node exists on any of the MPI nodes...
                return SireCluster::MPI::MPICluster::getFrontend(uid);
            else
                return Frontend();
        #else
            return Frontend();
        #endif
    }
}

/** Return the list of all of the UIDs of the local nodes
    (the nodes that exist in this address space) */
QList<QUuid> Cluster::localUIDs()
{
    QMutexLocker lkr( &(globalCluster()->datamutex) );

    return globalCluster()->local_backends.keys();
}

/** Return the list of all of the UIDs of all of the nodes 
    in this entire cluster */
QList<QUuid> Cluster::UIDs()
{
    #ifdef __SIRE_USE_MPI__
        if (::usingMPI())
            return SireCluster::MPI::MPICluster::UIDs();
        else
            return Cluster::localUIDs();
    #else
        return Cluster::localUIDs();
    #endif
}

/** Shutdown this cluster */
void Cluster::shutdown()
{
    /////check to see if we are still running
    {
        QMutexLocker lkr( execMutex() );
        
        if (not global_cluster_is_running)
            return;
            
        global_cluster_is_running = false;
    }

    #ifdef __SIRE_USE_MPI__
        if (::usingMPI())
            //shutdown MPI - this stops the backends from 
            //receiving any more work from remote nodes
            SireCluster::MPI::MPICluster::shutdown();
    #endif

    QMutexLocker lkr( &(globalCluster()->datamutex) );
    
    //shutdown all of the backends
    for (QHash<QUuid,Backend>::iterator it = globalCluster()->local_backends.begin();
         it != globalCluster()->local_backends.end();
         ++it)
    {
        it.value().shutdown();
    }

    //wake all threads waiting for the cluster to be shutdown
    globalCluster()->execwaiter.wakeAll();
}
