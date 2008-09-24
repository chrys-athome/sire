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

#ifndef SIREMPI_DETAIL_MPIDETAIL_H
#define SIREMPI_DETAIL_MPIDETAIL_H

#ifdef __SIRE_USE_MPI__
#include <mpi.h>
#endif

#include <QThread>
#include <QMutex>
#include <QSemaphore>
#include <QWaitCondition>
#include <QUuid>

#include "SireError/exception.h"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/noncopyable.hpp>

SIRE_BEGIN_HEADER

namespace SireMPI
{

class MPINode;
class MPINodes;

namespace detail
{

class MPINodeData;
class MPINodesData;

enum { MPIWORKER_START        = 0x0001,
       MPIWORKER_STOP         = 0x0002,
       MPIWORKER_ABORT        = 0x0004,
       MPIWORKER_PROGRESS     = 0x0008,
       MPIWORKER_INTERIM      = 0x000f,
       MPIWORKER_RESULT       = 0x0010 };

/** Private class containing the data for the MPINode class */
class MPINodeData : private QThread
{
public:
    MPINodeData();
    
    MPINodeData(const boost::shared_ptr<MPINodesData> &communicator, 
                int rank, bool is_master);
    
    MPINodeData(const MPINodeData &other);
    
    ~MPINodeData();

    MPINodeData& operator=(const MPINodeData &other);

    void startBackend();
    void stopBackend();

    bool isBusy() const;
    
    bool wasAborted() const;
    
    void wait();
    bool wait(int time);
    
    void lock();
    void unlock();
    
    MPIPromise start(const MPIWorker &worker);
    void stop();
    void abort();
    
    void getProgress();
    void getInterimResult();

    void sendMessage(int message);

    boost::weak_ptr<MPINodesData> parent;
    boost::weak_ptr<MPINodeData> self_ptr;
    
    QMutex data_mutex;

    QMutex start_mutex;
    QWaitCondition start_waiter;

    /** Unique ID for this node */
    QUuid uid;
    
    /** This promise holds the current calculation on this node, or
        the result of the last calculation */
    MPIPromise mpipromise;
    
    #ifdef __SIRE_USE_MPI__
    int mpirank;
    #endif

    bool is_aborted;
    bool is_master;

protected:
    void run();
};

/** Private class used by the MPINodes class */
class MPINodesData
{

friend class MPINodeData;   // so can see the MPI::Comm pointer

public:
    static boost::shared_ptr<MPINodesData> construct();
    
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

    #ifdef __SIRE_USE_MPI__
        MPI::Comm* mpiCommunicator();
        int mpiTag();
        int masterRank();
    #endif

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

        /** This rank of this master node in the communicator */
        int my_mpirank;

        /** The tag number that is used for all communications
            by nodes using this communicator */
        int mpitag;

        /** The total number of nodes in this communicator */
        int nnodes;
    #endif
};

}

}

SIRE_END_HEADER

#endif
