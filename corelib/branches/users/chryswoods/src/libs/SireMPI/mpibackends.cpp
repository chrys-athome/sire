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

#include "mpibackends.h"
#include "mpifrontends.h"
#include "mpiworker.h"
#include "mpinode.h"
#include "mpinodes.h"

#include "SireError/errors.h"
#include "SireError/printerror.h"

#include "SireStream/streamdata.hpp"

#include <QMutex>
#include <QWaitCondition>
#include <QHash>
#include <QThread>
#include <QByteArray>

#include <boost/tuple/tuple.hpp>

using namespace SireMPI;
using namespace SireStream;

using boost::shared_ptr;
using boost::tuple;

static const int SHUTDOWN_MPI = 0;
static const int START_MPI_BACKEND = 1;
static const int STOP_MPI_BACKEND = 2;

#ifdef __SIRE_USE_MPI__

//////////////
//////////////
// MPI enabled version of these classes
//////////////
//////////////

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
#endif // HAVE_LSEEK64å

namespace SireMPI
{
namespace detail
{

/** Private implementation of MPIBackendsPvt */
class MPIBackendsPvt
{
public:
    MPIBackendsPvt() : datamutex(QMutex::Recursive), comm_world(0)
    {}
    
    ~MPIBackendsPvt()
    {}

    /** Mutex to protect access to this object */
    QMutex datamutex;

    /** Mutex used to protect access to the event loop */
    QMutex execmutex;

    /** Mutex and waiter used to ensure only one message is broadcast
        to the nodes at a time */
    QMutex message_mutex;

    /** Mutex and waiter used to ensure only one message is broadcast
        to the nodes at a time */
    QWaitCondition message_waiter;

    /** The global communicator used to 
        communicate with all of the nodes in this 
        set of MPINodes */
    MPI::Intracomm *comm_world;
    
    /** The registry of all active backends */
    QHash<QUuid, MPIBackend> active_backends;
    
    /** The list of all backends that are in the process of shutting down */
    QList<MPIBackend> stopped_backends;
    
    struct Message
    {
        QByteArray data;
        int envelope[3];
    };
    
    /** The message to be broadcast to nodes */
    Message message;
};

/** Private implementation of MPIBackendPvt */
class MPIBackendPvt : public QThread
{
public:
    MPIBackendPvt() : keep_running(true)
    {}
    
    ~MPIBackendPvt()
    {
        keep_running = false;

        //give the thread 10 seconds to exit
        if (not QThread::wait(10000))
        {
            QThread::terminate();
        }
        
        recv_comm.Free();
        send_comm.Free();
    }
    
    /** The unique ID of the node */
    QUuid uid;
    
    /** The communicator used to receive instructions 
        from the master */
    MPI::Intracomm recv_comm;
    
    /** The communicator used to send data back to the master */
    MPI::Intracomm send_comm;
    
    /** Whether or not to keep looping */
    bool keep_running;

protected:
    void sendResponse(int response, const MPIWorker &worker);
    void runJob(MPIWorker &worker);

    void run();
};

} // end of namespace detail
} // end of namespace SireMPI

using namespace SireMPI::detail;

namespace SireMPI
{

static QHash<const void*, MPIBackends> backends_registry;

Q_GLOBAL_STATIC( QMutex, registryMutex );

/** Return the backends manager for the collection of nodes in 'nodes' */
MPIBackends SIREMPI_EXPORT getBackends(const MPINodes &nodes)
{
    QMutex *mutex = registryMutex();
    
    QMutexLocker lkr(mutex);
    
    if ( not backends_registry.contains(nodes.communicator()) )
    {
        backends_registry.insert( nodes.communicator(), MPIBackends(nodes) );
    }
    
    return backends_registry.value(nodes.communicator());
}

/** Return the backends manager for the collection of nodes in 'nodes' */
MPIBackends SIREMPI_EXPORT getBackends(const MPIBackendNodes &nodes)
{
    QMutex *mutex = registryMutex();
    
    QMutexLocker lkr(mutex);
    
    if ( not backends_registry.contains(nodes.communicator()) )
    {
        backends_registry.insert( nodes.communicator(), MPIBackends(nodes) );
    }
    
    return backends_registry.value(nodes.communicator());
}

} // end of namespace SireMPI

////////
//////// Implementation of MPIBackends
////////

/** Null constructor */
MPIBackends::MPIBackends() : d( new MPIBackendsPvt() )
{}

/** Construct to manage the backends on this node for the group of nodes
    in 'nodes' */
MPIBackends::MPIBackends(const MPINodes &nodes)
            : d( new MPIBackendsPvt() )
{
    d->comm_world = const_cast<MPI::Intracomm*>(
                        static_cast<const MPI::Intracomm*>( nodes.communicator() ) );
}

/** Construct to manage the backends on this node for the group of nodes
    in 'nodes' */
MPIBackends::MPIBackends(const MPIBackendNodes &nodes)
            : d( new MPIBackendsPvt() )
{
    d->comm_world = const_cast<MPI::Intracomm*>(
                        static_cast<const MPI::Intracomm*>( nodes.communicator() ) );
}

/** Copy constructor */
MPIBackends::MPIBackends(const MPIBackends &other)
            : d(other.d)
{}

/** Destructor */
MPIBackends::~MPIBackends()
{}

/** Copy assignment operator */
MPIBackends& MPIBackends::operator=(const MPIBackends &other)
{
    d = other.d;
    return *this;
}

/** Comparison operator */
bool MPIBackends::operator==(const MPIBackends &other) const
{
    return d.get() == other.d.get();
}

/** Comparison operator */
bool MPIBackends::operator!=(const MPIBackends &other) const
{
    return d.get() != other.d.get();
}

/** Return the global rank of this node in MPI::COMM_WORLD */
int MPIBackends::globalRank()
{
    if (MPI::Is_initialized())
        return MPI::COMM_WORLD.Get_rank();
    else
        return 0;
}

/** Broadcast a message to the nodes in this communicator */
void MPIBackends::broadcastMessage(int message, int data)
{
    QMutexLocker lkr( &(d->message_mutex) );
    
    d->message.envelope[0] = message;
    d->message.envelope[1] = data;
    d->message.envelope[2] = 0;
    d->message.data = QByteArray();
    
    d->message_waiter.wakeAll();
}

/** Broadcast a message to the nodes in this communicator */
void MPIBackends::broadcastMessage(int message, const QByteArray &data)
{
    QMutexLocker lkr( &(d->message_mutex) );
    
    d->message.envelope[0] = message;
    d->message.envelope[1] = 0;
    d->message.envelope[2] = data.count();
    d->message.data = data;
    
    d->message_waiter.wakeAll();
}

/** Start a front end for the node 'node' */
MPIFrontend MPIBackends::start(const MPINode &node)
{
    //start the backend - give it the UID of the node
    QByteArray data;
    QDataStream ds(&data, QIODevice::WriteOnly);
    
    ds << node.UID();
    
    this->broadcastMessage(START_MPI_BACKEND, data);

    return MPIFrontend(node, true);
}

/** Stop the backend for the node 'node' */
void MPIBackends::stop(const QUuid &uid)
{
    QByteArray data;
    QDataStream ds(&data, QIODevice::WriteOnly);
    
    ds << uid;
    
    this->broadcastMessage(STOP_MPI_BACKEND, data);
}

static QUuid readUID(const QByteArray &data)
{
    QDataStream ds(data);
    QUuid uid;
    ds >> uid;
    return uid;
}

/** Private function that contains an event loop that processes
    all of the events on this node that involves the collection
    of nodes 'nodes' */
int MPIBackends::_pvt_exec()
{
    qDebug() << MPI::COMM_WORLD.Get_rank() << CODELOC;

    QMutexLocker lkr( &(d->execmutex) );

    qDebug() << MPI::COMM_WORLD.Get_rank() << CODELOC;

    BOOST_ASSERT( MPI::Is_initialized() );
    BOOST_ASSERT( d->comm_world != 0 );

    qDebug() << MPI::COMM_WORLD.Get_rank() << CODELOC;
   
    //get our rank in the MPINodes communicator
    int my_mpirank = d->comm_world->Get_rank();

    qDebug() << MPI::COMM_WORLD.Get_rank() << CODELOC;
    
    //now go into a loop waiting for instructions
    bool keep_looping = true;

    int master_rank = 0;

    qDebug() << MPI::COMM_WORLD.Get_rank() << CODELOC;

    do
    {   
        try
        {
            MPI::Status status;
            int message[3];
            QByteArray data;

            qDebug() << MPI::COMM_WORLD.Get_rank() << CODELOC;

            //ensure that only one message can be broadcast at a time
            QMutexLocker lkr( &(d->message_mutex) );

            qDebug() << MPI::COMM_WORLD.Get_rank() << CODELOC;
            
            if (my_mpirank == master_rank)
            {
                qDebug() << MPI::COMM_WORLD.Get_rank() << CODELOC;

                //wait for the message to be broadcast
                d->message_waiter.wait( &(d->message_mutex) );

                qDebug() << MPI::COMM_WORLD.Get_rank() << CODELOC;
                
                //ok - copy the message to be broadcast
                for (int i=0; i<3; ++i)
                    message[i] = d->message.envelope[i];
            }

            qDebug() << MPI::COMM_WORLD.Get_rank() << CODELOC;
            
            //wait for instructions that are broadcast to all nodes
            //from node 0
            d->comm_world->Bcast( message, 3, MPI::INT, master_rank );
        
            qDebug() << MPI::COMM_WORLD.Get_rank() << CODELOC;

            //now get the data that is broadcast to all nodes
            if (message[2] > 0)
            {
                qDebug() << MPI::COMM_WORLD.Get_rank() << CODELOC;

                data = QByteArray(message[2] + 1, ' ');
                d->comm_world->Bcast( data.data(), message[2], MPI::BYTE,
                                      master_rank );
                
                qDebug() << MPI::COMM_WORLD.Get_rank() << CODELOC;
            }

            //what is the message?
            switch( message[0] )
            {
                case START_MPI_BACKEND:
                    //start a new MPI backend loop that listens to instructions
                    //on the specified channel from the specified master
                    if (message[1] == my_mpirank)
                    {
                        //this is a request to start the backend on this node
                        this->startBackend( ::readUID(data) );
                    }
                    else
                    {
                        //this node will not be involved
                        MPI::Intracomm null_comm = d->comm_world->Split(0,0);
                        null_comm.Free();
                    }
                    
                    break;
        
                case STOP_MPI_BACKEND:
                    //stop the specified MPI backend loop
                    if (message[1] == my_mpirank)
                    {
                        this->stopBackend( ::readUID(data) );
                    }
                    
                    break;
                
                case SHUTDOWN_MPI:
                    //completely shut down MPI on this node
                    if (message[1] == my_mpirank or message[1] == -1)
                    {
                        this->stopAllBackends();
                        keep_looping = false;
                    }
                    break;

                default:
                    qWarning() << "Could not interpret MPI instruction" << message[0];
                    break;
            }
        }
        catch(const SireError::exception &e)
        {
            SireError::printError(e);
        }
        catch(const std::exception &e)
        {
            SireError::printError(SireError::std_exception(e));
        }
        catch(...)
        {
            SireError::printError( 
                    SireError::unknown_exception("An unknown error occured!", 
                                                  CODELOC));
        }

    } while (keep_looping);

    return 0;
}

/** Enter the MPI event processing loop to process the events
    of the nodes in 'nodes' */
int MPIBackends::exec()
{
    return this->_pvt_exec();
}

/** This class private class is used to run SireMPI::exec in a background
    thread - this is useful for running it as a background thread of
    the master process */
class SireMPI_ExecRunner : private QThread
{
public:
    SireMPI_ExecRunner() : QThread()
    {}
    
    void start(MPIBackends mpibackends)
    {
        qDebug() << MPI::COMM_WORLD.Get_rank() << CODELOC;
        backends = mpibackends;

        qDebug() << MPI::COMM_WORLD.Get_rank() << CODELOC;
        QThread::start();
        qDebug() << MPI::COMM_WORLD.Get_rank() << CODELOC;
    }

    bool isRunning() const
    {
        return QThread::isRunning();
    }

    ~SireMPI_ExecRunner()
    {
        this->wait();
    }

protected:
    void run()
    {
        qDebug() << MPI::COMM_WORLD.Get_rank() << CODELOC;
        try
        {
            qDebug() << MPI::COMM_WORLD.Get_rank() << CODELOC;
            backends.exec();
            qDebug() << MPI::COMM_WORLD.Get_rank() << CODELOC;
            return;
        }
        catch(const SireError::exception &e)
        {
             SireError::printError(e);
        }
        catch(const std::exception &e)
        {
            SireError::printError(SireError::std_exception(e));
        }
        catch(...)
        {
            SireError::printError( 
                    SireError::unknown_exception("An unknown error occured!", 
                                                    CODELOC));
        }
    }
    
private:
    MPIBackends backends;
};

/** Enter the MPI event processing loop, but run the loop in 
    a background thread */
void MPIBackends::execBG()
{
    //use an object to start and stop the loop
    //(as hopefully the object will be deleted when the library
    // exits, and thus an MPI shutdown will be called)
    qDebug() << MPI::COMM_WORLD.Get_rank() << CODELOC;
    SireMPI_ExecRunner *runner = new SireMPI_ExecRunner();

    qDebug() << MPI::COMM_WORLD.Get_rank() << CODELOC;
    runner->start(*this);
    qDebug() << MPI::COMM_WORLD.Get_rank() << CODELOC;

    //eventually need to delete runner or we'll have a memory leak...
}

/** Start a backend that receives instructions from the node with UID 'uid'
    in the MPI communicator */
void MPIBackends::startBackend(const QUuid &uid)
{
    BOOST_ASSERT( d->comm_world != 0 );

    QMutexLocker lkr( &(d->datamutex) );

    //we shouldn't have this tag already...
    if ( d->active_backends.contains(uid) )
        throw SireError::program_bug( QObject::tr(
            "It should not be possible to start two backends with the "
            "same UID (%1)").arg(uid.toString()), CODELOC );

    //create a backend (this creates the necessary communicators)
    MPIBackend backend(d->comm_world, uid);
    
    //start the thread
    backend.start();
    
    d->active_backends.insert(uid, backend);
}

/** Stop the backend that is performing the work directed by the 
    node with UID 'uid' in the MPI communicator */
void MPIBackends::stopBackend(const QUuid &uid)
{
    QMutexLocker lkr( &(d->datamutex) );

    MPIBackend backend = d->active_backends.take(uid);
    
    //stop the backend
    backend.stop();
        
    //add this backend to the list of ones that have been stopped
    d->stopped_backends.append(backend);
}

/** Stop all of the backends that are running on this node using
    this MPI communicator */
void MPIBackends::stopAllBackends()
{
    QMutexLocker lkr( &(d->datamutex) );
    
    QList<QUuid> uids = d->active_backends.keys();
    
    foreach (QUuid uid, uids)
    {
        MPIBackend backend = d->active_backends.take(uid);
        backend.stop();
        
        d->stopped_backends.append(backend);
    }
    
    //now wait for them all to finish
    while (not d->stopped_backends.isEmpty())
    {
        MPIBackend backend = d->stopped_backends.takeFirst();
        backend.wait();
    }
}

/** Shutdown this MPI communicator */
void MPIBackends::shutdown()
{
    this->stopAllBackends();
}

////////
//////// Implementation of MPIBackend
////////

/** Null constructor */
MPIBackend::MPIBackend()
{}

/** Construct a backend to perform the work given to it by a master
    node (in the communicator 'communicator') */
MPIBackend::MPIBackend(void *communicator, const QUuid &uid)
           : d(new MPIBackendPvt())
{
    BOOST_ASSERT( communicator != 0) ;
    
    //split the communicator so it just contains this node and the
    //master node - this is rank 1, the master is rank 0
    MPI::Intracomm *comm_world = const_cast<MPI::Intracomm*>(
                                    static_cast<const MPI::Intracomm*>(communicator) );
                                    
    d->uid = uid;
    d->recv_comm = comm_world->Split(1, 1);
    d->send_comm = d->recv_comm.Clone();
}

/** Destructor */
MPIBackend::~MPIBackend()
{}

/** Start this backend */
void MPIBackend::start()
{
    if (d.get() != 0)
        d->start();
}

/** Stop this backend */
void MPIBackend::stop()
{
    if (d.get() != 0)
        d->keep_running = false;
}

/** Wait for this backend to finish */
void MPIBackend::wait()
{
    if (d.get() != 0)
        d->wait();
}

/** Return the rank of this node in MPI::COMM_WORLD */
int MPIBackend::globalRank()
{
    if (MPI::Is_initialized())
    {
        return MPI::COMM_WORLD.Get_rank();
    }
    else
        return 0;
}

/** Small function used to send the MPIWorker back to the master */
void MPIBackendPvt::sendResponse(int response, const MPIWorker &worker)
{
    int envelope[3];
    envelope[0] = response;

    QByteArray worker_data;
    double current_progress = 0;

    if (response == MPIBackend::JOB_FINISHED or 
        response == MPIBackend::JOB_ABORTED )
    {
        envelope[2] = 0;
    }
    else
    {
        envelope[2] = 1;
        current_progress = worker.progress();
    }

    if (response == MPIBackend::JOB_PROGRESS_UPDATE or 
        response == MPIBackend::JOB_ABORTED)
    {
        envelope[1] = 0;
    }
    else
    {
        worker_data = SireStream::save(worker);
        envelope[1] = worker_data.size();
    }
    
    //send the envelope describing the response
    send_comm.Send(envelope, 3, MPI::INT, 0, 0);
    
    if (envelope[1] > 0)
    {
        send_comm.Send(worker_data.constData(), worker_data.size(), 
                       MPI::BYTE, 0, 0);
    }
    
    if (envelope[2] > 0)
    {
        send_comm.Send(&current_progress, 1, MPI::DOUBLE, 0, 0);
    }
}

/** Run the job contained in the passed worker */
void MPIBackendPvt::runJob(MPIWorker &worker)
{
    try
    {
        while (not worker.hasFinished())
        {
            //do we have an instruction?
            if (recv_comm.Iprobe(0, 0))
            {
                //yes there is - receive it
                int instruction;
                recv_comm.Recv(&instruction, 1, MPI::INT, 0, 0);
            
                switch (instruction)
                {
                    case MPIBackend::STOP_WORK:
                        this->sendResponse(MPIBackend::JOB_STOPPED, worker);
                        return;
                    
                    case MPIBackend::ABORT_WORK:
                        this->sendResponse(MPIBackend::JOB_ABORTED, worker);
                        return;
                        
                    case MPIBackend::SEND_PROGRESS:
                        this->sendResponse(MPIBackend::JOB_PROGRESS_UPDATE, worker);
                        break; 
                        
                    case MPIBackend::SEND_INTERIM_RESULT:
                        this->sendResponse(MPIBackend::JOB_RESULT_UPDATE, worker);
                        break;
                }
            }

            if (not keep_running)
                return;

            //now run a chunk of work
            worker.runChunk();
        }
        
        this->sendResponse(MPIBackend::JOB_FINISHED, worker);
        return;
    }
    catch(const SireError::exception &e)
    {
        this->sendResponse( MPIBackend::JOB_STOPPED, MPIError(e) );
    }
    catch(const std::exception &e)
    {
        this->sendResponse( MPIBackend::JOB_STOPPED, MPIError(e) );
    }
    catch(...)
    {
        this->sendResponse( MPIBackend::JOB_STOPPED,
                                MPIError(SireError::unknown_exception( QObject::tr(
                                    "An unknown error has occurred!"), CODELOC ) ) );
    }
}

/** This is the function run by the MPI backend that actually processes
    the MPIWorker object that is sent by the master node, and performs
    the work contained in that worker. Once the work is complete, it
    then repacks that worker and sends it back to the master */
void MPIBackendPvt::run()
{
    try
    {
        do
        {
            //is there a message to receive?
            if (recv_comm.Iprobe(0, 0))
            {
                //yes there is - receive it
                int envelope[2];
                recv_comm.Recv(envelope, 2, MPI::INT, 0, 0);

                QByteArray worker_data;

                if (envelope[1] > 0)
                {
                    //there is an accompanying bytearray package
                    worker_data = QByteArray(envelope[1]+1, ' ');
                    recv_comm.Recv(worker_data.data(), envelope[1], 
                                   MPI::BYTE, 0, 0);
                }

                if (envelope[0] != MPIBackend::START_WORK)
                {
                    //this is an old message from a previous job - ignore it
                    continue;
                }
                       
                //extract the worker
                tuple< shared_ptr<void>,QString > object 
                                            = SireStream::load(worker_data);
                                                                                
                //assume that this is the right type
                MPIWorker *worker = static_cast<MPIWorker*>( object.get<0>().get() );

                if (worker)
                    this->runJob(*worker);
            }
            else
            {
                //wait a little before checking again
                if (keep_running)
                    QThread::msleep(5000);
            }

        } while (keep_running);
    }
    catch(const SireError::exception &e)
    {
        SireError::printError(e);
    }
    catch(const std::exception &e)
    {
        SireError::printError(SireError::std_exception(e));
    }
    catch(...)
    {
        SireError::printError( 
                 SireError::unknown_exception("An unknown error occured!", CODELOC));
    }
}

#else // ifdef __SIRE_USE_MPI__

//////////////
//////////////
// MPI disabled version of these classes
//////////////
//////////////

#error No non-MPI implementation of MPIBackend or MPIBackends is available!

#endif // ifdef __SIRE_USE_MPI__
