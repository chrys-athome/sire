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
#include "mpiworker.h"

#include "SireStream/streamdata.hpp"

#include <QMutex>
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

//////////////
//////////////
// MPI enabled version of these classes
//////////////
//////////////

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

    /** The global communicator used to 
        communicate with all of the nodes in this 
        set of MPINodes */
    MPI::Comm *comm_world;
    
    /** The registry of all active backends */
    QHash< tuple<int,int>, shared_ptr<MPIBackend> > active_backends;
    
    /** The list of all backends that are in the process of shutting down */
    QList< shared_ptr<MPIBackend> > stopped_backends;
};

/** Private implementation of MPIBackendPvt */
class MPIBackendPvt : public QThread
{
public:
    MPIBackendPvt() : mpitag(0), keep_running(true)
    {}
    
    ~MPIBackendPvt()
    {
        d->keep_running = false;

        //give the thread 10 seconds to exit
        if (not QThread::wait(10000))
        {
            QThread::terminate();
        }
        
        recv_mpicom.Free();
        send_mpicom.Free();
    }
    
    /** The communicator used to receive instructions 
        from the master */
    MPI::Intracomm recv_comm;
    
    /** The communicator used to send data back to the master */
    MPI::Intracomm send_comm;
    
    /** The tag of the channel to use for all communications */
    int mpitag;
    
    /** Whether or not to keep looping */
    bool keep_running;

protected:
    void run();
};

} // end of namespace detail
} // end of namespace SireMPI

using namespace SireMPI::detail;

static QHash<void*, MPIBackends*> backends_registry;

Q_GLOBAL_STATIC( QMutex, registryMutex );

/** Return the backends manager for the collection of nodes in 'nodes' */
MPIBackends SIREMPI_EXPORT &getBackends(const MPINodes &nodes)
{
    QMutex *mutex = registryMutex();
    
    QMutexLocker lkr(mutex);
    
    if ( not backends_registry.contains(nodes.communicator()) )
    {
        backends_registry.insert( nodes.communicator(),
                                  shared_ptr<MPIBackends>(new MPIBackends(nodes)) );
    }
    
    return *( backends_registry.value(nodes.communicator()) );
}

////////
//////// Implementation of MPIBackends
////////

/** Null constructor */
MPIBackends::MPIBackends() : d( new MPINodesPvt() )
{}

/** Construct to manage the backends on this node for the group of nodes
    in 'nodes' */
MPIBackends::MPIBackends(const MPINodes &nodes)
            : d( new MPINodesPvt() )
{
    d->comm_world = static_cast<MPI::Comm*>( nodes.communicator() );
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

/** Private function that contains an event loop that processes
    all of the events on this node that involves the collection
    of nodes 'nodes' */
int MPIBackends::_pvt_exec(MPINodes &nodes)
{
    QMutexLocker lkr( execmutex );

    BOOST_ASSERT( MPI::Is_initialized() );
    BOOST_ASSERT( d->comm_world != 0 );
   
    //get our rank in the MPINodes communicator
    int my_mpirank = d->comm_world->Get_rank();
    
    //now go into a loop waiting for instructions
    bool keep_looping = true;

    int master_rank = 0;

    do
    {   
        try
        {
            MPI::Status status;
            int message[3];
            
            if (my_mpirank == master_rank)
            {
                //wait for the message to be broadcast
                ... - (copy from thread - need QWaitCondition)
            }
            
            //wait for instructions that are broadcast to all nodes
            //from node 0
            d->comm_world.Bcast( message, 3, MPI::INT, master_rank );

            //what is the message?
            switch( message[0] )
            {
                case START_MPI_BACKEND:
                    //start a new MPI backend loop that listens to instructions
                    //on the specified channel from the specified master
                    if (message[1] == my_mpirank)
                    {
                        //this is a request to start the backend on this node
                        this->start(master_rank, message[2]);
                    }
                    else
                    {
                        //this node will not be involved
                        MPI::Intracomm null_comm = comm_world.split(0,0);
                        null_comm.Free();
                    }
                    
                    break;
        
                case STOP_MPI_BACKEND:
                    //stop the specified MPI backend loop
                    if (message[1] == my_mpirank)
                    {
                        this->stop( master_rank, message[2] );
                    }
                    
                    break;
                
                case SHUTDOWN_MPI:
                    //completely shut down MPI on this node
                    if (message[1] == my_mpirank or message[1] == -1)
                    {
                        this->stopAll();
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
            SireMPI::writeErrorString(CODELOC, e);
        }
        catch(const std::exception &e)
        {
            SireMPI::writeErrorString(CODELOC, SireError::std_exception(e));
        }
        catch(...)
        {
            SireMPI::writeErrorString(CODELOC, 
                    SireError::unknown_exception("An unknown error occured!", 
                                                  CODELOC));
        }

    } while (keep_looping);

    return 0;
}

/** Enter the MPI event processing loop to process the events
    of the nodes in 'nodes' */
int MPIBackends::exec(MPINodes &nodes)
{
    //get the backends for this collection of nodes
    MPIBackends &backends = getBackends(nodes);
    return backends._pvt_exec(nodes);
}

/** This class private class is used to run SireMPI::exec in a background
    thread - this is useful for running it as a background thread of
    the master process */
class SireMPI_ExecRunner : private QThread
{
public:
    SireMPI_ExecRunner() : QThread()
    {}
    
    void start(MPINodes &mpinodes)
    {
        nodes = mpinodes;
        QThread::start();
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
        try
        {
            MPIBackends::exec(nodes);
            return;
        }
        catch(const SireError::exception &e)
        {
            SireMPI::writeErrorString(CODELOC, e);
        }
        catch(const std::exception &e)
        {
            SireMPI::writeErrorString(CODELOC, SireError::std_exception(e));
        }
        catch(...)
        {
            SireMPI::writeErrorString(CODELOC, 
                    SireError::unknown_exception("An unknown error occured!", 
                                                    CODELOC));
        }
    }
    
private:
    MPINodes nodes;
};

Q_GLOBAL_STATIC( SireMPI_ExecRunner, execRunner );

/** Enter the MPI event processing loop, but run the loop in 
    a background thread */
void MPIBackends::execBG(MPINodes &nodes)
{
    //use an object to start and stop the loop
    //(as hopefully the object will be deleted when the library
    // exits, and thus an MPI shutdown will be called)
    SireMPI_ExecRunner *runner = execRunner();
    
    if (runner == 0)
        //we are in the process of being destroyed
        return;
        
    if (runner->isRunning())
        return;
        
    runner->start(nodes);
}

/** Shutdown all of the backends associated with the nodes 'nodes' */
void MPIBackends::shutdown(MPINodes &nodes)
{
    //get the backends for this collection of nodes
    MPIBackends &backends = getBackends(nodes);
    backends.broadcastMessage( SHUTDOWN_MPI );
}

/** Start a backend that receives instructions from the node with rank 'master'
    in the MPI communicator, using the channel with tag 'tag' for communication */
void MPIBackends::start(int master, int tag)
{
    BOOST_ASSERT( d->comm_world.get() != 0 );

    QMutexLocker lkr( &(d->datamutex) );

    //we shouldn't have this tag already...
    if ( d->registry.contains( tuple<int,int>(master,tag) ) )
        throw SireError::program_bug( QObject::tr(
            "It should not be possible to start two backends with the "
            "same master (%1) and tag (%2)").arg(master).arg(tag), CODELOC );

    //create a backend (this creates the necessary communicators)
    shared_ptr<MPIBackend> backend( new MPIBackend(d->comm_world, tag) );
    
    //start the thread
    backend->start();
    
    registry.insert( tuple<int,int>(master,tag), backend );
}

/** Stop the backend that is performing the work directed by the 
    node with rank 'master' in the MPI communicator, and which communicates
    using the channel with tag 'tag' */
void MPIBackends::stop(int master, int tag)
{
    QMutexLocker lkr( &(d->datamutex) );

    shared_ptr<MPIBackend> backend = d->registry.take(tuple<int,int>(master_rank,tag) );
    
    if (backend.get() != 0)
    {
        //stop the backend
        backend->keep_running = false;
        
        //add this backend to the list of ones that have been stopped
        d->stopped_backends.append(backend);
    }
}

/** Stop all of the backends that are running on this node using
    this MPI communicator */
void MPIBackends::stopAll()
{
    QMutexLocker lkr( &(d->datamutex) );
    
    QList< tuple<int,int> > tags = d->registry.keys();
    
    foreach (tuple<int,int> tag, tags)
    {
        shared_ptr<MPIBackend> backend = d->registry.take(tag);
        backend->keep_running = false;
        
        d->stopped_backends.append(backend);
    }
    
    //now wait for them all to finish
    while (not d->stopped_backends.isEmpty())
    {
        shared_ptr<MPIBackend> backend = d->stopped_backends.takeFirst();
        backend->wait();
    }
}

////////
//////// Implementation of MPIBackend
////////

/** Construct a backend to perform the work given to it by a master
    node (in the communicator 'communicator'), tagging all messages
    using the tag 'mpitag' */
MPIBackend::MPIBackend(void *communicator, int mpitag)
           : d(new MPIBackendPvt())
{
    BOOST_ASSERT( communicator != 0) ;

    d->mpitag = mpitag;
    
    //split the communicator so it just contains this node and the
    //master node - this is rank 1, the master is rank 0
    MPI::Comm *comm_world = static_cast<MPI::Comm*>(communicator);
    d->recv_mpicom = comm_world->Split(mpitag, 1);
    d->send_mpicom = recv_mpicom.Clone();
}

/** Destructor */
MPIBackend::~MPIBackend()
{}

/** Start this backend */
void MPIBackend::start()
{
    d->start();
}

/** Small function used to send the MPIWorker back to the master */
void MPIBackend::sendResponse(int response, const MPIWorker &worker)
{
    int envelope[3];
    envelope[0] = response;

    QByteArray worker_data;
    double current_progress = 0;

    if (response == MPIWORKER_RESULT or response == MPIWORKER_ABORT )
    {
        envelope[2] = 0;
    }
    else
    {
        envelope[2] = 1;
        current_progress = worker.progress();
    }

    if (response == MPIWORKER_PROGRESS or response == MPIWORKER_ABORT)
    {
        envelope[1] = 0;
    }
    else
    {
        worker_data = SireStream::save(worker);
        envelope[1] = worker_data.size();
    }
    
    //send the envelope describing the response
    d->send_mpicom.Send(envelope, 3, MPI::INT, 0, d->mpitag);
    
    if (envelope[1] > 0)
    {
        d->send_mpicom.Send(worker_data.constData(), worker_data.size(), 
                            MPI::BYTE, 0, mpitag);
    }
    
    if (envelope[2] > 0)
    {
        d->send_mpicom.Send(&current_progress, 1, MPI::DOUBLE, 0, mpitag);
    }
}

/** Run the job contained in the passed worker */
void MPIBackend::runJob(MPIWorker &worker)
{
    try
    {
        while (not worker.hasFinished())
        {
            //do we have an instruction?
            if (d->recv_mpicom.Iprobe(0, d->mpitag))
            {
                //yes there is - receive it
                int instruction;
                d->recv_mpicom.Recv(&instruction, 1, MPI::INT, 0, d->mpitag);
            
                switch (instruction)
                {
                    case MPIWorker::STOP:
                        this->sendResponse(MPIWorker::STOP, worker);
                        return;
                    
                    case MPIWorker::ABORT:
                        this->sendResponse(MPIWorker::ABORT, worker);
                        return;
                        
                    case MPIWorker::PROGRESS:
                        this->sendResponse(MPIWorker::PROGRESS, worker);
                        break; 
                        
                    case MPIWorker::INTERIM:
                        this->sendResponse(MPIWorker::INTERIM, worker);
                        break;
                }
            }

            if (not d->keep_running)
                return;

            //now run a chunk of work
            worker.runChunk();
        }
        
        this->sendResponse(MPIWorker::RESULT, worker);
        return;
    }
    catch(const SireError::exception &e)
    {
        this->sendResponse( MPIWorker::STOP, MPIError(e) );
    }
    catch(const std::exception &e)
    {
        this->sendResponse( MPIWorker::STOP, MPIError(e) );
    }
    catch(...)
    {
        this->sendResponse( MPIWorker::STOP,
                                MPIError(SireError::unknown_exception( QObject::tr(
                                    "An unknown error has occurred!"), CODELOC ) ) );
    }
}

static void writeErrorString(const QString &location, 
                             const SireError::exception &e)
{
    QTextStream ts(stdout);
    
    ts << QObject::tr(" ** FATAL ERROR ON NODE %1 **\n"
                      " ** LOCATION %2 **\n%3\n")
                .arg(comm_world->Get_rank())
                .arg(location, e.toString());
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
            if (d->recv_mpicom.Iprobe(0, d->mpitag))
            {
                //yes there is - receive it
                int envelope[2];
                d->recv_mpicom.Recv(envelope, 2, MPI::INT, 0, d->mpitag);

                QByteArray worker_data;

                if (envelope[1] > 0)
                {
                    //there is an accompanying bytearray package
                    worker_data = QByteArray(envelope[1]+1, ' ');
                    d->recv_mpicom.Recv(worker_data.data(), envelope[1], 
                                        MPI::BYTE, 0, d->mpitag);
                }

                if (envelope[0] != MPIWORKER_START)
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
                if (d->keep_running)
                    QThread::msleep(5000);
            }

        } while (d->keep_running);
    }
    catch(const SireError::exception &e)
    {
        ::writeErrorString(CODELOC, e);
    }
    catch(const std::exception &e)
    {
        ::writeErrorString(CODELOC, SireError::std_exception(e));
    }
    catch(...)
    {
        ::writeErrorString(CODELOC, 
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
