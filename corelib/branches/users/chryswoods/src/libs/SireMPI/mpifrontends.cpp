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

#include "mpifrontends.h"
#include "mpinodes.h"

#include "SireError/errors.h"

using namespace SireMPI;

using boost::shared_ptr;

Q_GLOBAL_STATIC( QMutex, registryMutex );

static QHash<void*, MPIFrontEnds*> frontends_registry;

/** Return the frontends for the passed MPI nodes */
MPIFrontEnds SIREMPI_EXPORT getFrontEnds(const MPINodes &nodes)
{
    QMutexLocker lkr( registryMutex() );
    
    if (not frontends_registry.contains(nodes.communicator()))
    {
        frontends_registry.insert( nodes.communicator(),
                                   new MPIFrontEnds(nodes,true) );
    }
    
    return *( frontends_registry.value(nodes.communicator()) );
}

/** Return the MPI frontend for the node 'node' */
MPIFrontEnd SIREMPI_EXPORT getFrontEnd(const MPINode &node)
{
    if (node.isNull())
        throw SireError::nullptr_error( QObject::tr(
            "The null MPINode does not have a valid front end!"),
                CODELOC );

    return getFrontEnds( node.communicator() ).getFrontEnd(node);
}

namespace SireMPI
{
namespace detail
{

/** Private implementation of MPIFrontEnds */
class MPIFrontEndsPvt
{
public:
    MPIFrontEndsPvt()
    {}
    
    ~MPIFrontEndsPvt()
    {}
    
    /** Mutex to protect access to the data */
    QMutex datamutex;
    
    /** Weak pointer to the nodes themselves */
    MPINodesPtr nodes_ptr;
    
    /** The registry of all front ends for the communicator */
    QHash< QUuid,shared_ptr<MPIFrontEnd> > registry;
};

/** Private implementation of MPIFrontEnd */
class MPIFrontEndPvt
{
public:
    MPIFrontEndPvt() : datamutex(QMutex::Recursive)
    {}
    
    ~MPIFrontEndPvt()
    {}
    
    /** Mutex used to ensure that only one job is running
        at a time */
    QMutex workermutex;
    
    /** Mutex used to protect access to the receive communicator */
    QMutex recv_mutex;
    
    /** Mutex used to protect access to the send communicator */
    QMutex send_mutex;
    
    /** Weak pointer to the node attached to this front end */
    MPINodePtr node_ptr;
    
    /** Communicator used to send data to the backend */
    MPI::Intracomm send_comm;
    
    /** Communicator used to receive data from the backend */
    MPI::Intracomm recv_comm;
};

} // end of namespace detail
} // end of namespace SireMPI

//////////
////////// Implementation of MPIFrontEnds
//////////

/** Null constructor */
MPIFrontEnds::MPIFrontEnds() : d( new MPIFrontEndsPvt() )
{}

/** Private constructor called by getFrontEnds */
MPIFrontEnds::MPIFrontEnds(const MPINodes &nodes, bool)
             d( new MPIFrontEndsPvt() )
{
    d->nodes_ptr = nodes;

    //start a backend for each of the nodes
    QList<MPINode> mpinodes = nodes.getNFreeNodes(nodes.count());
    
    MPIBackends backends = getBackends(nodes);
    
    //start a backend on each node
    foreach (MPINode mpinode, mpinodes)
    {
        d->registry.insert( mpinode.UID(), backends.start(mpinode) );
    }
}

/** Constructor - holds all of the front ends for the nodes in 'nodes' */
MPIFrontEnds::MPIFrontEnds(const MPINodes &nodes)
{
    this->operator=( getFrontEnds(nodes) );
}

/* Destructor */
MPIFrontEnds::~MPIFrontEnds()
{}

/** Copy assignment operator */
MPIFrontEnds& operator=(const MPIFrontEnds &other)
{
    d = other.d;
    return *this;
}

/** Comparison operator */
bool MPIFrontEnds::operator==(const MPIFrontEnds &other) const
{
    return d.get() == other.d.get();
}

/** Comparison operator */
bool MPIFrontEnds::operator!=(const MPIFrontEnds &other) const
{
    return d.get() != other.d.get();
}

/** Return the front end for the node 'node' */
MPIFrontEnd MPIFrontEnds::getFrontEnd(const MPINode &node)
{
    QMutexLocker lkr( &(d->datamutex) );
    
    if (not d->registry.contains(node.UID()))
    {
        throw SireError::program_bug( QObject::tr(
            "There is no front end for the MPINode with UID %1.")
                .arg(node.UID().toString()), CODELOC );
    }

    return *( d->registry.value(node.UID()) );
}

/** Shutdown all of the backends */
void MPIFrontEnds::shutdown()
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
////////// Implementation of MPIFrontEnd
//////////

/** Null constructor */
MPIFrontEnd::MPIFrontEnd() : d( new MPIFrontEndPvt() )
{}

/** Private constructor used by MPIBackends */
MPIFrontEnd::MPIFrontEnd(const MPINode &node, bool)
            : d( new MPIFrontEndPvt() )
{
    d->node_ptr = node;
}

/** Construct the front end for the passed node */
MPIFrontEnd::MPIFrontEnd(const MPINode &node)
{
    this->operator=( getFrontEnd(node) );
}

/** Copy constructor */
MPIFrontEnd::MPIFrontEnd(const MPIFrontEnd &other)
            : d(other.d)
{}

/** Destructor */
MPIFrontEnd::~MPIFrontEnd()
{}

/** Copy assignment operator */
MPIFrontEnd& MPIFrontEnd::operator=(const MPIFrontEnd &other)
{
    d = other.d;
    return *this;
}

/** Comparison operator */
bool MPIFrontEnd::operator==(const MPIFrontEnd &other) const
{
    return d.get() == other.d.get();
}

/** Comparison operator */
bool MPIFrontEnd::operator!=(const MPIFrontEnd &other) const
{
    return d.get() != other.d.get();
}

/** Return the node connected to this front end */
MPINode MPIFrontEnd::node()
{
    if (d->node_ptr.isNull())
        throw SireError::program_bug( QObject::tr(
            "We should not have a null MPINode attached to a MPIFrontEnd...!"),
                CODELOC );
                
    return *( d->node_ptr );
}

/** Return whether or not the backend is busy */
bool MPIFrontEnd::isBusy()
{
    if (d->workermutex.tryLock())
    {
        d->workermutex.unlock();
        return false;
    }
    else
        return true;
}

/** Tell the backend to stop work */
void MPIFrontEnd::stopWork()
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
void MPIFrontEnd::abortWork()
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
void MPIFrontEnd::performWork(MPIPromise &promise)
{
    if (worker_data.isEmpty())
        return;

    BOOST_ASSERT( promise.node().UID() == this->node().UID() );

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
                d->recv_comm.Recv(&current_progress, MPI::DOUBLE, 1, 0);
            }
        
            //now what type of message is it?
            switch (envelope[0])
            {
                case MPIBackEnd::JOB_FINISHED:
                    promise.setFinalData(worker_data);
                    break;
                
                case MPIBackEnd::JOB_STOPPED:
                    promise.setStopped(worker_data, current_progress);
                    break;
                    
                case MPIBackEnd::JOB_ABORTED:
                    promise.setAborted();
                    break;
                    
                case MPIBackEnd::JOB_PROGRESS_UPDATE:
                    promise.setProgress(current_progress);
                    break;
                    
                case MPIBackEnd::JOB_RESULT_UPDATE:
                    promise.setInterimData(worker_data, current_progress);
                    break;
            }
        
        } while (promise.isRunning())
    }
}

/** Tell the backend to send a progress update */
void MPIFrontEnd::requestProgress()
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
void MPIFrontEnd::requestInterimResult()
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
