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
//mpich requires that mpi.h is included first
#include <mpi.h>                  // CONDITIONAL_INCLUDE
#endif

#include "mpinodes.h"
#include "mpinode.h"
#include "mpipromise.h"
#include "mpifrontends.h"
#include "mpibackends.h"

#include "SireError/errors.h"

#include <QMutex>
#include <QSemaphore>
#include <QUuid>
#include <QList>
#include <QThread>
#include <QSet>

#include <QDebug>

using namespace SireMPI;

using boost::shared_ptr;
using boost::weak_ptr;

#ifdef __SIRE_USE_MPI__

//////////////
//////////////
// MPI enabled version of these classes
//////////////
//////////////

namespace SireMPI
{
namespace detail
{

/** Private implementation of MPINodes */
class MPINodesPvt : public QThread
{
public:
    MPINodesPvt(int num_nodes=0) 
            : data_mutex( QMutex::Recursive ), sem(num_nodes), mpicomm(0)
    {}
    
    ~MPINodesPvt()
    {
        if (this->isRunning())
        {
            frontends.shutdown();
            this->wait();
        }
    
        if (sem.available() > 0)
        {
            sem.acquire( sem.available() );
            sem.release( sem.available() );
            mpicomm.Free();
        }
    }

    /** Mutex to protect access to the data of this class */
    QMutex data_mutex;
    
    /** Semaphore used to control access to the nodes */
    QSemaphore sem;
    
    /** Pointer to this object */
    MPINodesPtr this_ptr;

    /** All of the available MPI nodes in this communicator */
    QList<MPINode> free_nodes;
    
    /** All of the busy nodes in this communicator */
    QHash<QUuid,MPINodePtr> busy_nodes;

    /** The list of the UIDs of all of the nodes in this communicator */
    QList<QUuid> node_uids;

    /** The MPI communicator used to broadcast messages to
        the nodes of this group */
    MPI::Intracomm mpicomm;
    
    /** The frontends for all of the nodes - this is null until
        'exec' is called */
    MPIFrontends frontends;
    
    /** The backends for all of the nodes - this is null until
        'exec' is called */
    MPIBackends backends;
    
protected:
    void run()
    {
        backends.exec();
    }
};

/** Private implementation of MPIBackendNodes */
class MPIBackendNodesPvt
{
public:
    MPIBackendNodesPvt()
    {}
    
    ~MPIBackendNodesPvt()
    {
        mpicomm.Free();
    }

    /** The MPI communicator used to broadcast messages to
        the nodes of this group */
    MPI::Intracomm mpicomm;

    /** All of the backends for the nodes */
    MPIBackends backends;
};

} // end of namespace detail
} // end of namespace SireMPI

////////
//////// Implementation of MPINodes
////////

using namespace SireMPI::detail;

Q_GLOBAL_STATIC( QMutex, commWorldMutex );

static shared_ptr<MPINodesPvt> comm_world;

/** Return the nodes that corresponds to MPI::COMM_WORLD */
MPINodes MPINodes::COMM_WORLD()
{
    qDebug() << MPI::COMM_WORLD.Get_rank() << CODELOC;

    QMutexLocker lkr( commWorldMutex() );

    qDebug() << MPI::COMM_WORLD.Get_rank() << CODELOC;
    
    if (comm_world.get() == 0)
    {
        qDebug() << MPI::COMM_WORLD.Get_rank() << CODELOC;

        //ensure that MPI is initialized
        if (not MPI::Is_initialized())
        {
            int argc=0;
            char **argv=0;
            MPI::Init(argc, argv);
        }

        qDebug() << MPI::COMM_WORLD.Get_rank() << CODELOC;
        
        //now create a clone of MPI::COMM_WORLD that is
        //used to broadcast information to the nodes
        MPI::Intracomm mpicomm = MPI::COMM_WORLD.Clone();
        
        int nnodes = mpicomm.Get_size();
        int my_rank = mpicomm.Get_rank();

        qDebug() << MPI::COMM_WORLD.Get_rank() << CODELOC;
        
        comm_world.reset( new MPINodesPvt(nnodes) );
        comm_world->mpicomm = mpicomm;
        
        MPINodes nodes(comm_world);
        
        //now create all of the nodes
        for (int i=0; i<nnodes; ++i)
        {
            qDebug() << MPI::COMM_WORLD.Get_rank() << i << CODELOC;

            comm_world->node_uids.append( QUuid::createUuid() );

            MPINode node(nodes, i, i==my_rank);
        
            comm_world->free_nodes.append(node);
        }
    }

    qDebug() << MPI::COMM_WORLD.Get_rank() << CODELOC;
    
    return MPINodes(comm_world);
}

/** Constructor - this is equal to MPINodes::COMM_WORLD() */
MPINodes::MPINodes()
{
    this->operator=( MPINodes::COMM_WORLD() );
}

/** Construct from the passed pointer */
MPINodes::MPINodes(const shared_ptr<MPINodesPvt> &ptr) : d(ptr)
{}

/** Copy constructor */
MPINodes::MPINodes(const MPINodes &other) : d(other.d)
{}

/** Destructor */
MPINodes::~MPINodes()
{}

/** Copy assignment operator */
MPINodes& MPINodes::operator=(const MPINodes &other)
{
    d = other.d;
    return *this;
}

/** Comparison operator */
bool MPINodes::operator==(const MPINodes &other) const
{
    return d.get() == other.d.get();
}

/** Comparison operator */
bool MPINodes::operator!=(const MPINodes &other) const
{
    return d.get() != other.d.get();
}

/** Wait until all of the nodes are free */
void MPINodes::waitUntilAllFree()
{
    d->sem.acquire( d->sem.available() );
    d->sem.release( d->sem.available() );
}

/** Wait until all of the nodes are free, or until 'timeout'
    milliseconds have passed - returns whether or not all
    nodes are free */
bool MPINodes::waitUntilAllFree(int timeout)
{
    if (d->sem.tryAcquire(d->sem.available(), timeout))
    {
        d->sem.release( d->sem.available() );
        return true;
    }
    else
        return false;
}

/** Return a pointer to the MPI communicator for these nodes.
    This is a MPI::Intracomm object, but is returned as a void
    so that the non-MPI version doesn't break binary compatibility.
    The non-MPI version returns a null pointer. */
const void* MPINodes::communicator() const
{
    QMutexLocker lkr( &(d->data_mutex) );
    return &(d->mpicomm);
}

/** Enter the event loop for these nodes */
void MPINodes::exec()
{
    qDebug() << CODELOC;

    if (d->isRunning())
        //the event loop is already running
        return;

    qDebug() << CODELOC;

    //first start the backends for these nodes in a background thread
    d->backends = getBackends(*this);
    qDebug() << CODELOC;

    d->start();

    qDebug() << CODELOC;
    
    //now create the front ends - this actually starts all of the backends
    d->frontends = getFrontends(*this);

    qDebug() << CODELOC;
}

/** Shutdown the MPI event loops on all nodes involved in this communicator */
void MPINodes::shutdown()
{
    if (d->isRunning())
        d->frontends.shutdown();
}

/** Return the number of nodes in this communicator */
int MPINodes::count() const
{
    QMutexLocker lkr( &(const_cast<MPINodes*>(this)->d->data_mutex) );
    return d->sem.available();
}

/** Return the number of free nodes */
int MPINodes::nFreeNodes() const
{
    QMutexLocker lkr( &(const_cast<MPINodes*>(this)->d->data_mutex) );
    return d->free_nodes.count();
}

/** Return the number of busy nodes */
int MPINodes::nBusyNodes() const
{
    QMutexLocker lkr( &(const_cast<MPINodes*>(this)->d->data_mutex) );
    return d->busy_nodes.count();
}

/** Return the UID of the ith node 

    \throw SireError::invalid_index
*/
const QUuid& MPINodes::getUID(int rank) const
{
    QMutexLocker lkr( &(const_cast<MPINodes*>(this)->d->data_mutex) );
    
    if (rank < 0 or rank >= d->node_uids.count())
        throw SireError::invalid_index( QObject::tr(
            "There is no node with rank %1 in this communicator.")
                .arg(rank), CODELOC );
                
    return d->node_uids.at(rank);
}

/** This indicates that a node has been returned to the pool of available nodes */
void MPINodes::returnedNode()
{
    qDebug() << MPINode::globalRank() << CODELOC;
    QMutexLocker lkr( &(d->data_mutex) );

    qDebug() << MPINode::globalRank() << CODELOC;
    
    //loop over the busy nodes and remove the ones that have finished
    QMutableHashIterator<QUuid,MPINodePtr> it(d->busy_nodes);

    qDebug() << MPINode::globalRank() << CODELOC;

    int my_rank = d->mpicomm.Get_rank();

    qDebug() << MPINode::globalRank() << CODELOC;
    
    while (it.hasNext())
    {
        it.next();

        qDebug() << MPINode::globalRank() << CODELOC;
        
        if (it.value().isNull())
        {
            qDebug() << MPINode::globalRank() << CODELOC;

            //the job has finished
            int rank = d->node_uids.indexOf(it.key());
            d->free_nodes.append( MPINode(*this, rank, rank == my_rank) );
            
            it.remove();
            d->sem.release();
        }
    }

    qDebug() << MPINode::globalRank() << CODELOC;
    
    BOOST_ASSERT( d->free_nodes.count() + d->busy_nodes.count() 
                                    == d->sem.available() );

    qDebug() << MPINode::globalRank() << CODELOC;
}

MPINode MPINodes::_pvt_getNode()
{
    MPINode node = d->free_nodes.takeFirst();
    d->busy_nodes.insert( node.UID(), node );
    
    return node;
}

/** Return a node that is not busy. The node will be marked as busy
    until you return it. This function will block until a free
    node is available 

    \throw SireError::unavailable_resource
*/
MPINode MPINodes::getFreeNode()
{
    if (this->count() <= 0)
        throw SireError::program_bug( QObject::tr(
            "It should not be possible to create an empty set of MPINodes!!!"),
                CODELOC );

    d->sem.acquire(1);
    
    QMutexLocker lkr( &(d->data_mutex) );
    return this->_pvt_getNode();
}

/** Return a node that is not busy. The node will be marked as busy
    until you return it. This function will block until a free
    node is available, or until time milliseconds has passed.
    If we run out of time, then a null node is returned.

    \throw SireError::unavailable_resource
*/
MPINode MPINodes::getFreeNode(int time)
{
    if (this->count() <= 0)
        throw SireError::program_bug( QObject::tr(
            "It should not be possible to create an empty set of MPINodes!!!"),
                CODELOC );

    if (d->sem.tryAcquire(1, time))
    {
        QMutexLocker lkr( &(d->data_mutex) );
        return this->_pvt_getNode();
    }
    else
        return MPINode();
}

/** Return a list of N free nodes. This will block until all N nodes
    are available, and will return all of the nodes at once, marking
    each one as busy
    
    \throw SireError::unavailable_resource
*/
QList<MPINode> MPINodes::getNFreeNodes(int count)
{
    if (this->count() < count)
        throw SireError::unavailable_resource( QObject::tr(
            "You've requested %d nodes, but there are only %d available...")
                .arg(count).arg(this->count()), CODELOC );

    d->sem.acquire(count);
    
    QList<MPINode> free_nodes;
    QMutexLocker lkr( &(d->data_mutex) );
    
    for (int i=0; i<count; ++i)
    {
        free_nodes.append( this->_pvt_getNode() );
    }
    
    return free_nodes;
}

/** Return a list of N free nodes. This will block until all N nodes
    are available, and will return all of the nodes at once, marking
    each one as busy. This blocks until all N nodes are available, 
    or until time milliseconds has passed.
    If we run out of time, then an empty list is returned.
    
    \throw SireError::unavailable_resource
*/
QList<MPINode> MPINodes::getNFreeNodes(int count, int time)
{
    if (this->count() < count)
        throw SireError::unavailable_resource( QObject::tr(
            "You've requested %d nodes, but there are only %d available...")
                .arg(count).arg(this->count()), CODELOC );

    QList<MPINode> free_nodes;

    if (d->sem.tryAcquire(count, time))
    {
        QMutexLocker lkr( &(d->data_mutex) );

        for (int i=0; i<count; ++i)
        {
            free_nodes.append( this->_pvt_getNode() );
        }
    }

    return free_nodes;
}

/** Return the total number of nodes in this group */
int MPINodes::nNodes() const
{
    return this->count();
}

////////
//////// Implementation of MPIBackendNodes
////////

static shared_ptr<MPIBackendNodesPvt> backend_comm_world;

/** Call this on the backend nodes to get the backend to the MPINodes
    object that represents the MPI::COMM_WORLD communicator */
MPIBackendNodes MPIBackendNodes::COMM_WORLD()
{
    QMutexLocker lkr( commWorldMutex() );
    
    if (backend_comm_world.get() == 0)
    {
        if (not MPI::Is_initialized())
        {
            int argc=0;
            char **argv=0;
            MPI::Init(argc,argv);
        }

        //now create a clone of MPI::COMM_WORLD that is
        //used to broadcast information to the nodes
        MPI::Intracomm mpicomm = MPI::COMM_WORLD.Clone();
        
        backend_comm_world.reset( new MPIBackendNodesPvt() );
        backend_comm_world->mpicomm = mpicomm;
        
        MPIBackendNodes nodes(backend_comm_world);
        
        //get the backend for this node
        backend_comm_world->backends = getBackends(nodes);
    }
    
    return MPIBackendNodes(backend_comm_world);
}

/** Constructor - this constructs the backend nodes for MPI::COMM_WORLD */
MPIBackendNodes::MPIBackendNodes()
{
    this->operator=( MPIBackendNodes::COMM_WORLD() );
}

/** Construct from the passed pointer */
MPIBackendNodes::MPIBackendNodes(const shared_ptr<detail::MPIBackendNodesPvt> &ptr)
                : d(ptr)
{}

/** Copy constructor */
MPIBackendNodes::MPIBackendNodes(const MPIBackendNodes &other)
                : d(other.d)
{}

/** Destructor */
MPIBackendNodes::~MPIBackendNodes()
{}

/** Copy assignment operator */
MPIBackendNodes& MPIBackendNodes::operator=(const MPIBackendNodes &other)
{
    d = other.d;
    return *this;
}

/** Comparison operator */
bool MPIBackendNodes::operator==(const MPIBackendNodes &other) const
{
    return d.get() == other.d.get();
}

/** Comparison operator */
bool MPIBackendNodes::operator!=(const MPIBackendNodes &other) const
{
    return d.get() != other.d.get();
}

/** Return the number of nodes in the communicator */
int MPIBackendNodes::nNodes() const
{
    return d->mpicomm.Get_size();
}

/** Return the number of nodes in the communicator */
int MPIBackendNodes::count() const
{
    return this->nNodes();
}

/** Return a pointer to the MPI communicator for these nodes.
    This is a MPI::Intracomm object, but is returned as a void
    so that the non-MPI version doesn't break binary compatibility.
    The non-MPI version returns a null pointer. */
const void* MPIBackendNodes::communicator() const
{
    return &(d->mpicomm);
}

/** Enter the event loop for these node - this function blocks until
    the event loop has finished */
void MPIBackendNodes::exec()
{
    MPIBackends backends = getBackends(*this);
    backends.exec();
}

#else //ifdef __SIRE_USE_MPI__

//////////////
//////////////
// MPI disabled version of these classes
//////////////
//////////////
    #error There is no non-MPI version of MPINodes
    
#endif // ifdef __SIRE_USE_MPI__

////////////
//////////// Implementation of MPINodesPtr
////////////

/** Null constructor */
MPINodesPtr::MPINodesPtr()
{}

/** Construct a pointer to the nodes 'nodes' */
MPINodesPtr::MPINodesPtr(const MPINodes &nodes) : d(nodes.d)
{}

/** Copy constructor */
MPINodesPtr::MPINodesPtr(const MPINodesPtr &other) : d(other.d)
{}

/** Destructor */
MPINodesPtr::~MPINodesPtr()
{}

/** Copy assignment operator */
MPINodesPtr& MPINodesPtr::operator=(const MPINodesPtr &other)
{
    d = other.d;
    return *this;
}

/** Comparison operator */
bool MPINodesPtr::operator==(const MPINodesPtr &other) const
{
    return d.lock().get() == other.d.lock().get();
}

/** Comparison operator */
bool MPINodesPtr::operator!=(const MPINodesPtr &other) const
{
    return d.lock().get() != other.d.lock().get();
}

/** Dereference this pointer

    \throw SireError::nullptr_error
*/
MPINodes MPINodesPtr::operator*() const
{
    boost::shared_ptr<MPINodesPvt> ptr = d.lock();
    
    if (ptr.get() == 0)
        throw SireError::nullptr_error( QObject::tr(
            "Attempting to dereference a null MPINodes pointer"),   
                CODELOC );
                
    return MPINodes(ptr);
}

/** Return whether or not the pointer is null */
bool MPINodesPtr::isNull() const
{
    return d.expired();
}

////////////
//////////// Implementation of MPIBackendNodesPtr
////////////

/** Null constructor */
MPIBackendNodesPtr::MPIBackendNodesPtr()
{}

/** Construct a pointer to the nodes 'nodes' */
MPIBackendNodesPtr::MPIBackendNodesPtr(const MPIBackendNodes &nodes) : d(nodes.d)
{}

/** Copy constructor */
MPIBackendNodesPtr::MPIBackendNodesPtr(const MPIBackendNodesPtr &other) : d(other.d)
{}

/** Destructor */
MPIBackendNodesPtr::~MPIBackendNodesPtr()
{}

/** Copy assignment operator */
MPIBackendNodesPtr& MPIBackendNodesPtr::operator=(const MPIBackendNodesPtr &other)
{
    d = other.d;
    return *this;
}

/** Comparison operator */
bool MPIBackendNodesPtr::operator==(const MPIBackendNodesPtr &other) const
{
    return d.lock().get() == other.d.lock().get();
}

/** Comparison operator */
bool MPIBackendNodesPtr::operator!=(const MPIBackendNodesPtr &other) const
{
    return d.lock().get() != other.d.lock().get();
}

/** Dereference this pointer

    \throw SireError::nullptr_error
*/
MPIBackendNodes MPIBackendNodesPtr::operator*() const
{
    boost::shared_ptr<MPIBackendNodesPvt> ptr = d.lock();
    
    if (ptr.get() == 0)
        throw SireError::nullptr_error( QObject::tr(
            "Attempting to dereference a null MPIBackendNodes pointer"),   
                CODELOC );
                
    return MPIBackendNodes(ptr);
}

/** Return whether or not the pointer is null */
bool MPIBackendNodesPtr::isNull() const
{
    return d.expired();
}
