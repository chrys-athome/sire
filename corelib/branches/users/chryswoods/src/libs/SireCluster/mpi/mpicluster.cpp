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

#ifdef SIRE_USE_MPI

#include <mpi.h>   //mpich requires that mpi.h is included first

#include "mpicluster.h"
#include "sendqueue.h"
#include "receivequeue.h"

#include "Siren/mutex.h"
#include "Siren/errors.h"

using namespace SireCluster;
using namespace SireCluster::MPI;
using namespace Siren;

class MPIClusterData
{
public:
    MPIClusterData() : send_queue(0), receive_queue(0)
    {
        uid = QUuid::createUuid();
    }
    
    ~MPIClusterData()
    {
        delete send_queue;
        delete receive_queue;
    }
    
    /** The UID of this MPI process */
    QUuid uid;
    
    /** The MPI communicator used for by SireCluster */
    ::MPI::Intracomm global_comm;
    
    /** Which MPI rank to send to reach each
        process (identified by UID) - this allows
        messages to be routed */
    QHash<QUuid,int> uid_to_rank;
    
    /** Queue used to send messages */
    SendQueue *send_queue;
    
    /** Queue used to receive messages */
    ReceiveQueue *receive_queue;
};

Q_GLOBAL_STATIC( Mutex, mpiMutex );

static MPIClusterData *d(0);

/** Return the UID of this process - every process that can be reached
    via MPI has its own UID (the MPI rank is not used as a process
    may be in several inter-communicators) */
QUuid MPICluster::processUID()
{
    MutexLocker lkr( mpiMutex() );
    
    if (d)
        return d->uid;
    else
        return QUuid();
}

/** Return the MPI rank of the process that is associated
    with the passed UID - this is either the rank itself,
    or the rank of the process that acts as a bridge to
    the desired process */
int MPICluster::getRankFor(const QUuid &uid)
{
    MutexLocker lkr( mpiMutex() );
    
    if (not d)
        throw Siren::unavailable_resource( QObject::tr(
                "MPI has not been started, so it is not possible "
                "for it to be used to find the process with UID '%1'.")
                    .arg(uid.toString()), CODELOC );

    int rank = d->uid_to_rank.value(uid, -1);
    
    if (rank == -1)
        throw Siren::unavailable_resource( QObject::tr(
                "The location of the MPI resource that allows "
                "communication with 
}

/** Return the MPI ranks of the processes that are associated
    with the passed UIDs */


/** Return whether or not this is the master process of this
    MPI cluster - the master process is the one with rank == 0
    on MPI::COMM_WORLD. The master process is used to route
    messages between nodes (it is a star network with the master
    at the center) */
bool MPICluster::isMaster()
{
    if (not ::MPI::Is_initialized())
        return true;
    else
        return ::MPI::COMM_WORLD.Get_rank() == 0;
}

void MPICluster::start()
{
    MutexLocker lkr( mpiMutex() );
    
    if (d == 0)
    {
        if (not ::MPI::Is_initialized())
        {
            int argc = 0;
            char **argv = 0;
            
            ::MPI::Init_thread(argc, argv, MPI_THREAD_MULTIPLE);
            
            if (::MPI::Query_thread() != MPI_THREAD_MULTIPLE)
            {
                throw Siren::unsupported( QObject::tr(
                        "SireCluster needs an MPI implementation that supports "
                        "MPI_THREAD_MULTIPLE. Unfortunately, the MPI you are using "
                        "does not support multi-threaded MPI."), CODELOC );
            }
        }

        if (::MPI::COMM_WORLD.Get_size() == 1)
            //no need for MPI
            return;
    
        d = new MPIClusterData();

        //get the global MPI communicator
        ::MPI::Group mpigroup = ::MPI::COMM_WORLD.Get_group();
        d->global_comm = ::MPI::COMM_WORLD.Create(mpigroup);

        ::MPI::Intracomm send_comm, recv_comm;
        
        if ( MPICluster::isMaster() )
        {
            //create the send, then receive communicators
            send_comm = d->global_comm.Create(mpigroup);
            recv_comm = d->global_comm.Create(mpigroup);
        }
        else
        {
            //must be the other way around (as all other nodes
            //listen to the master)
            recv_comm = d->global_comm.Create(mpigroup);
            send_comm = d->global_comm.Create(mpigroup);
        }
    
        mpigroup.Free();

        //create and start the event loops
        d->send_queue = new SendQueue(send_comm);
        d->receive_queue = new ReceiveQueue(recv_comm);
        
        d->send_queue->start();
        d->receive_queue->start();

        //wait for everyone to get here
        d->global_comm.Barrier();
        
        //broadcast the rank / UID
        MPICluster::broadcast( ProcessStarted(d->uid) );
    }
}

void MPICluster::shutdown()
{
    MutexLocker lkr( mpiMutex() );
    
    if (d != 0)
    {
        //shutdown the cluster
        MPICluster::broadcast( ProcessEnded(d->uid) );
        
        d->send_queue->kill();
        d->receive_queue->kill();
        
        delete d;
        d = 0;
    }
}

#endif // SIRE_USE_MPI
