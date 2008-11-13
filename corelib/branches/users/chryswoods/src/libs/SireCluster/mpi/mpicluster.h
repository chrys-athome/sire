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

#ifndef SIRECLUSTER_MPI_MPICLUSTER_H
#define SIRECLUSTER_MPI_MPICLUSTER_H

#ifdef __SIRE_USE_MPI__

#include "sireglobal.h"

#include <QUuid>
#include <QList>

SIRE_BEGIN_HEADER

namespace SireCluster
{

class Frontend;
class Backend;

namespace detail
{
class MPIClusterPvt;
}

/** This class provides the global interface to all of the
    MPI nodes in the cluster (and, on the root node, the 
    global registry of all nodes available via MPI)
    
    This is a private class which is only available internally
    to SireCluster if MPI is available

    @author Christopher Woods
*/
class MPICluster
{
public:
    ~MPICluster();

    static void registerBackend(const Backend &backend);
    
    static Frontend getFrontend(const QUuid &uid);
    
    static QList<QUuid> UIDs();
    
    static void shutdown();

private:
    MPICluster();

    static MPICluster& cluster();

    void start();

    /** Private implementation of MPICluster */
    boost::shared_ptr<detail::MPIClusterPvt> d;
};

}

SIRE_END_HEADER

#endif // __SIRE_USE_MPI__

#endif
