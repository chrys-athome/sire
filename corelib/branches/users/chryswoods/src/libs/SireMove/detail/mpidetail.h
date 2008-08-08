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

#ifndef SIREMOVE_DETAIL_MPIDETAIL_H
#define SIREMOVE_DETAIL_MPIDETAIL_H

#ifdef __SIRE_USE_MPI__
#include <mpi.h>
#endif

#include <QMutex>
#include <QSemaphore>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireMove
{

class MPINodes;

namespace detail
{

class MPINodeData;
class MPINodesData;

/** Private class containing the data for the MPINode class */
class MPINodeData
{
public:
    MPINodeData();
    
    MPINodeData(const MPINodes &communicator, 
                int rank, bool is_master);
    
    MPINodeData(const MPINodeData &other);
    
    ~MPINodeData();

    boost::weak_ptr<MPINodesData> parent;
    int rank;
    QUuid uid;
    bool is_master;
};

/** Private class used by the MPINodes class */
class MPINodesData
{
public:
    static MPINodes construct();
    
    ~MPINodesData();
    
    int count() const;
    
    int nFreeNodes() const;
    int nBusyNodes() const;
    
    void waitUntilAllFree();
    bool waitUntilAllFree(int timeout);
    
    MPINode getFreeNode();
    MPINode getFreeNode(int time);
    
    QList<MPINode> getNFreeNodes(int count);
    QList<MPINode> getNFreeNodes(int count, int time);

    void returnNode(const MPINode &node);

private:
    MPINodesData();

    MPINode _pvt_getNode();

    /** Weak pointer to this object */
    boost::weak_ptr<MPINodesData> this_ptr;

    /** Mutex to protect access to the data of this class */
    QMutex data_mutex;
    
    /** Semaphore used to control access to the nodes */
    QSemaphore sem;

    /** All of the available MPI nodes in this communicator */
    QList<MPINode> free_nodes;
    
    /** All of the busy nodes in this communicator */
    QList< boost::weak_ptr<MPINodeData> > busy_nodes;

    #ifdef __SIRE_USE_MPI__
    /** The MPI communicator used to communicate with 
        the nodes of this group */
    MPI::Comm *mpicomm;
    #endif

    /** The total number of nodes in this communicator */
    int nnodes;

    /** This rank of this node in this communicator */
    int my_rank;
};

}

}

SIRE_END_HEADER

#endif
