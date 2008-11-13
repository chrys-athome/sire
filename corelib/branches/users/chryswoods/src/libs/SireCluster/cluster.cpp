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

using namespace SireCluster;
//using namespace SireMPI;

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
    
    /** Mutex to protect access to this cluster */
    QMutex datamutex;
    
    /** The registry of all backends that are local to this 
        address space */
    QHash<QUuid,Backend> local_backends;
};

} // end of namespace detail
} // end of namespace SireCluster

using namespace SireCluster::detail;

Q_GLOBAL_STATIC( Cluster, globalCluster );

/** Constructor */
Cluster::Cluster() : d( new ClusterPvt() )
{}

/** Destructor */
Cluster::~Cluster()
{}

/** Register the backend 'backend' with the cluster */
void Cluster::registerBackend(const Backend &backend)
{
    if (backend.isNull())
        return;
        
    Cluster *cluster = globalCluster();
    
    QMutexLocker lkr( &(cluster->d->datamutex) );
    
    if (not cluster->d->local_backends.contains(backend.UID()))
    {
        cluster->d->local_backends.insert( backend.UID(), backend );
        
        //now inform the MPI connected nodes that a backend with this
        //UID is available on this node
        //SireMPI::MPICluster::registerBackend(backend);
    }
}

/** Return a Frontend that will allow us to communicate with the 
    Backend with UID 'uid'. A null Frontend is returned if
    no Backend with this UID exists */
Frontend Cluster::getFrontend(const QUuid &uid)
{
    if (uid.isNull())
        return Frontend();

    Cluster *cluster = globalCluster();
    
    QMutexLocker lkr( &(cluster->d->datamutex) );
    
    if (cluster->d->local_backends.contains(uid))
    {
        //return a local frontend for this local backend
        return Frontend( cluster->d->local_backends.value(uid) );
    }
    else
    {
        //see if this node exists on any of the MPI nodes...
        return Frontend(); // SireMPI::MPICluster::getFrontend(uid);
    }
}

/** Return the list of all of the UIDs of the local nodes
    (the nodes that exist in this address space) */
QList<QUuid> Cluster::localUIDs()
{
    Cluster *cluster = globalCluster();
    
    QMutexLocker lkr( &(cluster->d->datamutex) );
    
    return cluster->d->local_backends.keys();
}

/** Return the list of all of the UIDs of all of the nodes 
    in this entire cluster */
QList<QUuid> Cluster::UIDs()
{
    return QList<QUuid>(); // SireMPI::MPICluster::UIDs();
}

/** Shutdown this cluster */
void Cluster::shutdown()
{
    Cluster *cluster = globalCluster();
    
    QMutexLocker lkr( &(cluster->d->datamutex) );
    
    //shutdown all of the backends
    for (QHash<QUuid,Backend>::iterator it = cluster->d->local_backends.begin();
         it != cluster->d->local_backends.end();
         ++it)
    {
        it.value().shutdown();
    }
}
