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

#include "node.h"
#include "nodes.h"
#include "promise.h"
#include "promises.h"
#include "workpacket.h"

#include "resources/workqueue.h"  // CONDITIONAL_INCLUDE

#include "Siren/objptr.hpp"
#include "Siren/errors.h"

using namespace SireCluster;
using namespace SireCluster::resources;
using namespace Siren;

static const RegisterHandle<Node> r_node;

/** Construct a single node, which runs work using the local thread */
Node::Node() : ImplementsHandle< Node,Handles<WorkQueue> >()
{}

/** Internal constructor used to construct a node using the passed work queue */
Node::Node(WorkQueue *workqueue) : ImplementsHandle< Node,Handles<WorkQueue> >(workqueue)
{
    QPair<int,int> r = resource().nBusyFree();

    if (r.first + r.second != 1)
        throw Siren::program_bug( QObject::tr(
                "It is a mistake to create a Node that does not contain "
                "just a single resource! (%1)").arg(r.first + r.second), CODELOC );
}

/** Copy constructor */
Node::Node(const Node &other) : ImplementsHandle< Node,Handles<WorkQueue> >(other)
{}

/** Destructor */
Node::~Node()
{}

/** Copy assignment operator */
Node& Node::operator=(const Node &other)
{
    Handles<WorkQueue>::operator=(other);
    return *this;
}

/** Comparison operator */
bool Node::operator==(const Node &other) const
{
    return Handles<WorkQueue>::operator==(other);
}

/** Comparison operator */
bool Node::operator!=(const Node &other) const
{
    return Handles<WorkQueue>::operator!=(other);
}

uint Node::hashCode() const
{
    return qHash( Node::typeName() );
}

/** Return a string representation of the node */
QString Node::toString() const
{
    if (isNull())
        return QObject::tr("Node( local_thread )");
    else
        return QObject::tr("Node( %1 )").arg( resource().toString() );
}

/** Submit the job held in 'workpacket' to be run on this node,
    returning a Promise that will hold the result (and can be
    used to cancel, delay, stop or abort the job) */
Promise Node::submit(const WorkPacket &workpacket)
{
    if (isNull())
    {
        //run the job in the local thread
        return Promise::runLocal(workpacket);
    }
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
Promises Node::submit(const QVector<WorkPacketPtr> &workpackets)
{
    if (isNull())
    {
        //run everything in the local thread
        return Promises::runLocal(workpackets);
    }
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
Promises Node::submit(const QList<WorkPacketPtr> &workpackets)
{
    return this->submit( workpackets.toVector() );
}

/** Return whether or not this node is busy */
bool Node::isBusy() const
{
    if (isNull())
        return false;
    else
    {
        HandleLocker lkr(*this);
        return resource().nBusyFree().first > 0;
    }
}

/** Return whether or not this node is free */
bool Node::isFree() const
{
    if (isNull())
        return true;
    else
    {
        HandleLocker lkr(*this);
        return resource().nBusyFree().second > 0;
    }
}

/** Merge the passed nodes together, returning the combined resource. */
Nodes Node::merge(Nodes nodes0, Nodes nodes1)
{
    return Nodes::merge(nodes0, nodes1);
}

/** Merge the passed nodes together, returning the combined resource. */
Nodes Node::merge(Node node0, Node node1)
{
    return Nodes::merge(node0, node1);
}

/** Merge the passed nodes together, returning the combined resource. */
Nodes Node::merge(Nodes nodes0, Node node1)
{
    return Nodes::merge(nodes0, node1);
}

/** Merge the passed nodes together, returning the combined resource. */
Nodes Node::merge(Node node0, Nodes nodes1)
{
    return Nodes::merge(node0, nodes1);
}
