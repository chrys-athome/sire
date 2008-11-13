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

#include <QHash>

#include "cluster.h"
#include "backend.h"
#include "frontend.h"

#ifdef __SIRE_USE_MPI__
#include "mpi/mpicluster.h"
#endif

using namespace SireCluster;

using boost::shared_ptr;

namespace SireCluster
{
namespace detail
{

/** Private implementation of Cluster */
class ClusterPvt
{
public:
    ClusterPvt()
    {}
    
    ~ClusterPvt()
    {}
    
    /** The registry of all backends that are local to this 
        address space */
    QHash<QUuid,Backend> local_backends;
};

} // end of namespace detail
} // end of namespace SireCluster

using namespace SireCluster::detail;

Q_GLOBAL_STATIC_WITH_ARGS( QMutex, globalMutex, QMutex::Recursive );

static Cluster *global_cluster(0);

static Cluster& Cluster::globalCluster()
{
    if (global_cluster == 0)
    {
        global_cluster = new Cluster();
        global_cluster->start();
    }
    
    return *global_cluster;
}

/** Constructor */
Cluster::Cluster() : d( new ClusterPvt() )
{}

/** Destructor */
Cluster::~Cluster()
{}

/** Start the cluster */
void Cluster::start()
{
    QMutexLocker lkr( globalMutex() );
    
    //create a backend that is local to this node
    Backend::create();
}

/** Register the backend 'backend' with the cluster */
void Cluster::registerBackend(const Backend &backend)
{
    if (backend.isNull())
        return;
    
    QMutexLocker lkr( globalMutex() );
        
    if (not cluster().d->local_backends.contains(backend.UID()))
    {
        cluster().d->local_backends.insert( backend.UID(), backend );
        
        #ifdef __SIRE_USE_MPI__
            //now inform the MPI connected nodes that a backend with this
            //UID is available on this node
            MPICluster::registerBackend(backend);
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

    QMutexLocker lkr( globalMutex() );
    
    if (cluster().d->local_backends.contains(uid))
    {
        //return a local frontend for this local backend
        return Frontend( cluster().d->local_backends.value(uid) );
    }
    else
    {
        lkr.unlock();
        
        #ifdef __SIRE_USE_MPI__
            //see if this node exists on any of the MPI nodes...
            return MPICluster::getFrontend(uid);
        #else
            return Frontend();
        #endif
    }
}

/** Return the list of all of the UIDs of the local nodes
    (the nodes that exist in this address space) */
QList<QUuid> Cluster::localUIDs()
{
    QMutexLocker lkr( globalMutex() );
    
    return cluster().d->local_backends.keys();
}

/** Return the list of all of the UIDs of all of the nodes 
    in this entire cluster */
QList<QUuid> Cluster::UIDs()
{
    #ifdef __SIRE_USE_MPI__
        return MPICluster::UIDs();
    #else
        return Cluster::localUIDs();
    #endif
}

/** Shutdown this cluster */
void Cluster::shutdown()
{
    QMutexLocker lkr( globalMutex() );

    if (global_cluster == 0)
    {
        //the cluster has already shutdown
        return;
    }
    
    Cluster &c = cluster();

    #ifdef __SIRE_USE_MPI__
        //shutdown MPI - this stops the backends from 
        //receiving any more work from remote nodes
        MPICluster::shutdown();
    #endif
    
    //shutdown all of the backends
    for (QHash<QUuid,Backend>::iterator it = c.d->local_backends.begin();
         it != c.d->local_backends.end();
         ++it)
    {
        it.value().shutdown();
    }
    
    //delete the cluster
    delete global_cluster;
    global_cluster = 0;
}
