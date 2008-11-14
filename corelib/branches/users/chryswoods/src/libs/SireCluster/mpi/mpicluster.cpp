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

#ifdef __SIRE_USE_MPI__

#include <mpi.h>   //mpich requires that mpi.h is included first

#include <QHash>
#include <QMutex>
#include <QUuid>

#include "SireCluster/cluster.h"
#include "SireCluster/frontend.h"
#include "SireCluster/backend.h"

#include "mpicluster.h"
#include "messages.h"
#include "sendqueue.h"
#include "receivequeue.h"

#include "SireError/errors.h"

#include <QDebug>

using namespace SireCluster;
using namespace SireCluster::MPI;

#ifndef HAVE_LSEEK64
    //////
    ////// add an lseek64 function stub to fill a function
    ////// that is missing - mpich needs lseek64 to be
    ////// defined, even if it is not available! Otherwise
    ////// dlopen errors as the symbol can't be found
    //////
    extern "C"
    {
        int lseek64(int fd, int offset, int whence)
        {
            throw SireError::program_bug( QObject::tr(
                "MPI implementation is calling lseek64 which is not supported "
                "on OS X (Leopard - 32bit)"), CODELOC );
            
            return 0;
        }
    }
#endif // HAVE_LSEEK64

/** Private implementation of MPICluster */
class MPIClusterPvt
{
public:
    MPIClusterPvt();
    
    ~MPIClusterPvt();
    
    /** Mutex to protect access to the data of this cluster */
    QMutex datamutex;
    
    /** The UIDs of all backends, together with the rank of the 
        process that contains that backend */
    QHash<QUuid,int> backend_registry;

    /** The send message event loop */
    SendQueue *send_queue;
    
    /** The receive message event loops */
    ReceiveQueue *receive_queue;
};

Q_GLOBAL_STATIC( MPIClusterPvt, globalCluster );

static void ensureMPIStarted()
{
    if (not ::MPI::Is_initialized())
    {
        int argc = 0;
        char **argv = 0;
        ::MPI::Init(argc, argv);
    }
}

/** Construct the global cluster */
MPIClusterPvt::MPIClusterPvt() : send_queue(0), receive_queue(0)
{
    //create the private send and receive communicators
    ::ensureMPIStarted();
        
    ::MPI::Intracomm send_comm, recv_comm;
        
    if ( MPICluster::isMaster() )
    {
        //create the send, then receive communicators
        send_comm = ::MPI::COMM_WORLD.Clone();
        recv_comm = ::MPI::COMM_WORLD.Clone();
    }
    else
    {
        //must be the other way around (as all other nodes
        //listen to the master)
        recv_comm = ::MPI::COMM_WORLD.Clone();
        send_comm = ::MPI::COMM_WORLD.Clone();
    }
        
    //create and start the event loops
    send_queue = new SendQueue(send_comm);
    receive_queue = new ReceiveQueue(recv_comm);
        
    send_queue->start();
    receive_queue->start();
}

/** Destructor */
MPIClusterPvt::~MPIClusterPvt()
{
    if (send_queue)
    {
        send_queue->stop();
        send_queue->wait();
        delete send_queue;
    }
    
    if (receive_queue)
    {
        receive_queue->stop();
        receive_queue->wait();
        delete receive_queue;
    }
}

/** Start the MPI backend */
void MPICluster::start()
{
    ::ensureMPIStarted();
    globalCluster();
}

/** Return the rank of this process in the MPI cluster */
int MPICluster::getRank()
{
    ::ensureMPIStarted();
    return ::MPI::COMM_WORLD.Get_rank();
}

/** Return the rank of the master process */
int MPICluster::master()
{
    return 0;
}

/** Return whether or not this is the master process */
bool MPICluster::isMaster()
{
    return MPICluster::getRank() == MPICluster::master();
}

/** Send the message 'message' (the destination is available
    in message.destination()) */
void MPICluster::send(const Message &message)
{
    if (message.isNull())
        return;
        
    globalCluster()->send_queue->send(message);
}

/** Receive the message 'message' */
void MPICluster::received(const Message &message)
{
    if (message.isNull())
        return;
        
    globalCluster()->receive_queue->received(message);
}

/** Call this function on the master MPI process to register
    that the backend with UID 'uid' is on the MPI process
    with rank 'rank' */
void MPICluster::registerBackend(int rank, const QUuid &uid)
{
    if ( not MPICluster::isMaster() )
    {
        //why is this message here - it should have been
        //sent to the master!
        throw SireError::program_bug( QObject::tr(
            "A request to register the node with UID %1 on process %2 "
            "has ended up on process %3, while the master is process %4.")
                .arg(uid.toString())
                .arg(rank)
                .arg( MPICluster::getRank() )
                .arg( MPICluster::master() ),
                    CODELOC );
    }

    QMutexLocker lkr( &(globalCluster()->datamutex) );
    
    if (not globalCluster()->backend_registry.contains(uid))
    {
        globalCluster()->backend_registry.insert(uid, rank);
    }
}    

/** Register the local Backend 'backend' with the MPI cluster so that
    it can be connected to by any MPI-connected node */
void MPICluster::registerBackend(const Backend &backend)
{
    if ( MPICluster::isMaster() )
    {
        MPICluster::registerBackend( MPICluster::master(), backend.UID() );
    }
    else
    {
        MPICluster::send( Messages::RegisterBackend(backend.UID()) );
    }
}

/** Return the frontend for backend with UID 'uid', or return
    a null Frontend if there is no such backend. This blocks
    until the Frontend is available */
Frontend MPICluster::getFrontend(const QUuid &uid)
{
    if (uid.isNull())
        return Frontend();
        
    //Message_GetFrontend message(uid);
    //MPICluster::send( message );
    
    //return message.frontend();
    
    return Frontend();
}

/** Return the list of all of the UIDs of all of the backends
    that are available via MPI */
QList<QUuid> MPICluster::UIDs()
{
    //Message_GetUIDs message;
    //MPICluster::send( message );
    
    //return message.UIDs();
    
    return QList<QUuid>();
}

/** Return whether or not the MPICluster is running */
bool MPICluster::isRunning()
{
    if (not ::MPI::Is_initialized())
        return false;
        
    else
    {
        return globalCluster()->send_queue->isRunning() or
               globalCluster()->receive_queue->isRunning();
    }
}

/** This just shuts down this node */
void MPICluster::informedShutdown()
{
    if (not MPICluster::isRunning())
        return;

    globalCluster()->send_queue->stop();
    globalCluster()->receive_queue->stop();
    
    globalCluster()->send_queue->wait();
    globalCluster()->receive_queue->wait();

    //////////
    {
        QMutexLocker lkr( &(globalCluster()->datamutex) );
        globalCluster()->backend_registry.clear();
    }
    
    //shut down the cluster - this will call MPICluster::shutdown,
    //but this won't recurse, as MPICluster::isRunning() will be false
    Cluster::shutdown();
}

/** Shutdown the MPI cluster */
void MPICluster::shutdown()
{
    if (not MPICluster::isRunning())
        return;

    //this will shutdown the entire cluster!
    MPICluster::send( Messages::Shutdown() );
}

#endif // __SIRE_USE_MPI__
