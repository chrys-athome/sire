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

#include <QWaitCondition>
#include <QMutex>

#include <QUuid>

#include "node.h"
#include "frontend.h"
#include "nodes.h"
#include "workpacket.h"

#include "SireError/errors.h"

using namespace SireCluster;

using boost::shared_ptr;

namespace SireCluster
{
namespace detail
{

/** Private implementation of Node */
class NodePvt
{
public:
    NodePvt(const Frontend &f, const NodesPtr &n)
          : frontend(f), nodes(n)
    {}
    
    ~NodePvt()
    {
        nodes.returnFrontend(frontend);
    }
    
    Frontend frontend;
    NodesPtr nodes;
};

} //end of namespace detail 
} //end of namespace SireCluster

using namespace SireCluster::detail;

/** Null constructor */
Node::Node()
{}

/** Copy constructor */
Node::Node(const Node &other) : d(other.d)
{}

/** Destructor */
Node::~Node()
{}

/** Internal function called by 'Nodes' that is used to create a Node
    that is part of 'Nodes' and uses the Frontend 'frontend' */
Node Node::create(const Nodes &nodes, const Frontend &frontend)
{
    Node node;
    node.d.reset( new NodePvt(frontend, nodes) );
    
    return node;
}
    
/** Internal function called by 'Nodes' that tells this node that
    it is no longer part of its parent. This is called only after
    the Nodes object itself has removed the Node from it's registry */
void Node::evict()
{
    if (not this->isNull())
    {
        d->nodes.reset();
    }
}

/** Internal function called by 'Nodes' that tells this node that
    it is being moved to be part of 'nodes'. This is called only
    after the nodes object has updated it's registry */
void Node::rehome(const Nodes &nodes)
{
    if (not this->isNull())
    {
        Nodes old_nodes = d->nodes.lock();
        
        if (not old_nodes.isEmpty())
        {
            old_nodes.remove(*this);
        }
        
        d->nodes = nodes;
    }
}

/** Return the frontend of this node - this will be null if 
    this is a null node */
Frontend Node::frontend()
{
    if (not this->isNull())
    {
        return d->frontend;
    }
    else
        return Frontend();
}

/** Copy assignment operator */
Node& Node::operator=(const Node &other)
{
    d = other.d;
    return *this;
}

/** Comparison operator */
bool Node::operator==(const Node &other) const
{
    return d.get() == other.d.get();
}

/** Comparison operator */
bool Node::operator!=(const Node &other) const
{
    return not this->operator==(other);
}

/** Return a string representation of this node */
QString Node::toString() const
{
    Node *nonconst_this = const_cast<Node*>(this);

    QUuid uid = nonconst_this->UID();
    
    if (uid.isNull())
        return QObject::tr( "Node::null" );
        
    else if (nonconst_this->isHomeless())
        return QObject::tr( "Node( %1  **HOMELESS** )" ).arg(uid.toString());

    else
        return QObject::tr( "Node( %1 )" ).arg(uid.toString());
}

/** Return whether or not this node is null */
bool Node::isNull()
{
    return d.get() == 0;
}

/** Return whether or not this node is local to this process */
bool Node::isLocal()
{
    if (not this->isNull())
        return d->frontend.isLocal();
    else
        return true;
}

/** Return whether or not this Node object is homeless
    (is not part of any Nodes set). A homeless node can
    finish any job that has started, but it is not allowed
    to start any more jobs */
bool Node::isHomeless()
{
    if (not this->isNull())
        return d->nodes.expired();
    else
        return true;
}

/** Return the unique ID of the Node */
QUuid Node::UID()
{
    if (not this->isNull())
        return d->frontend.UID();
    else
        return QUuid();
}

/** Return the nodes object that this node belongs to. In some rare
    circumstances this node may be homeless, in which case 
    an empty set of nodes will be returned */
Nodes Node::nodes()
{
    if (not this->isNull())
        return d->nodes.lock();
    else
        return Nodes();
}

/** Start the job in the WorkPacket 'workpacket' on this node 

    \throw SireError::unavailable_resource
*/
void Node::startJob(const WorkPacket &workpacket)
{
    if (this->isNull())
        throw SireError::unavailable_resource( QObject::tr(
            "You cannot start a job on a null Node."), CODELOC );

    if (this->isHomeless())
        throw SireError::unavailable_resource( QObject::tr(
            "You cannot start a new job on a homeless Node (%1). "
            "Add this node to a Nodes scheduler object before you "
            "try to run this job again.")
                .arg(this->UID()), CODELOC );
        
    d->frontend.startJob(workpacket);
}

/** Stop any running job on this node - this does not block */
void Node::stopJob()
{
    if (not this->isNull())
        d->frontend.stopJob();
}

/** Abort any running job on this node - this does not block */
void Node::abortJob()
{
    if (not this->isNull())
        d->frontend.abortJob();
}

/** Wait for the Node to stop work */
void Node::wait()
{
    if (not this->isNull())
        d->frontend.wait();
}

/** Wait for the Node to stop work (or until 'timeout' milliseconds
    have passed) - this returns whether or not the Node has stopped
    work */
bool Node::wait(int timeout)
{
    if (not this->isNull())
        return d->frontend.wait(timeout);
    else
        return true;
}

/** Return the progress of the Node on the current WorkPacket */
float Node::progress()
{
    if (not this->isNull())
        return d->frontend.progress();
    else
        return Frontend().progress();
}

/** Return the work in progress */
WorkPacket Node::interimResult()
{
    if (not this->isNull())
        return d->frontend.interimResult();
    else
        return Frontend().interimResult();
}

/** Return the final result - this blocks until the 
    result is ready */
WorkPacket Node::result()
{
    if (not this->isNull())
        return d->frontend.result();
    else
        return Frontend().result();
}
