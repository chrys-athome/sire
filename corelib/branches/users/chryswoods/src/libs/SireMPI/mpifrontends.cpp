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
//mpich requires that mpi.h is included first
#include <mpi.h>                  // CONDITIONAL_INCLUDE
#endif

#include <QMutex>
#include <QUuid>
#include <QHash>

#include "mpifrontends.h"
#include "mpibackends.h"
#include "mpinodes.h"
#include "mpinode.h"
#include "mpipromise.h"

#include "SireError/errors.h"

using namespace SireMPI;

using boost::shared_ptr;

Q_GLOBAL_STATIC( QMutex, registryMutex );

static QHash<const void*, MPIFrontends> frontends_registry;

namespace SireMPI
{

/** Return the frontends for the passed MPI nodes */
MPIFrontends SIREMPI_EXPORT getFrontends(const MPINodes &nodes)
{
    QMutexLocker lkr( registryMutex() );
    
    if (not frontends_registry.contains(nodes.communicator()))
    {
        frontends_registry.insert( nodes.communicator(), MPIFrontends(nodes,true) );
    }
    
    return frontends_registry.value(nodes.communicator());
}

/** Return the MPI frontend for the node 'node' */
MPIFrontend SIREMPI_EXPORT getFrontend(const MPINode &node)
{
    if (node.isNull())
        throw SireError::nullptr_error( QObject::tr(
            "The null MPINode does not have a valid front end!"),
                CODELOC );

    return getFrontends( node.communicator() ).getFrontEnd(node);
}

namespace detail
{

/** Private implementation of MPIFrontends */
class MPIFrontendsPvt
{
public:
    MPIFrontendsPvt()
    {}
    
    ~MPIFrontendsPvt()
    {}
    
    /** Mutex to protect access to the data */
    QMutex datamutex;
    
    /** Weak pointer to the nodes themselves */
    MPINodesPtr nodes_ptr;
    
    /** The registry of all front ends for the communicator */
    QHash<QUuid,MPIFrontend> registry;
};

/** Private implementation of MPIFrontend */
class MPIFrontendPvt
{
public:
    MPIFrontendPvt()
    {}
    
    ~MPIFrontendPvt()
    {
        send_comm.Free();
        recv_comm.Free();
    }
    
    /** Mutex used to ensure that only one job is running
        at a time */
    QMutex worker_mutex;
    
    /** Mutex used to protect access to the receive communicator */
    QMutex recv_mutex;
    
    /** Mutex used to protect access to the send communicator */
    QMutex send_mutex;
    
    /** UID of the node attached to this front end */
    QUuid node_uid;
    
    /** Communicator used to send data to the backend */
    MPI::Intracomm send_comm;
    
    /** Communicator used to receive data from the backend */
    MPI::Intracomm recv_comm;
};

} // end of namespace detail
} // end of namespace SireMPI

using namespace SireMPI::detail;

//////////
////////// Implementation of MPIFrontends
//////////

/** Null constructor */
MPIFrontends::MPIFrontends() : d( new MPIFrontendsPvt() )
{}

/** Copy constructor */
MPIFrontends::MPIFrontends(const MPIFrontends &other) : d(other.d)
{}

/** Private constructor called by getFrontEnds */
MPIFrontends::MPIFrontends(MPINodes nodes, bool)
             : d( new MPIFrontendsPvt() )
{
    qDebug() << MPI::COMM_WORLD.Get_rank() << CODELOC;

    d->nodes_ptr = nodes;

    qDebug() << MPI::COMM_WORLD.Get_rank() << CODELOC;

    //start a backend for each of the nodes
    QList<MPINode> mpinodes = nodes.getNFreeNodes(nodes.count());

    qDebug() << MPI::COMM_WORLD.Get_rank() << CODELOC;
    
    MPIBackends backends = getBackends(nodes);

    qDebug() << MPI::COMM_WORLD.Get_rank() << CODELOC;
    
    //start a backend on each node
    foreach (MPINode mpinode, mpinodes)
    {
        qDebug() << MPI::COMM_WORLD.Get_rank() << mpinode.UID().toString() << CODELOC;
        d->registry.insert( mpinode.UID(), backends.start(mpinode) );
        qDebug() << MPI::COMM_WORLD.Get_rank() << CODELOC;
    }

    qDebug() << MPI::COMM_WORLD.Get_rank() << CODELOC;
}

/** Constructor - holds all of the front ends for the nodes in 'nodes' */
MPIFrontends::MPIFrontends(const MPINodes &nodes)
{
    this->operator=( getFrontends(nodes) );
}

/* Destructor */
MPIFrontends::~MPIFrontends()
{}

/** Copy assignment operator */
MPIFrontends& MPIFrontends::operator=(const MPIFrontends &other)
{
    d = other.d;
    return *this;
}

/** Comparison operator */
bool MPIFrontends::operator==(const MPIFrontends &other) const
{
    return d.get() == other.d.get();
}

/** Comparison operator */
bool MPIFrontends::operator!=(const MPIFrontends &other) const
{
    return d.get() != other.d.get();
}

/** Return the front end for the node 'node' */
MPIFrontend MPIFrontends::getFrontEnd(const MPINode &node)
{
    QMutexLocker lkr( &(d->datamutex) );
    
    if (not d->registry.contains(node.UID()))
    {
        throw SireError::program_bug( QObject::tr(
            "There is no front end for the MPINode with UID %1.")
                .arg(node.UID().toString()), CODELOC );
    }

    return d->registry.value(node.UID());
}

/** Shutdown all of the backends */
void MPIFrontends::shutdown()
{
    QMutexLocker lkr( &(d->datamutex) );
    
    if (not d->nodes_ptr.isNull())
    {
        MPIBackends backends = getBackends(*(d->nodes_ptr));
        backends.shutdown();
        
        d->nodes_ptr = MPINodesPtr();
        d->registry.clear();
    }
}

//////////
////////// Implementation of MPIFrontend
//////////

/** Null constructor */
MPIFrontend::MPIFrontend() : d( new MPIFrontendPvt() )
{}

/** Private constructor used by MPIBackends */
MPIFrontend::MPIFrontend(const MPINode &node, bool)
            : d( new MPIFrontendPvt() )
{
    qDebug() << MPINode::globalRank() << CODELOC;

    d->node_uid = node.UID();

    qDebug() << MPINode::globalRank() << CODELOC;

    //ok, we now need to create the communicators to talk
    //to this node
    const MPI::Intracomm *comm_world 
            = static_cast<const MPI::Intracomm*>( node.communicator().communicator() );

    qDebug() << MPINode::globalRank() << CODELOC;

    BOOST_ASSERT( comm_world != 0 );

    qDebug() << MPINode::globalRank() << CODELOC;

    //create the communicators - send then receive, as receive then 
    //send in MPIBackend
    qDebug() << MPINode::globalRank() << CODELOC;
    d->send_comm = const_cast<MPI::Intracomm*>(comm_world)->Split(1, 0);
    qDebug() << MPINode::globalRank() << CODELOC;
    d->recv_comm = d->send_comm.Clone();
    qDebug() << MPINode::globalRank() << CODELOC;
}

/** Construct the front end for the passed node */
MPIFrontend::MPIFrontend(const MPINode &node)
{
    this->operator=( getFrontend(node) );
}

/** Copy constructor */
MPIFrontend::MPIFrontend(const MPIFrontend &other)
            : d(other.d)
{}

/** Destructor */
MPIFrontend::~MPIFrontend()
{}

/** Copy assignment operator */
MPIFrontend& MPIFrontend::operator=(const MPIFrontend &other)
{
    d = other.d;
    return *this;
}

/** Comparison operator */
bool MPIFrontend::operator==(const MPIFrontend &other) const
{
    return d.get() == other.d.get();
}

/** Comparison operator */
bool MPIFrontend::operator!=(const MPIFrontend &other) const
{
    return d.get() != other.d.get();
}

/** Return whether or not the backend is busy */
bool MPIFrontend::isBusy()
{
    if (d->worker_mutex.tryLock())
    {
        d->worker_mutex.unlock();
        return false;
    }
    else
        return true;
}

/** Tell the backend to stop work */
void MPIFrontend::stopWork()
{
    if (this->isBusy())
        return;
        
    //send a STOP request
    QMutexLocker lkr( &(d->send_mutex) );
    
    int envelope[2];
    envelope[0] = MPIBackend::STOP_WORK;
    envelope[1] = 0;

    d->send_comm.Send( envelope, 2, MPI::INT, 1, 0 );
}

/** Tell the backend to abort the current work */
void MPIFrontend::abortWork()
{
    if (this->isBusy())
        return;
        
    //send an ABORT request
    QMutexLocker lkr( &(d->send_mutex) );
    
    int envelope[2];
    envelope[0] = MPIBackend::ABORT_WORK;
    envelope[1] = 0;

    d->send_comm.Send( envelope, 2, MPI::INT, 1, 0 );
}

/** Perform work on the backend */
void MPIFrontend::performWork(MPIPromise &promise)
{
    BOOST_ASSERT( promise.node().UID() == d->node_uid );

    QByteArray worker_data = promise.initialData();
    
    if (worker_data.isEmpty())
        return;

    QMutexLocker lkr( &(d->worker_mutex) );
    
    ///// Send block ///////////
    {
        //send a message to the backend to start the work
        QMutexLocker lkr( &(d->send_mutex) );
        int envelope[2];
        envelope[0] = MPIBackend::START_WORK;
        envelope[1] = worker_data.count();
    
        d->send_comm.Send( envelope, 2, MPI::INT, 1, 0 );
        d->send_comm.Send( worker_data.constData(), worker_data.count(),
                           MPI::BYTE, 1, 0 );
    }

    ///// Receive block ////////
    {
        QMutexLocker lkr( &(d->recv_mutex) );

        //enter a loop waiting for results...
        do
        {
            int envelope[3];

            //wait for a response
            d->recv_comm.Recv( envelope, 3, MPI::INT, 1, 0 );
        
            QByteArray worker_data;
            double current_progress = 0;
        
            if (envelope[1] > 0)
            {
                //there is some result data available
                worker_data = QByteArray(envelope[1]+1, ' ');
            
                d->recv_comm.Recv( worker_data.data(), envelope[1], 
                                   MPI::BYTE, 1, 0 );
            }
        
            if (envelope[2] > 0)
            {
                //there is a progress value available
                d->recv_comm.Recv(&current_progress, 1, MPI::DOUBLE, 1, 0);
            }
        
            //now what type of message is it?
            switch (envelope[0])
            {
                case MPIBackend::JOB_FINISHED:
                    promise.setFinalData(worker_data);
                    break;
                
                case MPIBackend::JOB_STOPPED:
                    promise.setStopped(worker_data, current_progress);
                    break;
                    
                case MPIBackend::JOB_ABORTED:
                    promise.setAborted();
                    break;
                    
                case MPIBackend::JOB_PROGRESS_UPDATE:
                    promise.setProgress(current_progress);
                    break;
                    
                case MPIBackend::JOB_RESULT_UPDATE:
                    promise.setInterimData(worker_data, current_progress);
                    break;
            }
        
        } while (promise.isRunning());
    }
}

/** Tell the backend to send a progress update */
void MPIFrontend::requestProgress()
{
    if (not this->isBusy())
        return;
        
    //ask for a progress update
    QMutexLocker lkr( &(d->send_mutex) );
    
    int envelope[2];
    envelope[0] = MPIBackend::SEND_PROGRESS;
    envelope[1] = 0;

    d->send_comm.Send( envelope, 2, MPI::INT, 1, 0 );
}

/** Tell the backend to send an interim result */
void MPIFrontend::requestInterimResult()
{
    if (not this->isBusy())
        return;
        
    //ask for an interim result
    QMutexLocker lkr( &(d->send_mutex) );
    
    int envelope[2];
    envelope[0] = MPIBackend::SEND_INTERIM_RESULT;
    envelope[1] = 0;

    d->send_comm.Send( envelope, 2, MPI::INT, 1, 0 );
}
