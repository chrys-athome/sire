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

#include "nodes.h"

using namespace SireCluster;

using boost::shared_ptr;
using boost::weak_ptr;

namespace SireCluster
{
namespace detail
{

/** Private implementation of Nodes */
class NodesPvt
{
public:
    NodesPvt()
    {}
    
    ~NodesPvt()
    {}
    
    /** Lock used to protect access to the queues
          - this is locked whenever nodes are added or
             removed from the Nodes */
    QReadWriteLock nodeslock;
    
    /** Mutex to protect access to the main data
        (the busy and free queues) */
    QMutex datamutex;
    
    /** Pointer to a semaphore that is used to control
        the reservation and allocation of nodes */
    shared_ptr<QSemaphore> nodesem;
    
    /** The collection of all non-null Frontends in this
        nodes object, indexed by the UID of the backend they
        are connected to */
    QHash<QUuid,Frontend> frontends;
    
    /** The set of UIDs of the busy frontends */
    QSet<QUuid> busy_frontends;
    
    /** The list of UIDs of the free frontends - local front
        ends will tend to be at the beginning of this list */
    QList<QUuid> free_frontends;
};

} // end of namespace detail
} // end of namespace SireCluster

using namespace SireCluster::detail;

//////////////
////////////// Implementation of Nodes
//////////////

/** Construct an empty set of nodes */
Nodes::Nodes() : d( new NodesPvt() )
{}

/** Copy constructor - Nodes are explicitly shared */
Nodes::Nodes(const Nodes &other) : d(other.d)
{}

/** Destructor */
Nodes::~Nodes()
{
    if (d.unique())
    {
        this->removeAll();
    }
}

/** Copy assignment operator - Nodes are explicitly shared */
Nodes& Nodes::operator=(const Nodes &other)
{
    d = other.d;
    return *this;
}

/** Comparison operator */
bool Nodes::operator==(const Nodes &other) const
{
    return d.get() == other.d.get();
}

/** Comparison operator */
bool Nodes::operator!=(const Nodes &other) const
{
    return d.get() != other.d.get();
}

/** Return a free node - this blocks until a free node
    is available
    
    \throw SireError::unavailable_resource
*/
Node Nodes::getFreeNode()
{
    QReadLocker read_lkr( &(d->nodeslock) );
    
    if (d->node_sem.available() == 0)
        throw SireError::unavailable_resource( QObject::tr(
            "It is not possible to get a free node as there are no "
            "nodes available in this Nodes object."), CODELOC );

    //reserve a node
    d->nodesem->acquire();

    //now wait to collect my reservation
    QMutexLocker lkr( &(d->datamutex) );

    Node node = _pvt_getNode();
    
    return node;
}

/** Return 'n' free nodes - this blocks until all of the 
    nodes are available
    
    \throw SireError::unavailable_resource
*/
QList<Node> Nodes::getNFreeNodes(int n)
{
    QList<Node> nodes;

    if (n <= 0)
    {
        return nodes;
    }
    else if (n == 1)
    {
        nodes.append( this->getFreeNode() );
    }
    else
    {
        QReadLocker read_lkr( &(d->nodeslock) );
    
        if (d->node_sem->available() < n)
            throw SireError::unavailable_resource( QObject::tr(
                "It is not possible to get %1 free nodes, as there "
                "are only %2 nodes available in this Nodes object.")
                    .arg(n).arg(d->node_sem.available()), CODELOC );
    
        //reserve 'n' nodes
        d->nodesem->aquire(n);

        QMutexLocker lkr( &(d->datamutex) );

        for (int i=0; i<n; ++i)
        {
            nodes.append( _pvt_getNode() );
        }
    }
    
    return nodes;
}

/** Try to get a free node - giving only 'timeout' milliseconds
    to get that node */
Node Nodes::getFreeNode(int timeout)
{
    QTime t;
    t.start();
    
    if (timeout < 0)
        return this->getFreeNode();
    
    Node node;
                
    QReadLocker read_lkr( &(d->nodeslock) );
    
    if (d->node_sem.available() == 0)
        throw SireError::unavailable_resource( QObject::tr(
            "It is not possible to get a free node as there are no "
            "nodes available in this Nodes object."), CODELOC );

    timeout -= t.elapsed();
    
    if (timeout <= 0)
        return node;

    //reserve a node
    if (d->nodesem->tryAcquire(1, timeout))
    {
        //now collect my reservation
        QMutexLocker lkr( &(d->datamutex) );
        node = _pvt_getNode();
    }
    
    return node;
}

/** Try to get 'n' free nodes, within the time 'timeout'.
    If this fails, then *no* nodes are returned */
QList<Node> Nodes::getNFreeNodes(int n, int timeout)
{
    QTime t;
    t.start();

    if (timeout <= 0)
        return this->getNFreeNodes(n);

    QList<Node> nodes;
    
    if (n <= 0)
    {
        return nodes;
    }
    else if (n == 1)
    {
        nodes.append( this->getFreeNode(timeout) );
    }
    else
    {
        QReadLocker read_lkr( &(d->nodeslock) );
        
        timeout -= t.elapsed();
        
        if (timeout <= 0)
            return nodes;
        
        if (d->nodesem->acquire(n, timeout))
        {
            for (int i=0; i<n; ++i)
            {
                nodes.append( _pvt_getNode() );
            }
        }
    }
    
    return nodes;
}

/** Wait until all of the nodes are free */
void Nodes::waitUntilAllFree()
{
    QReadLocker read_lkr( &(d->nodeslock) );
    
    d->nodesem->acquire( d->nodesem->available() );
    d->nodesem->release( d->nodesem->available() );
}

/** Wait until all of the nodes are free, or until
    timeout milliseconds have passed - this returns
    whether or not all of the nodes are free */
bool Nodes::waitUntilAllFree(int timeout)
{
    QReadLocker read_lkr( &(d->nodeslock) );
    
    if (d->nodesem->acquire( d->nodesem->available(), timeout ))
    {
        d->nodesem->release( d->nodesem->available() );
        return true;
    }
    else
        return false;
}

/** Return the number of free nodes */
int Nodes::nFreeNodes() const
{
    QReadLocker read_lkr( &(d->nodeslock) );
    QMutexLocker lkr( &(d->datamutex) );
    return d->free_frontends.count();
}

/** Return the number of busy nodes */
int Nodes::nBusyNodes() const
{
    QReadLocker read_lkr( &(d->nodeslock) );
    QMutexLocker lkr( &(d->datamutex) );
    return d->busy_frontends.count();
}

/** Return the total number of nodes available */
int Nodes::nNodes() const
{
    QReadLocker read_lkr( &(d->nodeslock) );
    QMutexLocker lkr( &(d->datamutex) );
    return d->frontends.count();
}

/** Return the total number of nodes available */
int Nodes::count() const
{
    return this->nNodes();
}

/** Remove all nodes from this set - this aborts any running
    jobs, then disconnects from all of the backends. 
    Note that this does not block  */
void Nodes::removeAll()
{
    //// Do everything that doesn't involve
    //// changing the semaphore
    {
        QReadLocker read_lkr( &(d->nodeslock) );
    
        QMutexLocker lkr(&datamutex);

        for (QHash<QUuid,Frontend>::iterator it = d->frontends.begin();
             it != d->frontends.end();
             ++it)
        {
            it.value().abortJob();
        }
    
        d->frontends.clear();
        d->busy_frontends.clear();
        d->free_frontends.clear();
    }
    
    ///// Now change the semaphore
    {
        QWriteLocker write_lkr( &(d->nodeslock) );
        d->nodesem.release( d->nodesem.available() );
        d->nodesem.reset( new QSemaphore(0) );
    }
}

//////////////
////////////// Implementation of NodesPtr
//////////////

/** Function called by the destructor of NodePvt to return
    a front end back to the Nodes from whence it came */
void NodesPtr::returnFrontend(const Frontend &frontend)
{
    shared_ptr<NodesPvt> nodes = d.lock();
    
    if (nodes.get() == 0)
        return;
    
    QUuid uid = frontend.UID();
                
    QMutexLocker lkr( &(nodes.datamutex) );
    
    if (nodes->busy_frontends.contains(uid))
    {
        //put the local front ends at the beginning of 
        //the list so that they are used first

        if (frontend.isLocal())
            nodes->free_frontends.prepend(uid);
        else
            nodes->free_frontends.append(uid);
    
        nodes->busy_frontends.remove(uid);
        nodes->nodesem->release();
    }
}
