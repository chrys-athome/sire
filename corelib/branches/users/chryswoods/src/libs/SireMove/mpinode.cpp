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

#include <boost/weak_ptr.hpp>

#include "mpinode.h"
#include "mpinodes.h"

#include "detail/mpidetail.h"     // CONDITIONAL_INCLUDE

#include "SireError/errors.h"

using namespace SireMove;
using namespace SireMove::detail;

//////////
////////// Implementation of MPINodeData
//////////

MPINodeData::MPINodeData() : rank(0), is_busy(false), is_master(false)
{}

MPINodeData::MPINodeData(const MPINodes &communicator, 
                         int processor_rank, bool ismaster)
            : parent(communicator.d), rank(processor_rank),
              uid( QUuid::createUuid() ), 
              is_busy(false),
              is_master(ismaster)
{
    BOOST_ASSERT( rank >= 0 );
}

MPINodeData::MPINodeData(const MPINodeData &other)
            : parent(other.parent), rank(other.rank), uid(other.uid),
              is_busy(false), is_master(other.is_master)
{}

MPINodeData::~MPINodeData()
{
    //wait until this node is no longer busy...
    run_mutex.lock();
    data_mutex.lock();
    is_busy = false;
    data_mutex.unlock();
    run_mutex.unlock();

    boost::shared_ptr<MPINodesData> communicator = parent.lock();

    if (communicator.get() != 0)
    {
        //create a null copy of this data and return the node
        boost::shared_ptr<MPINodeData> data_copy( new MPINodeData(*this) );
        
        communicator->returnNode( MPINode(data_copy) );
    }
}

bool MPINodeData::isBusy() const
{
    QMutexLocker lkr( &(const_cast<MPINodeData*>(this)->data_mutex) );
    return is_busy;
}

void MPINodeData::wait()
{
    run_mutex.lock();
    run_mutex.unlock();
}

bool MPINodeData::wait(int time)
{
    if (run_mutex.tryLock(time))
    {
        run_mutex.unlock();
        return true;
    }
    else
        return false;
}

void MPINodeData::lock()
{
    run_mutex.lock();
    data_mutex.lock();
    is_busy = true;
    data_mutex.unlock();
}

void MPINodeData::unlock()
{
    data_mutex.lock();
    
    if (not is_busy)
    {
        data_mutex.unlock();
        return;
    }
    
    is_busy = false;
    data_mutex.unlock();
    run_mutex.unlock();
}

//////////
////////// Implementation of MPINode
//////////

static boost::shared_ptr<MPINodeData> shared_null;

static const boost::shared_ptr<MPINodeData>& getSharedNull()
{
    if (shared_null.get() == 0)
    {
        shared_null.reset( new MPINodeData() );
    }
    
    return shared_null;
}

/** Null constructor */
MPINode::MPINode() : d( getSharedNull() )
{}

/** Construct an MPINode which is part of the passed communicator,
    with the specified rank, and saying whether or not this is 
    the master node of this communicator */
MPINode::MPINode(const MPINodes &communicator, int rank, bool is_master)
        : d( new MPINodeData(communicator, rank, is_master) )
{}

/** Private copy constructor */
MPINode::MPINode(const boost::shared_ptr<detail::MPINodeData> &data)
        : d( data )
{}

/** Copy constructor */
MPINode::MPINode(const MPINode &other)
        : d(other.d)
{}

/** Destructor */
MPINode::~MPINode()
{}

/** Copy assignment operator */
MPINode& MPINode::operator=(const MPINode &other)
{
    d = other.d;
    return *this;
}

/** Comparison operator */
bool MPINode::operator==(const MPINode &other) const
{
    return d.get() == other.d.get();
}

/** Comparison operator */
bool MPINode::operator!=(const MPINode &other) const
{
    return d.get() != other.d.get();
}

/** Return the unique ID for this MPI node */
const QUuid& MPINode::UID() const
{
    return d->uid;
}

/** Return whether or not this node is null */
bool MPINode::isNull() const
{
    return d->parent.expired() or d->uid.isNull();
}

/** Return this node to the pool of unused threads - this returns
    immediately, but the node will only be returned once it is no 
    longer busy, and no other copies of this MPINode exist */
void MPINode::returnNode()
{
    this->operator=( MPINode() );
}

/** Return the communicator in which this node belongs 

    \throw SireError::unavailable_resource
*/
MPINodes MPINode::communicator() const
{
    boost::shared_ptr<MPINodesData> parent = d->parent.lock();

    if (parent.get() == 0)
        throw SireError::unavailable_resource( QObject::tr(
            "A null MPINode doesn't have a communicator!"), CODELOC );
            
    return MPINodes(parent);
}

/** Return the rank of this node in its communicator */
int MPINode::rank() const
{
    return d->rank;
}

/** Return whether this node is the master node for the communicator */
bool MPINode::isMaster() const
{
    return d->is_master;
}

/** Return whether or not this node is busy doing some work */
bool MPINode::isBusy() const
{
    return d->isBusy();
}
    
/** Wait until this node is no longer busy doing work */
void MPINode::wait()
{
    d->wait();
}

/** Wait until this node is no longer busy doing work, or until
    'time' milliseconds have passed. This returns whether or not
    the nodes is not busy */
bool MPINode::wait(int time)
{
    return d->wait(time);
}

void MPINode::lock()
{
    d->lock();
}

void MPINode::unlock()
{
    d->unlock();
}

////////
//////// Implementation of MPINodeWorker
////////

MPINodeWorker::MPINodeWorker() : mpinode(0)
{}

MPINodeWorker::MPINodeWorker(MPINode *node) : mpinode(node)
{
    mpinode->lock();
}

MPINodeWorker::~MPINodeWorker()
{
    if (mpinode)
        mpinode->unlock();
}

MPIPromise< tuple<System,Moves,int> > 
MPINodeWorker::runSim(const System &system, const Moves &moves, 
                      int nmoves, bool record_stats)
{
    //transmit the data to the node

    //create the promise
    
    //start the simulation
    
    //return the promise
    
    return promise;
}
