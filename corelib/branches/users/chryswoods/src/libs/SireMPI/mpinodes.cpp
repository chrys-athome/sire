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

#include <boost/weak_ptr.hpp>

#include "mpinodes.h"
#include "mpinode.h"
#include "mpipromise.h"

#include "SireBase/incremint.h"

#include "SireError/errors.h"

#include "detail/mpidetail.h"   // CONDITIONAL_INCLUDE

#include <QDebug>

using namespace SireMPI;
using namespace SireMPI::detail;

using namespace SireBase;

Q_GLOBAL_STATIC( QMutex, mpiMutex );

////////
//////// Implementation of MPINodesData
////////

MPINodesData::MPINodesData() : sem(1), nnodes(1)
{
    #ifndef __SIRE_USE_MPI__
    mpicomm = 0;
    #endif
}

#ifdef __SIRE_USE_MPI__
    //increment used to get new tags - the global tag is 1, so
    //this starts at 2
    static Incremint last_mpitag(1);

    static int getUniqueMPITag()
    {
        int mpitag = last_mpitag.increment();
        qDebug() << "New mpitag == " << mpitag;
    
        return mpitag;
    }
#endif

/** By default we construct the an object that represents
    the global MPI_WORLD */
boost::shared_ptr<MPINodesData> MPINodesData::construct()
{
    boost::shared_ptr<MPINodesData> d( new MPINodesData() );
    
    d->this_ptr = d;
    
    #ifdef __SIRE_USE_MPI__
    
    //protect access to MPI functions
    {
        QMutex *mpi_mutex = mpiMutex();
        
        if (mpi_mutex == 0)
            //we are being destroyed
            return boost::shared_ptr<MPINodesData>();
    
        QMutexLocker lkr(mpi_mutex);
    
        if (not SireMPI::exec_running())
        {
            //SireMPI::exec() has not been called in this process. This suggests
            //that we are not part of an explicit MPI executable.
            //We need to call it ourselves and set it running in a background thread
            int argc = 0;
            SireMPI::bg_exec(argc, 0);
        }
        
        //this is the global communicator
        d->mpicomm = &(SireMPI::COMM_WORLD());
        
        //how many MPI nodes are there?
        d->nnodes = d->mpicomm->Get_size();
        
        //what is our rank?
        d->my_mpirank = d->mpicomm->Get_rank();
        
        //get a unique mpi tag for all communications between nodes
        d->mpitag = getUniqueMPITag();
        
        //now create MPINode objects to represent each node
        for (int i=0; i<d->nnodes; ++i)
        {
            d->free_nodes.append( MPINode(d, i, i == d->my_mpirank) );
        }
    }
    
    #else
    d->free_nodes.append( MPINode(d, 0, true) );
    
    #endif

    return d;
}

/** Wait until all of the nodes are free */
void MPINodesData::waitUntilAllFree()
{
    sem.tryAcquire(nnodes);
}

/** Wait until all of the nodes are free, or until 'timeout'
    milliseconds have passed - returns whether or not all
    nodes are free */
bool MPINodesData::waitUntilAllFree(int timeout)
{
    return sem.tryAcquire(nnodes, timeout);
}

/** Destructor - wait until all of the nodes in this communicator
    are free */
MPINodesData::~MPINodesData()
{}

/** Return the MPI communicator */
#ifdef __SIRE_USE_MPI__
    MPI::Comm* MPINodesData::mpiCommunicator()
    {
        QMutexLocker lkr(&data_mutex);
        return mpicomm;
    }

    int MPINodesData::mpiTag()
    {
        QMutexLocker lkr(&data_mutex);
        return mpitag;
    }
#endif

/** Return the number of nodes in this communicator */
int MPINodesData::count() const
{
    QMutexLocker lkr( &(const_cast<MPINodesData*>(this)->data_mutex) );
    return nnodes;
}

/** Return the number of free nodes */
int MPINodesData::nFreeNodes() const
{
    QMutexLocker lkr( &(const_cast<MPINodesData*>(this)->data_mutex) );
    return free_nodes.count();
}

/** Return the number of busy nodes */
int MPINodesData::nBusyNodes() const
{
    QMutexLocker lkr( &(const_cast<MPINodesData*>(this)->data_mutex) );
    return busy_nodes.count();
}

/** This code is called by the MPINodeData constructor to show that
    a node has been returned to the available nodes pool */
void MPINodesData::returnNode(const MPINode &node)
{
    QMutexLocker lkr(&data_mutex);
    
    //loop over the busy nodes and remove the one that matches this node
    QMutableListIterator< boost::weak_ptr<MPINodeData> > it(busy_nodes);
    
    while (it.hasNext())
    {
        boost::shared_ptr<MPINodeData> my_node = it.next().lock();
        
        if (my_node.get() == 0)
        {
            it.remove();
            
            qDebug() << "Returning a weak node...";
            sem.release();
            
            continue;
        }
        
        if (my_node->uid == node.UID())
        {
            #ifdef __SIRE_USE_MPI__
                BOOST_ASSERT( my_node->mpirank == node.rank() );
            #endif
                
            it.remove();
            
            qDebug() << "Returning node" << node.UID().toString();
            sem.release();
        }
    }
    
    //add this node to the list of free nodes
    free_nodes.append(node);
    
    BOOST_ASSERT( free_nodes.count() + busy_nodes.count() == nnodes );
}

MPINode MPINodesData::_pvt_getNode()
{
    MPINode free_node = free_nodes.takeLast();
    busy_nodes.append( free_node.d );
    
    qDebug() << "Here is the available node" << free_node.UID().toString();
    
    return free_node;
}

/** Return one free node. This blocks until a node is available */
MPINode MPINodesData::getFreeNode()
{
    if (nnodes <= 0)
        throw SireError::program_bug( QObject::tr(
            "It should not be possible to create an empty set of MPINodes!!!"),
                CODELOC );

    sem.acquire(1);
    
    QMutexLocker lkr(&data_mutex);
    return this->_pvt_getNode();
}

/** Return one free node. This blocks until a node is available, or
    until 'time' milliseconds have passed */
MPINode MPINodesData::getFreeNode(int time)
{
    if (nnodes <= 0)
        throw SireError::program_bug( QObject::tr(
            "It should not be possible to create an empty set of MPINodes!!!"),
                CODELOC );

    if (sem.tryAcquire(1, time))
    {
        QMutexLocker lkr(&data_mutex);
        return this->_pvt_getNode();
    }
    else
        return MPINode();
}

/** Return 'count' free nodes */
QList<MPINode> MPINodesData::getNFreeNodes(int count)
{
    if (nnodes < count)
        throw SireError::unavailable_resource( QObject::tr(
            "You've requested %d nodes, but there are only %d available...")
                .arg(count).arg(nnodes), CODELOC );

    sem.acquire(count);
    
    QList<MPINode> free_nodes;
    QMutexLocker lkr(&data_mutex);
    
    for (int i=0; i<count; ++i)
    {
        free_nodes.append( this->_pvt_getNode() );
    }
    
    return free_nodes;
}

QList<MPINode> MPINodesData::getNFreeNodes(int count, int time)
{
    if (nnodes < count)
        throw SireError::unavailable_resource( QObject::tr(
            "You've requested %d nodes, but there are only %d available...")
                .arg(count).arg(nnodes), CODELOC );

    QList<MPINode> free_nodes;

    if (sem.tryAcquire(count, time))
    {
        QMutexLocker lkr(&data_mutex);

        for (int i=0; i<count; ++i)
        {
            free_nodes.append( this->_pvt_getNode() );
        }
    }

    return free_nodes;
}

/////////
///////// Implementation of MPINodes
/////////

static boost::shared_ptr<detail::MPINodesData> mpi_comm_world;

Q_GLOBAL_STATIC( QMutex, worldMutex );

/** Return the group of nodes that represent MPI_COMM_WORLD - i.e.
    all of the MPI nodes available to this instance
    (ignoring spawned or remote node groups) */
MPINodes MPINodes::world()
{
    QMutex *world_mutex = worldMutex();
    
    if (world_mutex == 0)
        //we are being destroyed
        return MPINodes( boost::shared_ptr<detail::MPINodesData>() );

    QMutexLocker lkr(world_mutex);
    
    if (mpi_comm_world.get() == 0)
    {
        mpi_comm_world = MPINodesData::construct(); 
    }
    
    return mpi_comm_world;
}

/** Null constructor - this the equivalent of MPI_COMM_WORLD */
MPINodes::MPINodes()
{
    this->operator=( MPINodes::world() );
}

/** Private constructor used by static construction functions */
MPINodes::MPINodes(const boost::shared_ptr<MPINodesData> &data)
         : d(data)
{}

/** Copy constructor */
MPINodes::MPINodes(const MPINodes &other)
         : d(other.d)
{}

/** Destructor */
MPINodes::~MPINodes()
{
    if (d.unique())
    {
        //we are about to completely delete this communicator. We need to
        //let all of the nodes know this!
        d->waitUntilAllFree();
    
        //remove the parent from the nodes, or they will all go
        //silly when they use their destructors!
        for (QList<MPINode>::iterator it = d->free_nodes.begin();
             it != d->free_nodes.end();
             ++it)
        {
            it->d->stopBackend();
            it->d->parent.reset();
        }
    }
}

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

/** Return a node that is not busy. The node will be marked as busy
    until you return it. This function will block until a free
    node is available 

    \throw SireError::unavailable_resource
*/
MPINode MPINodes::getFreeNode()
{
    return d->getFreeNode();
}

/** Return a list of N free nodes. This will block until all N nodes
    are available, and will return all of the nodes at once, marking
    each one as busy
    
    \throw SireError::unavailable_resource
*/
QList<MPINode> MPINodes::getNFreeNodes(int count)
{
    return d->getNFreeNodes(count);
}

/** Return a node that is not busy. The node will be marked as busy
    until you return it. This function will block until a free
    node is available, or until time milliseconds has passed.
    If we run out of time, then a null node is returned.

    \throw SireError::unavailable_resource
*/
MPINode MPINodes::getFreeNode(int time)
{
    return d->getFreeNode();
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
    return d->getNFreeNodes(count);
}

/** Return the total number of nodes in this group */
int MPINodes::nNodes() const
{
    return d->count();
}

/** Return the total number of nodes in this group */
int MPINodes::count() const
{
    return d->count();
}

/** Return the current number of free nodes */
int MPINodes::nFreeNodes() const
{
    return d->nFreeNodes();
}

/** Return the current number of busy nodes */
int MPINodes::nBusyNodes() const
{
    return d->nBusyNodes();
}
