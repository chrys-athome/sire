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

#include "Siren/handle.h"

#include "sireglobal.h"

#include <QUuid>

SIRE_BEGIN_HEADER

class QUuid;

namespace SireCluster
{

class WorkQueue;
class Promise;
class Promises;
class Nodes;

/** Node provides the frontend to a single resource
    that may be used to run WorkPacket jobs. These jobs are
    scheduled via the WorkQueue that is handled by this
    Nodes object, from which Promise or Promises objects
    are returned from which the results of the job
    can be obtained
    
    @author Christopher Woods
*/
class SIRECLUSTER_EXPORT Node 
        : public Siren::ImplementsHandle< Node,Siren::Handles<WorkQueue> >
{
public:
    Node();
    
    Node(const Node &other);
    
    ~Node();
    
    Node& operator=(const Node &other);
    
    bool operator==(const Node &other) const;
    bool operator!=(const Node &other) const;
    
    QString toString() const;
    uint hashCode() const;
    
    Promise submit(const WorkPacket &workpacket);
    Promises submit(const QList<WorkPacket> &workpackets);

    bool isBusy() const;
    bool isFree() const;

    static Nodes merge(Nodes &nodes0, Nodes &nodes1);
    static Nodes merge(Node &node0, Node &node1);
    static Nodes merge(Nodes &nodes0, Node &node1);
    static Nodes merge(Node &node0, Nodes &nodes1);

protected:
    friend class Cluster;
    Node(WorkQueue *workqueue);
};

}

Q_DECLARE_METATYPE( SireCluster::Node )

SIRE_EXPOSE_CLASS( SireCluster::Node )

SIRE_END_HEADER

#endif
