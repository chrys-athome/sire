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

MPINodeData::MPINodeData() : rank(0), is_master(false)
{}

MPINodeData::MPINodeData(const MPINodes &communicator, 
                         int processor_rank, bool ismaster)
            : parent(communicator.d), rank(processor_rank),
              uid( QUuid::createUuid() ), is_master(ismaster)
{
    BOOST_ASSERT( rank >= 0 );
}

MPINodeData::MPINodeData(const MPINodeData &other)
            : parent(other.parent), rank(other.rank), uid(other.uid)
{}

MPINodeData::~MPINodeData()
{
    boost::shared_ptr<MPINodesData> communicator = parent.lock();

    if (communicator.get() != 0)
    {
        //create a null copy of this data and return the node
        boost::shared_ptr<MPINodeData> data_copy( new MPINodeData(*this) );
        
        communicator->returnNode( MPINode(data_copy) );
    }
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
bool MPINode::isBusy()
{
    return false;
}
    
/** Wait until this node is no longer busy doing work */
void MPINode::wait()
{
}

/** Wait until this node is no longer busy doing work, or until
    'time' milliseconds have passed. This returns whether or not
    the nodes is not busy */
bool MPINode::wait(int time)
{
    return true;
}
