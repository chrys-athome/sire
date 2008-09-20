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

#include <boost/weak_ptr.hpp>

#include "mpinode.h"
#include "mpinodes.h"

#include "detail/mpidetail.h"     // CONDITIONAL_INCLUDE

#include "SireStream/streamdata.hpp"
#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include "SireError/errors.h"

#include <QDebug>

using namespace SireMPI;
using namespace SireMPI::detail;

using namespace SireStream;

static const int SHUTDOWN_MPI = 0;
static const int START_MPI_BACKEND = 1;
static const int STOP_MPI_BACKEND = 2;

//////////
////////// Implementation of MPIBackend
//////////

#ifdef __SIRE_USE_MPI__

    namespace SireMPI
    {

    class MPIBackend : private QThread
    {
    public:
        ~MPIBackend();

        static void start(MPI::Comm *mpicom, int master, int tag);
        static void stop(MPI::Comm *mpicom, int master, int tag);

        static void stopAll();

    protected:
        MPIBackend(MPI::Comm *mpicom, int master_rank, int mpitag);
    
        void run();
    
        static QHash< int, shared_ptr<MPIBackend> > registry;
        static QList< shared_ptr<MPIBackend> > stopped_backends;
    
        /** The communicator used to talk to the master */
        MPI::Comm *mpicom;
    
        /** The rank of the master process in the communicator */
        int master_rank;
        
        /** The tag that will be used for all communication */
        int mpitag;
    
        /** flag used to stop the backend */
        bool keep_running;
    };

    /** The registry of all active backends on this node */
    QHash< int, shared_ptr<MPIBackend> MPIBackend::registry;
    
    /** Constructor, telling the background the rank of the master
        process, the communicator used to communicate with that
        process, and the  */
    MPIBackend::MPIBackend() : QThread()
    {}
    
    /** Destructor */
    MPIBackend::~MPIBackend()
    {
        keep_running = false;
        
        //give the thread 10 seconds to exit
        if (not QThread::wait(10000))
        {
            QThread::terminate();
        }
    }
    
    /** Small function used to send the MPIWorker back to the master */
    void MPIBackend::sendResponse(const MPIWorker &worker)
    {
        QByteArray worker_data = SireStream::save(worker);
        mpicom->Send(worker.constData(), worker.size(), MPI::BYTE,
                     master_rank, mpitag);
    }
    
    /** This is the function run by the MPI backend that actually processes
        the MPIWorker object that is sent by the master node, and performs
        the work contained in that worker. Once the work is complete, it
        then repacks that worker and sends it back to the master */
    void MPIBackend::run()
    {
        do
        {
            try
            {
                //is there a message to receive?
                MPI::Status status;
            
                if (mpicom->Iprobe(master_rank, mpitag, status))
                {
                    //yes there is! - get the size
                    int size = status.Get_count(MPI::BYTE);
                    QByteArray worker_data(size+1, ' ');
                
                    //now receive the data
                    mpicom->Recv(worker_data.data(), size, MPI::BYTE, 
                                 master_rank, mpitag);
                                     
                    //extract the worker
                    tuple< shared_ptr<void>,QString > object 
                                                = SireStream::load(worker_data);
                                                
                    //assume that this is the right type
                    MPIWorker *worker = static_cast<MPIWorker*>( object.get<0>().get() );
                    
                    if (worker)
                    {
                        //run the backend loop of the worker - this stores
                        //the result of the work within itself
                        worker->runBackend();
                        
                        //send the worker back to the master
                        this->sendResponse(*worker);
                    }
                }
                else
                {
                    //wait a little before checking again
                    QThread::msleep(500);
                }
            }
            catch(const SireError::exception &e)
            {
                this->sendResponse( MPIError(e) );
            }
            catch(const std::exception &e)
            {
                this->sendResponse( MPIError(e) );
            }
            catch(...)
            {
                this->sendResponse( MPIError(SireError::unknown_exception( QObject::tr(
                                       "An unknown error has occurred!"), CODELOC ) ) );
            }

        } while (keep_running);
    }

    /** Start a backend */
    void MPIBackend::start(MPI::Comm *mpicom, int master, int tag)
    {
        //we shouldn't have this tag already...
        if (registry.contains(tag))
            throw SireError::program_bug( QObject::tr(
                "It should not be possible to start two backends with the "
                "same tag (%1)").arg(tag), CODELOC );
    
        //create a backend
        shared_ptr<MPIBackend> backend( new MPIBackend(mpicom, master, tag) );
        
        //start the thread
        backend->start();
        
        //add it to the registry
        registry.insert( tag, backend );
    }
    
    /** Stop the backend with the specified tag */
    void MPIBackend::stop(MPI::Comm *mpicom, int master, int tag)
    {
        shared_ptr<MPIBackend> backend = registry.take(tag);
        
        if (backend.get() != 0)
        {
            if (mpicom != backend->mpicom)
                qWarning() << CODELOC << "\n\nMix up in MPI::Comm at shutdown...";
                
            if (master != backend->master_rank)
                qWarning() << CODELOC 
                           << "\n\nMix up in MPI rank at shutdown...";

            //stop the backend
            backend->keep_running = false;
            
            //add this backend to the list of ones that have been stopped
            stopped_backends.append(backend);
        }
    }

    void MPIBackend::stopAll()
    {
        //stop all backends
        QList<int> tags = registry.keys();
        
        foreach (int tag, tags)
        {
            shared_ptr<MPIBackend> backend = registry.take(tag);
            backend->keep_running = false;
            
            stopped_backends.append(backend);
        }
        
        //now wait for them all to finish
        while (not stopped_backends.isEmpty())
        {
            shared_ptr<MPIBackend> backend = stopped_backends.takeFirst();
            backend->wait();
        }
    }

    /** Global mutex to protect access to 'exec_is_running' */
    static QMutex exec_mutex;

    /** This variable is used to see if an MPI loop (from SireMPI::exec) is 
        currently running */
    static bool exec_is_running(false);

    /** Return whether SireMPI::exec is running */
    bool SIREMPI_EXPORT exec_running()
    {
        QMutexLocker lkr(&exec_mutex);
        return exec_is_running;
    }

    /** This is the main function that you must call to allow SireMPI
        to work in your process. You must only call this once in a process
        - you cannot call it several times!!! */
    int SIREMPI_EXPORT exec(int argc, const char **argv)
    {
        exec_mutex.lock();
    
        if (exec_is_running)
        {
            exec_mutex.unlock();
            return 0;
        }
        else
        {
            exec_is_running = true;
            exec_mutex.unlock();
        }

        if (not MPI::Is_initialized())
        {
            MPI::Init(argc, argv);
        }
    
        //now go into a loop waiting for instructions
        bool keep_looping = true;
    
        do
        {   
            try
            {
                int message[2];
        
                //listen for any messages on channel 1
                MPI::Status status;
                MPI::Comm_world.Recv( message, 2, MPI_ANY_SOURCE, 1, status );
        
                //what is the message?
                switch( message[0] )
                {
                    case START_MPI_BACKEND:
                        //start a new MPI backend loop that listens to instructions
                        //on the specified channel from the specified master
                        MPIBackend::start( &(MPI::Comm_world),
                                           status.Get_source(), message[1] );
                        break;
            
                    case STOP_MPI_BACKEND:
                        //stop the specified MPI backend loop
                        MPIBackend::stop( &(MPI::Comm_world),
                                          status.Get_source(), message[1] );
                        break;
                    
                    case SHUTDOWN_MPI:
                        //completely shut down MPI on this node
                        MPIBackend::stopAll();
                        keep_looping = false;
                        break;
                    
                    default:
                        qWarning() << "Could not interpret MPI instruction" << message[0];
                        break;
                }
            }
            catch(const SireError::exception &e)
            {
                qWarning() << "An error occured in SireMPI::exec()\n"
                           << e.toString();
            }
            catch(const std::exception &e)
            {
                qWarning() << "An error occured in SireMPI::exec()\n"
                           << SireError::std_exception(e).toString();
            }
            catch(...)
            {
                qWarning() << "An unknown error occured in SireMPI::exec()\n";
            }
    
        } while (keep_looping);
    
        //ok, we have finished - call MPI::Finalize
        MPI::Finalize();
    
        exec_mutex.lock();
        exec_is_running = false;
        exec_mutex.unlock();
    
        return 0;
    }

    /** This class private class is used to run SireMPI::exec in a background
        thread - this is useful for running it as a background thread of
        the master process */
    class SireMPI_ExecRunner : private QThread
    {
    public:
        SireMPI_ExecRunner(int argc, const char **argv) : QThread()
        {
            exec_mutex.lock();
        
            if (not MPI::Is_initialized())
                MPI::Init(argc, argv);
            
            exec_mutex.unlock();
        
            this->start();
        }
    
        ~SireMPI()
        {}
    
    protected:
        void run()
        {
            SireMPI::exec(0,0);
        }
    };

    /** Call SireMPI::exec, but set it running in a background thread. You should
        really only do this on the master node */
    void SIREMPI_EXPORT bg_exec(int argc, const char **argv)
    {
        new SireMPI_ExecRunner(argc, argv);
    }

    /** Call this at the end of the program (on the master node) to shutdown
        all of the MPI nodes */
    void SIREMPI_EXPORT shutdown()
    {
        if (this->isRunning())
        {
            //shutdown each node (except ourselves!)
            int nnodes = MPI::COMM_WORLD.Get_size();
            int my_rank = MPI::COMM_WORLD.Get_rank();
            
            int message[2];
            message[0] = SHUTDOWN_MPI;
            message[1] = 0;
            
            for (int i=0; i<nnodes; ++i)
            {
                if (i != my_rank)
                {
                    MPI::COMM_WORLD.Send(message, 2, MPI::INT, i, 1);
                }
            }
            
            //now shutdown this node
            MPI::COMM_WORLD.Send(message, 2, MPI::INT, my_rank, 1);
        }
    }

    } // end of namespace SireMPI

#else

    #error Need to write non-mpi versions!!!

#endif

//////////
////////// Implementation of MPINodeData
//////////

/** Null constructor */
MPINodeData::MPINodeData() : QThread(), is_master(false)
{
    #ifdef __SIRE_USE_MPI__
        mpirank = 0;
    #endif
}

/** Function used to start the code necessary on the backend node
    so that this processor can communicate with the backend */
void MPINodeData::startBackend()
{
    #ifdef __SIRE_USE_MPI__
        //tell the node to start a new backend loop using the provided
        //tag for all communication

        boost::shared_ptr<MPINodesData> communicator = parent.lock();
    
        BOOST_ASSERT( communicator.get() != 0 );

        MPI::Comm *mpicomm = communicator->mpiCommunicator();
        int mpitag = communicator->mpiTag();

        int message[2];
        message[0] = START_MPI_BACKEND;
        message[1] = communicator->mpiTag();

        mpicomm->Send(message, 2, MPI::INT, mpirank, 1); 

    #else
        #error Need to write this
    #endif
}

/** This function tells the backend node that we won't be communicating any more,
    so to stop the backend loop */
void MPINodeData::stopBackend()
{
    #ifdef __SIRE_USE_MPI__
        //tell the node to stop the backend loop

        boost::shared_ptr<MPINodesData> communicator = parent.lock();
    
        BOOST_ASSERT( communicator.get() != 0 );

        MPI::Comm *mpicomm = communicator->mpiCommunicator();
        int mpitag = communicator->mpiTag();

        int message[2];
        message[0] = STOP_MPI_BACKEND;
        message[1] = communicator->mpiTag();

        mpicomm->Send(message, 2, MPI::INT, mpirank, 1); 
    
    #else
        #error Need to write this
    
    #endif
}

/** This constructor is called only by the MPINodesData communicator, and is
    used to create a new MPINode with specified communicator, processor rank
    in that communicator, and whether or not this is the master processor
    for the communicator */
MPINodeData::MPINodeData(const boost::shared_ptr<MPINodesData> &communicator, 
                         int processor_rank, bool ismaster)
            : QThread(),
              parent(communicator),
              uid( QUuid::createUuid() ), 
              is_master(ismaster)
{
    BOOST_ASSERT( not parent.expired() );
    BOOST_ASSERT( processor_rank >= 0 );
    
    #ifdef __SIRE_USE_MPI__
        mpirank = processor_rank;
    #endif

    this->startBackend();
}

/** This copy constructor is used when the node is returned to the 
    list of free nodes in the communicator */
MPINodeData::MPINodeData(const MPINodeData &other)
            : QThread(),
              parent(other.parent), uid(other.uid),
              is_master(other.is_master)
{
    #ifdef __SIRE_USE_MPI__
        mpirank = other.mpirank;
    #endif
}

/** Destructor - this blocks until the node is not busy, and then
    it either completely deletes the node, or posts a new copy
    back to the list of free nodes in the communicator */
MPINodeData::~MPINodeData()
{
    //wait until this node is no longer busy...
    this->wait();

    boost::shared_ptr<MPINodesData> communicator = parent.lock();

    if (communicator.get() != 0)
    {
        //create a null copy of this data and return the node
        boost::shared_ptr<MPINodeData> data_copy( new MPINodeData(*this) );
        
        communicator->returnNode( MPINode(data_copy) );
    }
}

/** Return whether or not the node is busy (processing an MPIWorker) */
bool MPINodeData::isBusy() const
{
    return QThread::isRunning();
}

/** Block until the node is no longer busy */
void MPINodeData::wait()
{
    QThread::wait();
}

/** Block until the node is no longer busy, or time ms have passed.
    Returns whether or not the node is busy */
bool MPINodeData::wait(int time)
{
    return QThread::wait(time);
}

/** Start and run the job that is contained in the passed worker */
MPIPromise MPINodeData::start(const MPIWorker &worker)
{
    if (this->isNull())
        throw SireMPI::broken_node( QObject::tr(
            "You cannot run any work on the null MPI node."), CODELOC );

    QMutexLocker lkr(&data_mutex);

    //wait until the node is no longer busy
    while (this->isBusy())
    {
        lkr.unlock();
        this->wait();
        lkr.relock();
    }

    //save the worker to be processed
    mpipromise = MPIPromise(worker, MPINode(self_ptr.lock()));

    //reset the aborted status
    is_aborted = false;

    //now start the thread that is used to start the job and wait
    //for responses from the backend node
    start_mutex.lock();
    QThread::start();
    starter_waiter.wait( &starter_mutex );
}

/** Return whether or not the last job run on this node was aborted */
bool MPINodeData::wasAborted() const
{
    QMutexLocker lkr( const_cast<QMutex*>(&data_mutex) );
    return is_aborted;
}

/** This is what is run in the background to send a job to the backend 
    node and to wait for a response */
void MPINodeData::run()
{
    //tell the parent thread that we are now definitely running
    start_mutex.lock();
    starter_waiter.wakeAll();
    start_mutex.unlock();
    
    //copy the promise from the parent thread into a thread-local copy
    MPIPromise local_promise;
    
    {
        QMutexLocker lkr(&data_mutex);
        is_aborted = false;
        
        if (mpipromise.isNull())
            return;
            
        local_promise = mpipromise;
        
        if (is_aborted)
        {
            //we've been aborted before even starting!
            local_promise.aborted();
        }
    }

    //now send the data for the worker to the backend node
    #ifdef __SIRE_USE_MPI__
    
        shared_ptr<MPINodesData> communicator = parent.lock();
        BOOST_ASSERT( communicator.get() != 0 );
        
        MPI::Comm *mpicom = communicator->mpiCommunicator();
        int mpitag = communicator->mpiTag();
        
        BOOST_ASSERT( mpicom != 0 );
        BOOST_ASSERT( mpitag > 1 );
        
        QByteArray worker_data = local_promise.workerData();
        
        mpicom->Send( worker_data.constData(), worker_data.size(), MPI::BYTE,
                      mpirank, mpitag );
    
    #else
        #error Need to write this!
        
    #endif
    
    //now wait for the work to be complete - wait for the result
    //to be sent back
    
    QByteArray result_data;
    
    #ifdef __SIRE_USE_MPI__
    
        MPI::Status status;
        mpicom->Probe( mpicom, mpitag, status );
        
        //get the size of the response
        int count = status.Get_count(MPI::BYTE):
        
        //create space for the response
        result_data = QByteArray(count+1, ' ');
        
        //receive the response
        mpicom->Recv( result_data.data(), count, MPI::BYTE,
                      mpicom, mpitag );
                      
    #else
        #error Need to write this...
    #endif
    
    if (this->isAborted())
        local_promise.setAborted();

    else if ( SireStream::getTypeName(result_data) 
                    == QLatin1String(MPIError::typeName()) )
    {
        local_promise.setError(result_data);
    }
    else
        local_promise.setFinal(result_data);
}

/** Stop any jobs running on this node */
void MPINodeData::stop()
{
    QMutexLocker lkr(&data_mutex);
    
    if (not QThread::isRunning())
        //nothing to stop
        return;
        
    //send a message to the node telling it to stop
    this->sendMessage( MPIWORKER_STOP );
}

/** Abort any jobs running on this node */
void MPINodeData::abort()
{
    QMutexLocker lkr(&data_mutex);
    
    if (not QThread::isRunning())
        //nothing to abort
        return;

    if (is_aborted)
        //already aborted
        return;
        
    //send a message to the node telling it to abort
    this->sendMessage( MPIWORKER_ABORT );
    is_aborted = true;
}

//////////
////////// Implementation of MPINode
//////////

static boost::shared_ptr<MPINodeData> shared_null;

static const boost::shared_ptr<MPINodeData>& getSharedNull()
{
    if (shared_null.get() == 0)
    {
        shared_null.reset( new MPINodeData() );
    }
    
    return shared_null;
}

/** Null constructor */
MPINode::MPINode() : d( getSharedNull() )
{}

/** Construct an MPINode which is part of the passed communicator,
    with the specified rank, and saying whether or not this is 
    the master node of this communicator */
MPINode::MPINode(const MPINodes &communicator, int rank, bool is_master)
        : d( new MPINodeData(communicator.d, rank, is_master) )
{}

/** Private copy constructor */
MPINode::MPINode(const boost::shared_ptr<detail::MPINodeData> &data)
        : d( data )
{}

/** Copy constructor */
MPINode::MPINode(const MPINode &other)
        : d(other.d)
{}

/** Destructor */
MPINode::~MPINode()
{}

/** Copy assignment operator */
MPINode& MPINode::operator=(const MPINode &other)
{
    d = other.d;
    return *this;
}

/** Comparison operator */
bool MPINode::operator==(const MPINode &other) const
{
    return d.get() == other.d.get();
}

/** Comparison operator */
bool MPINode::operator!=(const MPINode &other) const
{
    return d.get() != other.d.get();
}

/** Return the unique ID for this MPI node */
const QUuid& MPINode::UID() const
{
    return d->uid;
}

/** Return whether or not this node is null */
bool MPINode::isNull() const
{
    return d->parent.expired() or d->uid.isNull();
}

/** Return this node to the pool of unused threads - this returns
    immediately, but the node will only be returned once it is no 
    longer busy, and no other copies of this MPINode exist */
void MPINode::returnNode()
{
    this->operator=( MPINode() );
}

/** Return the communicator in which this node belongs 

    \throw SireError::unavailable_resource
*/
MPINodes MPINode::communicator() const
{
    boost::shared_ptr<MPINodesData> parent = d->parent.lock();

    if (parent.get() == 0)
        throw SireError::unavailable_resource( QObject::tr(
            "A null MPINode doesn't have a communicator!"), CODELOC );
            
    return MPINodes(parent);
}

/** Return the rank of this node in its communicator */
int MPINode::rank() const
{
    #ifdef __SIRE_USE_MPI__
        return d->mpirank;
    #else
        return 0;
    #endif
}

/** Return whether this node is the master node for the communicator */
bool MPINode::isMaster() const
{
    return d->is_master;
}

/** Return whether or not this node is busy doing some work */
bool MPINode::isBusy() const
{
    return d->isBusy();
}
    
/** Wait until this node is no longer busy doing work */
void MPINode::wait()
{
    d->wait();
}

/** Wait until this node is no longer busy doing work, or until
    'time' milliseconds have passed. This returns whether or not
    the nodes is not busy */
bool MPINode::wait(int time)
{
    return d->wait(time);
}

/** Start and set the node working in the job contained in 'worker'. 
    This blocks until any other jobs running on this node have
    finished. This returns an MPIPromise that can be used to follow
    the progress of the work and get the result */
MPIPromise MPINode::start(const MPIWorker &worker)
{
    return d->start(worker);
}

/** This tells the node to stop performing any work that it may 
    be running. This stops the work at the next convienient moment
    and returns to the promise the result at the point of stopping.
    This does not block */
void MPINode::stop()
{
    d->stop();
}

/** This tells the node to abort any work it may be running. This
    stops the work at the next convienient moment and returns to
    the promise the work from before it started. This does not block. */
void MPINode::abort()
{
    d->abort();
}

/** Return whether the last (or current) job has been aborted */
bool MPINode::wasAborted() const
{
    return d->wasAborted();
}

/** Return the promise that represents the current work in progress.
    This will be a null promise if this node is not busy */
MPIPromise MPINode::workInProgress()
{
    return d->mpipromise;
}

/** Return the progress (percentage) of the current job. If no jobs
    have been run, then this is zero. If the last job has finished
    then this is 100 */
double MPINode::getProgress()
{
    return d->getProgress();
}

/** Return the interim result - this blocks until the interim result
    is available */
MPIPromise MPINode::getInterimResult()
{
    return d->getInterimResult();
}

/** Return the last result - if this is running then this blocks
    until the running job has finished */
MPIPromise MPINode::getLastResult()
{
    return d->getLastResult();
}
