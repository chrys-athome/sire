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

#include <QVector>

#include "nodes.h"
#include "node.h"
#include "workpacket.h"
#include "promise.h"
#include "promises.h"

#include "resources/workqueue.h"  // CONDITIONAL_INCLUDE

#include "Siren/objptr.hpp"
#include "Siren/errors.h"

using namespace SireCluster;
using namespace SireCluster::resources;
using namespace Siren;

static const RegisterHandle<Nodes> r_nodes;

/** Construct a set of nodes that only contain the local thread */
Nodes::Nodes() : ImplementsHandle< Nodes,Handles<WorkQueue> >()
{}

/** Construct to hold the resource in 'node' */
Nodes::Nodes(const Node &node) : ImplementsHandle< Nodes,Handles<WorkQueue> >(node)
{}

/** Construct to handle the nodes in the passed work queue */
Nodes::Nodes( WorkQueue *workqueue )
      : ImplementsHandle< Nodes,Handles<WorkQueue> >(workqueue)
{}

/** Copy constructor */
Nodes::Nodes(const Nodes &other) : ImplementsHandle< Nodes,Handles<WorkQueue> >(other)
{}

/** Destructor */
Nodes::~Nodes()
{}

/** Copy assignment operator */
Nodes& Nodes::operator=(const Nodes &other)
{
    Handles<WorkQueue>::operator=(other);
    return *this;
}

/** Comparison operator */
bool Nodes::operator==(const Nodes &other) const
{
    return Handles<WorkQueue>::operator==(other);
}

/** Comparison operator */
bool Nodes::operator!=(const Nodes &other) const
{
    return Handles<WorkQueue>::operator!=(other);
}

uint Nodes::hashCode() const
{
    return qHash(Nodes::typeName());
}

/** Return whether this Nodes object only contains the 
    local thread */
bool Nodes::isLocalOnly() const
{
    return Handles<WorkQueue>::isNull();
}

/** Return a string representation of the nodes */
QString Nodes::toString() const
{
    if (isNull())
        return QObject::tr("Nodes( local_thread )");
    else 
        return QObject::tr("Nodes( %1 )").arg( resource().toString() );
}

/** Submit the job held in 'workpacket' to be run on this node,
    returning a Promise that will hold the result (and can be
    used to cancel, delay, stop or abort the job) */
Promise Nodes::submit(const WorkPacket &workpacket)
{
    if (isNull())
        return Promise::runLocal(workpacket);
    else
    {
        HandleLocker lkr(*this);
        return resource().submit(workpacket);
    }
}

/** Submit all of the jobs in 'workpackets' to be run on this
    node. There is no guarantee about the order in which the jobs
    will be run. The set of promises (ordered in the same order
    as the workpackets) is returned, which will hold the results
    of the jobs (and can be used to cancel, delay, stop or abort
    specific jobs or all of the jobs) */
Promises Nodes::submit(const QVector<WorkPacketPtr> &workpackets)
{
    if (isNull())
        return Promises::runLocal(workpackets);
    else
    {
        HandleLocker lkr(*this);
        return resource().submit(workpackets);
    }
}

/** Submit all of the jobs in 'workpackets' to be run on this
    node. There is no guarantee about the order in which the jobs
    will be run. The set of promises (ordered in the same order
    as the workpackets) is returned, which will hold the results
    of the jobs (and can be used to cancel, delay, stop or abort
    specific jobs or all of the jobs) */
Promises Nodes::submit(const QList<WorkPacketPtr> &workpackets)
{
    return this->submit( workpackets.toVector() );
}

/** Return the pair of (busy nodes, free nodes) */
QPair<int,int> Nodes::nBusyFree() const
{
    if (isNull())
        return QPair<int,int>(0,1);
    else 
    {
        HandleLocker lkr(*this);
        return resource().nBusyFree();
    }
}

/** Return the number of free resources */
int Nodes::nFree() const
{
    return nBusyFree().second;
}

/** Return the number of busy resources */
int Nodes::nBusy() const
{
    return nBusyFree().first;
}

/** Return the total number of resources in this cluster of nodes */
int Nodes::nNodes() const
{
    QPair<int,int> b = this->nBusyFree();
    return b.first + b.second;
}

/** Return the total number of resources in this cluster of nodes */
int Nodes::count() const
{
    return nNodes();
}

/** Merge the two sets of nodes in 'nodes0' and 'nodes1' together.
    This merges the work queues, and puts all of the pending jobs
    into the queue of the new Nodes object (which is returned).
    The passed nodes are cleared */
Nodes Nodes::merge(Nodes nodes0, Nodes nodes1)
{
    if (nodes0.isLocalOnly())
        return nodes1;

    else if (nodes1.isLocalOnly())
        return nodes0;
    
    else if (nodes0 == nodes1)
        return nodes0;
    
    else
    {
        HandleLocker lkr0(nodes0);
        HandleLocker lkr1(nodes1);
        
        return Nodes( nodes0.resource().merge(nodes1.resource()) );
    }
}

/** Merge the two sets of nodes in 'nodes0' and 'nodes1' together.
    This merges the work queues, and puts all of the pending jobs
    into the queue of the new Nodes object (which is returned).
    The passed nodes are cleared */
Nodes Nodes::merge(Node node0, Node node1)
{
    return Nodes::merge( Nodes(node0), Nodes(node1) );
}

/** Merge the two sets of nodes in 'nodes0' and 'nodes1' together.
    This merges the work queues, and puts all of the pending jobs
    into the queue of the new Nodes object (which is returned).
    The passed nodes are cleared */
Nodes Nodes::merge(Nodes nodes0, Node node1)
{
    return Nodes::merge( Nodes(nodes0), Nodes(node1) );
}

/** Merge the two sets of nodes in 'nodes0' and 'nodes1' together.
    This merges the work queues, and puts all of the pending jobs
    into the queue of the new Nodes object (which is returned).
    The passed nodes are cleared */
Nodes Nodes::merge(Node node0, Nodes nodes1)
{
    return Nodes::merge( Nodes(node0), Nodes(nodes1) );
}
