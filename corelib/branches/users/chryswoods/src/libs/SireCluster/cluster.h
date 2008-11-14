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

#include "sireglobal.h"

#include <QUuid>
#include <QList>

#include <boost/shared_ptr.hpp>

SIRE_BEGIN_HEADER

namespace SireCluster
{

class Backend;
class Frontend;

/** This class provides the global registry for all nodes in the cluster.
    A node is defined as a resource that can run a WorkPacket. A node
    consists of a Backend (the object in which the WorkPacket is 
    run) and a Frontend (the object that allows the node to communicate
    with the Backend)
    
    @author Christopher Woods
*/
class SIRECLUSTER_EXPORT Cluster
{
public:
    static void registerBackend(const Backend &backend);
    
    static Frontend getFrontend(const QUuid &uid);

    static QList<QUuid> localUIDs();
    
    static QList<QUuid> UIDs();

    static int getRank();
    
    static void start();

    static void shutdown();

    static void wait();
    
    static bool isRunning();

    static bool supportsMPI();

    static void exec();
};

}

SIRE_EXPOSE_CLASS( SireCluster::Cluster )

SIRE_END_HEADER

#endif
