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

#include "node.h"

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
};

} //end of namespace detail 
} //end of namespace SireCluster

/** Null constructor */
Node::Node()
{}

/** Copy constructor */
Node::Node(const Node &other) : d(other.d)
{}

/** Destructor */
Node::~Node()
{}

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

/** Return whether or not this node is null */
bool Node::isNull() const
{
    return d.get() == 0;
}

/** Return whether or not this node is local to this process */
bool Node::isLocal() const
{
    if (not this->isNull())
        return d->frontend.isLocal();
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

/** Start the job in the WorkPacket 'workpacket' on this node */
void Node::startJob(const WorkPacket &workpacket)
{
    if (not this->isNull())
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
        return frontend.wait(timeout);
    else
        return true;
}

/** Return the progress of the Node on the current WorkPacket */
float Node::progress()
{
    if (not this->isNull())
        return frontend.progress();
    else
        return Frontend().progress();
}

/** Return the work in progress */
WorkPacket Node::interimResult()
{
    if (not this->isNull())
        return frontend.interimResult();
    else
        return Frontend().interimResult();
}

/** Return the final result - this blocks until the 
    result is ready */
WorkPacket Node::result()
{
    if (not this->isNull())
        return frontend.result();
    else
        return Frontend().result();
}
