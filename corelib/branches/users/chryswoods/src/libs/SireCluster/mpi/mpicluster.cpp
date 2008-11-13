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

#include "SireCluster/frontend.h"
#include "SireCluster/backend.h"

#include "mpicluster.h"
#include "messages.h"
#include "sendqueue.h"
#include "receivequeue.h"

#include "SireError/errors.h"

using namespace SireCluster;
using namespace SireCluster::MPI;

namespace SireCluster
{
namespace MPI
{
namespace detail
{

/** Private implementation of MPICluster */
class MPIClusterPvt
{
public:
    MPIClusterPvt() : datamutex(QMutex::Recursive),
                      send_queue(0), receive_queue(0)
    {}
    
    ~MPIClusterPvt()
    {
        delete send_queue;
        delete receive_queue;
    }
    
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

} // end of namespace detail
} // end of namespace MPI
} // end of namespace SireCluster

using namespace SireCluster::MPI::detail;

Q_GLOBAL_STATIC_WITH_ARGS( QMutex, globalMutex, (QMutex::Recursive) );

static MPICluster *global_cluster;

/** Return the global MPICluster */
MPICluster& MPICluster::cluster()
{
    QMutexLocker lkr( globalMutex() );

    if (global_cluster == 0)
    {
        global_cluster = new MPICluster();

        //create the private send and receive communicators
        if (not ::MPI::Is_initialized())
        {
            int argc = 0;
            char **argv = 0;
            ::MPI::Init(argc, argv);
        }
        
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
        global_cluster->d->send_queue = new SendQueue(send_comm);
        global_cluster->d->receive_queue = new ReceiveQueue(recv_comm);
        
        global_cluster->d->send_queue->start();
        global_cluster->d->receive_queue->start();
    }
    
    return *global_cluster;
}

/** Constructor */
MPICluster::MPICluster() : d( new MPIClusterPvt() )
{}

/** Destructor */
MPICluster::~MPICluster()
{}

/** Return the rank of this process in the MPI cluster */
int MPICluster::getRank()
{
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

    MPICluster &c = cluster();
    
    QMutexLocker lkr( &(c.d->datamutex) );
    
    if (not c.d->backend_registry.contains(uid))
    {
        c.d->backend_registry.insert(uid, rank);
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

/** Shutdown the MPI cluster */
void MPICluster::shutdown()
{
    if ( MPICluster::isMaster() )
    {
        //this will shutdown the entire cluster!
        
    }
}

#endif // __SIRE_USE_MPI__
