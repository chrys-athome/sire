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

#ifndef SIRECLUSTER_CLUSTER_H
#define SIRECLUSTER_CLUSTER_H

#include <QList>
#include <QHash>
#include <QUuid>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireCluster
{

class Nodes;

/** This static class provides the global registry for all nodes in the cluster.
    A node is defined as a resource that can run a WorkPacket. A node
    consists of a Backend (the object in which the WorkPacket is 
    run) and a Frontend (the object that allows the node to communicate
    with the Backend)
    
    @author Christopher Woods
*/
class SIRECLUSTER_EXPORT Cluster
{
public:
    static QList<QUuid> UIDs();
    static QList<QUuid> localUIDs();

    static QHash<QUuid,QString> descriptions();
    static QHash<QUuid,QString> localDescriptions();

    static bool isLocal(const QUuid &uid);

    static void start();

    static void shutdown();

    static void wait();
    
    static bool isRunning();

    static Nodes getNode();
    static Nodes getNode(int timeout);

    static Nodes getLocalNode();
    static Nodes getLocalNode(int timeout);

    static Nodes getNode(const QUuid &uid);
    static Nodes getNode(const QUuid &uid, int timeout);

    static Nodes getLocalNode(const QUuid &uid);
    static Nodes getLocalNode(const QUuid &uid, int timeout);
    
    static Nodes getNode(const QString &description);
    static Nodes getNode(const QString &description, int timeout);
    
    static Nodes getLocalNode(const QString &description);
    static Nodes getLocalNode(const QString &description, int timeout);
    
    static Nodes getNodes(int nnodes);
    static Nodes getNodes(int nnodes, int timeout);
    
    static Nodes getLocalNodes(int nnodes);
    static Nodes getLocalNodes(int nnodes, int timeout);
    
    static Nodes getNodes(const QList<QUuid> &uids);
    static Nodes getNodes(const QList<QUuid> &uids, int timeout);
    
    static Nodes getLocalNodes(const QList<QUuid> &uids);
    static Nodes getLocalNodes(const QList<QUuid> &uids, int timeout);
    
    static Nodes getNodes(const QString &description, int nnodes);
    static Nodes getNodes(const QString &description, int nnodes, int timeout);
    
    static Nodes getLocalNodes(const QString &description, int nnodes);
    static Nodes getLocalNodes(const QString &description, int nnodes, int timeout);
};

}

SIRE_EXPOSE_CLASS( SireCluster::Cluster )

SIRE_END_HEADER

#endif
