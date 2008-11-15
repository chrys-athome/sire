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

#ifndef SIRECLUSTER_NODES_H
#define SIRECLUSTER_NODES_H

#include "sireglobal.h"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

SIRE_BEGIN_HEADER

namespace SireCluster
{

class Node;
class NodesPtr;

namespace detail
{
class NodesPvt;
}

/** This class holds, and schedules, a collection of Node objects.
    
    @author Christopher Woods
*/
class SIRECLUSTER_EXPORT Nodes
{

friend class NodesPtr;

public:
    Nodes();
    
    Nodes(const Nodes &other);
    
    ~Nodes();
    
    Nodes& operator=(const Nodes &other);
    
    bool operator==(const Nodes &other) const;
    bool operator!=(const Nodes &other) const;
    
    Node getFreeNode();
    QList<Node> getNFreeNodes(int n);
    
    Node getFreeNode(int timeout);
    QList<Node> getNFreeNodes(int n, int timeout);
    
    void waitUntilAllFree();
    bool waitUntilAllFree(int timeout);
    
    int nFreeNodes() const;
    int nBusyNodes() const;
    int nNodes() const;
    int count() const;

    void removeAll();

private:
    /** Private implementation of the Nodes */
    boost::shared_ptr<detail::NodesPvt> d;
};

/** This class holds a weak pointer to the Nodes */
class NodesPtr
{
public:
    NodesPtr();
    NodesPtr(const Nodes &nodes);
    
    NodesPtr(const NodesPtr &other);
    
    ~NodesPtr();
    
    NodesPtr& operator=(const NodesPtr &other);
    
    Nodes operator*() const;

    void returnFrontend(const Frontend &frontend);

private:
    /** Weak pointer to the nodes */
    boost::weak_ptr<detail::NodesPvt> d;
};

}

Q_DECLARE_METATYPE( SireCluster::Nodes )

SIRE_EXPOSE_CLASS( SireCluster::Nodes )

SIRE_END_HEADER

#endif
