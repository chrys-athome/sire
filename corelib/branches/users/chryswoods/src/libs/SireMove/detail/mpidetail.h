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

#include <QThread>
#include <QMutex>
#include <QSemaphore>
#include <QWaitCondition>
#include <QUUid>

#include "SireError/exception.h"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/noncopyable.hpp>

SIRE_BEGIN_HEADER

namespace SireMove
{

class System;
class Moves;
class MovesBase;

class MPINode;
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

    bool isBusy() const;
    
    void wait();
    bool wait(int time);
    
    void lock();
    void unlock();

    boost::weak_ptr<MPINodesData> parent;
    
    QMutex data_mutex;
    QMutex run_mutex;
    
    QUuid uid;
    
    #ifdef __SIRE_USE_MPI__
    int mpirank;
    #endif
    
    bool is_busy;
    bool is_master;
};

/** Private class used by MPINode and MPIPromise */
class MPIWorker : private QThread, public boost::noncopyable
{
public:
    ~MPIWorker();
    
    static boost::shared_ptr<MPIWorker> 
                runSim(MPINodeData *nodedata,
                       const System &system, const MovesBase &moves,
                       int nmoves, bool record_stats);

    #ifdef __SIRE_USE_MPI__
        static void backendLoop(MPI::Comm *mpicomm, int mpimaster);
    #endif

    bool hasFinished();
    bool isError();
    bool isAborted();

    void stop();
    void abort();

    void wait();
    bool wait(int ms);

    QByteArray interimResult();

    QByteArray result();

    double progress();

private:
    #ifdef __SIRE_USE_MPI__
        static void sendReply(MPI::Comm *mpicomm, int mpimaster, int message,
                              const QByteArray &arguments);
                             
        static void sendError(MPI::Comm *mpicomm, int mpimaster,
                              const SireError::exception &e);
                                
        static bool runSim_0(MPI::Comm *mpicomm, int mpimaster, 
                             QDataStream &ds);
                             
        static bool actOnMessage(MPI::Comm *mpicomm, int mpimaster, 
                                 const QByteArray &message);
    #endif

    MPIWorker();
    
    void run();
    
    void sendMessage(int *message, int size=1, bool blocking=false);
    void sendMessage(const QByteArray &data, bool blocking=false);
    
    void receiveMessage(int *message, int size=1);
    void receiveMessage(QByteArray &data, int size);
    
    void setError(const QByteArray &error_data);
    void setResult(const QByteArray &result_data);
    void setProgress(const QByteArray &progress_data);
    void setInterim(const QByteArray &result_data);
    void setAborted();
    
    QByteArray processResult();
    
    enum { MPIWORKER_ERROR    = 1,
           MPIWORKER_FINISHED = 2,
           MPIWORKER_PROGRESS = 4,
           MPIWORKER_INTERIM  = 8,
           MPIWORKER_ABORTED  = 16,
           MPIWORKER_STOPPED  = 32 };
    
    enum { MPIWORKER_IS_NULL     = 0,
           MPIWORKER_IS_STARTING = 1,
           MPIWORKER_IS_BUSY     = 2,
           MPIWORKER_IS_ERROR    = 4,
           MPIWORKER_IS_ABORTED  = 8,
           MPIWORKER_IS_FINISHED = 16 };
    
    /** Mutex used to serialise access to the data */
    QMutex data_mutex;

    /** Mutex and wait condition used to start the background thread */
    QMutex start_mutex;
    QWaitCondition start_waiter;

    /** Wait condition used to get the progress of the work */
    QWaitCondition progress_waiter;
    
    /** Wait condition used to get the interim results of the work */
    QWaitCondition interim_waiter;
    
    /** Wait condition used to get the latest result of the work */
    QWaitCondition result_waiter; 

    /** Pointer to the data of the node that is being worked on */
    MPINodeData *nodedata;

    /** The current progress of the work */
    double current_progress;
    
    /** Current status of this worker */
    int current_status;
    
    /** A databuffer used by the MPI communication system */
    QByteArray databuffer;
};

/** Private class used by the MPINodes class */
class MPINodesData
{

friend class MPINodeData;   // so can see the MPI::Comm pointer

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

    #ifdef __SIRE_USE_MPI__
        MPI::Comm* mpiCommunicator();
        int masterRank();
    #endif

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

        /** This rank of this master node in the communicator */
        int my_mpirank;

        /** The total number of nodes in this communicator */
        int nnodes;
    #endif
};

}

}

SIRE_END_HEADER

#endif
