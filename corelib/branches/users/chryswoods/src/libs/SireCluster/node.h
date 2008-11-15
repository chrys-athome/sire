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

#ifndef SIRECLUSTER_NODE_H
#define SIRECLUSTER_NODE_H

#include "sireglobal.h"

#include <boost/shared_ptr.hpp>

SIRE_BEGIN_HEADER

namespace SireCluster
{

class Nodes;
class Frontend;
class WorkPacket;

namespace detail
{
class NodePvt;
}

/** This is a Node in a cluster. A Node is a resource that can
    be used to run WorkPackets. A Node is always part of a
    'Nodes' scheduler object, that coordinates the WorkPackets
    that are assigned to the nodes. Alternatively, you can
    grab a node manually from the Nodes object and you can
    assign WorkPackets to it yourself
    
    Essentially, a Node is a means of directing WorkPackets
    to Frontends, so that they can be communicated on to
    Backends that perform the actual work.
    
    @author Christopher Woods
*/
class SIRECLUSTER_EXPORT Node
{

friend class Nodes;

public:
    Node();
    
    Node(const Node &other);
    
    ~Node();
    
    Node& operator=(const Node &other);
    
    bool operator==(const Node &other) const;
    bool operator!=(const Node &other) const;
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<Node>() );
    }
    
    QString toString() const;
    
    Nodes nodes();
    
    bool isHomeless();
    
    bool isLocal();
    
    bool isNull();
    
    QUuid UID();
    
    void startJob(const WorkPacket &workpacket);
    
    void stopJob();
    void abortJob();
    
    void wait();
    bool wait(int timeout);
    
    float progress();
    WorkPacket interimResult();
    
    WorkPacket result();

protected:
    static Node create(const Nodes &nodes, 
                       const Frontend &frontend); // called by Nodes
    
    void evict(); // called by Nodes
    void rehome(const Nodes &nodes); // called by Nodes

    Frontend frontend(); // called by Nodes

private:
    /** Private implementation of Node */
    boost::shared_ptr<detail::NodePvt> d;
};

}

Q_DECLARE_METATYPE( SireCluster::Node )

SIRE_EXPOSE_CLASS( SireCluster::Node )

SIRE_END_HEADER

#endif
