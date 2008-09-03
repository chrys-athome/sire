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

#include "SireError/errors.h"

using namespace SireMove;
using namespace SireMove::detail;

//////////
////////// Implementation of MPINodeData
//////////

MPINodeData::MPINodeData() : is_busy(false), is_master(false)
{
    #ifdef __SIRE_USE_MPI__
        mpirank = 0;
        mpicomm = 0;
    #endif
}

MPINodeData::MPINodeData(const MPINodes &communicator, 
                         int processor_rank, bool ismaster)
            : parent(communicator.d),
              uid( QUuid::createUuid() ), 
              is_busy(false),
              is_master(ismaster)
{
    BOOST_ASSERT( not parent.expired() );
    BOOST_ASSERT( processor_rank >= 0 );
    
    #ifdef __SIRE_USE_MPI__
        mpirank = processor_rank;
        mpicomm = parent.lock()->mpicomm;
        
        BOOST_ASSERT( mpicomm != 0 );
    #endif
}

MPINodeData::MPINodeData(const MPINodeData &other)
            : parent(other.parent), uid(other.uid),
              is_busy(false), is_master(other.is_master)
{
    #ifdef __SIRE_USE_MPI__
        mpirank = other.mpirank;
        mpicomm = other.mpicomm;
    #endif
}

MPINodeData::~MPINodeData()
{
    //wait until this node is no longer busy...
    run_mutex.lock();
    data_mutex.lock();
    is_busy = false;
    data_mutex.unlock();
    run_mutex.unlock();

    boost::shared_ptr<MPINodesData> communicator = parent.lock();

    if (communicator.get() != 0)
    {
        //create a null copy of this data and return the node
        boost::shared_ptr<MPINodeData> data_copy( new MPINodeData(*this) );
        
        communicator->returnNode( MPINode(data_copy) );
    }
}

bool MPINodeData::isBusy() const
{
    QMutexLocker lkr( &(const_cast<MPINodeData*>(this)->data_mutex) );
    return is_busy;
}

void MPINodeData::wait()
{
    run_mutex.lock();
    run_mutex.unlock();
}

bool MPINodeData::wait(int time)
{
    if (run_mutex.tryLock(time))
    {
        run_mutex.unlock();
        return true;
    }
    else
        return false;
}

void MPINodeData::lock()
{
    run_mutex.lock();
    data_mutex.lock();
    is_busy = true;
    data_mutex.unlock();
}

void MPINodeData::unlock()
{
    data_mutex.lock();
    
    if (not is_busy)
    {
        data_mutex.unlock();
        return;
    }
    
    is_busy = false;
    data_mutex.unlock();
    run_mutex.unlock();
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
        : d( new MPINodeData(communicator, rank, is_master) )
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

/** Run a simulation involving 'nmoves' moves of the moves in 'moves' on
    the system 'system', with statistics recorded if 'record_stats' is true */
MPIPromise< tuple<System,Moves,qint32> > MPINode::runSim(const System &system,
                                                         const MovesBase &moves,
                                                         int nmoves, bool record_stats)
{
    return MPIPromise< tuple<System,Moves,qint32> >(
                MPIWorker::runSim( d.get(), system, moves, nmoves, record_stats ) );
}

///////////
/////////// Implementation of MPIWorker
///////////

MPIWorker::MPIWorker() : QThread(), boost::noncopyable()
{}

MPIWorker::~MPIWorker()
{
    QThread::wait();
}

/** Return whether or not the worker has finished */
bool MPIWorker::hasFinished()
{
    QMutexLocker lkr(&data_mutex);
    return current_status & MPIWORKER_IS_FINISHED;
}

/** Return whether or not the current result is an error */
bool MPIWorker::isError()
{
    QMutexLocker lkr(&data_mutex);
    return current_status & MPIWORKER_IS_ERROR;
}

/** Return whether or not the work has been reported */
bool MPIWorker::isAborted()
{
    QMutexLocker lkr(&data_mutex);
    return current_status & MPIWORKER_IS_ABORTED;
}

QByteArray MPIWorker::processResult()
{
    //we are already holding the data_mutex
    
    if ( current_status & MPIWORKER_IS_ABORTED )
    {
        return QByteArray();
    }
    else if ( current_status & MPIWORKER_IS_ERROR )
    {
        SireError::unpackAndThrow(returned_data);
    }
    else
    {
        //this is either the interim or final result...
        return returned_data;
    }
}

#ifdef __SIRE_USE_MPI__
    static void sendMessage(MPINodeData *nodedata, const void *message, int size,
                            const MPI::Datatype &datatype)
    {
        BOOST_ASSERT( nodedata != 0 );
        
        boost::shared_ptr<MPINodesData> nodesdata = nodedata->parent.lock();
        BOOST_ASSERT( nodesdata.get() != 0 );

        MPI::Comm *mpicomm = nodedatas->mpiCommunicator();
        BOOST_ASSERT( mpicomm != 0 );

        int mpirank = nodedata->mpirank;
    
        mpicomm->Isend( message, size, datatype, mpirank, 0 );
    }

    static void receiveMessage(MPINodeData *nodedata, void *message, int size,
                               const MPI::Datatype &datatype)
    {
        BOOST_ASSERT( nodedata != 0 );
        
        boost::shared_ptr<MPINodesData> nodesdata = nodedata->parent.lock();
        BOOST_ASSERT( nodesdata.get() != 0 );

        MPI::Comm *mpicomm = nodedatas->mpiCommunicator();
        BOOST_ASSERT( mpicomm != 0 );

        int mpirank = nodedata->mpirank;
        
        mpicomm->Recv( message, size, datatype, mpirank, 0 );
    }
#endif

void MPIWorker::sendMessage(int *message, int size)
{
    #ifdef __SIRE_USE_MPI__
        sendMessage(nodedata, message, size, MPI::INT);
    #else
        #error NOT IMPLEMENTED!!!
    #endif
}

void MPIWorker::sendMessage(const QByteArray &data)
{
    #ifdef __SIRE_USE_MPI__
        sendMessage(nodedata, data.constData(), data.count(), MPI::BYTE);
    #else
        #error NOT IMPLEMENTED!!!
    #endif
}

void MPIWorker::receiveMessage( int *message, int size )
{
    #ifdef __SIRE_USE_MPI__
        receiveMessage(nodedata, message, size, MPI::INT);
    #else
        #error NOT IMPLEMENTED!!!
    #endif
}

void MPIWorker::receiveMessage( QByteArray &data, int size )
{
    //ensure there is sufficient space to receive the message
    if (data.count() < size )
    {
        data = QByteArray( size + 1, " " );
    }
    
    #ifdef __SIRE_USE_MPI__
        receiveMessage(nodedata, data.data(), size, MPI::BYTE);
    #else
        #error NOT IMPLEMENTED!!!
    #endif
}

/** Return the interim result of the calculation - this blocks
    until an interim (or indeed final) result is available */
QByteArray MPIWorker::interimResult()
{
    QMutexLocker lkr(&data_mutex);

    if ( current_status & MPIWORKER_IS_FINISHED )
    {
        return this->processResult();
    }
    
    int message = MPIWORKER_INTERIM;
    this->sendMessage( &message );
    
    //wait for a result
    interim_waiter.wait( &data_mutex );
    
    return this->processResult();
}

/** Return the result of the work - this blocks until the work has finished,
    or there is an error, or the work is aborted */
QByteArray MPIWorker::result()
{
    QMutexLocker lkr(&data_mutex);
    
    if (not current_status & MPIWORKER_IS_FINISHED)
    {
        //wait for a result
        result_waiter.wait( &data_mutex );
    }
    
    return this->processResult();
}

/** Wait until the result is available - this blocks until the work has finished,
    or an error has occured, or the calculation was aborted */
void MPIWorker::wait()
{
    QMutexLocker lkr(&data_mutex);
    
    if (not current_status & MPIWORKER_IS_FINISHED)
    {
        //wait for the result
        result_waiter.wait( &data_mutex );
    }
}

/** Wait until the result is available, or 'ms' milliseconds has passed. This
    blocks until the work has finished (by either a successful completion, 
    error being thrown or calculation aborted) or until 'ms' milliseconds has
    passed. If the work finishes within the time, then 'true' is returned. */
bool MPIWorker::wait(int ms)
{
    QMutexLocker lkr(&data_mutex);
    
    if (not current_status & MPIWORKER_IS_FINISHED)
    {
        //wait for the result
        return result_waiter.wait( &data_mutex, ms );
    }
    else
        return true;
}

/** Abort the work if it is still running. This blocks until the 
    process has aborted */
void MPIWorker::abort()
{
    QMutexLocker lkr(&data_mutex);
    
    if (not current_status & MPIWORKER_IS_FINISHED)
    {
        int message = MPIWORKER_ABORTED;
        this->sendMessage( &message );
        
        //wait for a response
        result_waiter.wait(&data_mutex);
    }
}

/** Stop the work. This stops the work at its current point. This may abort
    the job, or may stop the job at an interim state, depending on how far
    along the work is, and whether or not interim states are possible */
void MPIWorker::stop()
{
    QMutexLocker lkr(&data_mutex);
    
    if (not current_status & MPIWORKER_IS_FINISHED)
    {
        int message = MPIWORKER_STOPPED;
        this->sendMessage( &message );
        
        //wait for a response
        result_waiter.wait(&data_mutex);
    }
}

/** Return the progress of the simulation - this blocks until the progress
    has been returned by the back-end */
double MPIWorker::progress()
{
    QMutexLocker lkr(&data_mutex);
    
    if (current_status & MPIWORKER_IS_FINISHED)
    {
        return current_progress;
    }
    
    //ask for the current progress
    int message = MPIWORKER_PROGRESS;
    this->sendMessage( &message );
    
    //wait for a response
    progress_waiter.wait( &data_mutex );
    
    //return the current progress
    return current_progress;
}

boost::shared_ptr<MPIWorker> MPIWorker::runSim(MPINodeData *nodedata,
                                               const System &system,
                                               const MovesBase &moves,
                                               int nmoves, bool record_stats)
{
    boost::shared_ptr<MPIWorker> worker( new MPIWorker() );

    if (nodedata == 0)
        return worker;

    //serialise the arguments to binary data
    QByteArray args_data;

    // write in brackets so that the datastream device is deleted
    // once the arguments have been written
    {
        QDataStream ds(&args_data, QIODevice::WriteOnly);
     
        ds << QString("runSim_0") << system << Moves(moves)
           << qint32(nmoves) << record_stats;
    }
        
    //copy the data into the worker
    worker->nodedata = nodedata;
    worker->args_data = args_data;

    //start the worker and wait until it has definitely started
    worker->start_mutex.lock();
    worker->start();
    worker->start_waiter.wait( &(worker->start_mutex) );
    worker->start_mutex.unlock();

    return worker;
}

void MPIWorker::setError(const QByteArray &error_data)
{
    QMutexLocker lkr(&data_mutex);
    
    current_status = MPIWORKER_IS_ERROR | MPIWORKER_IS_FINISHED;
    returned_data = error_data;
    current_progress = 0.0;
    
    //wake any threads waiting for a result
    interim_waiter.wakeAll();
    result_waiter.wakeAll();
}

void MPIWorker::setInterim(const QByteArray &result_data)
{
    QMutexLocker lkr(&data_mutex);
    
    current_status = MPIWORKER_IS_BUSY;
    returned_data = result_data;
    
    //wake any threads waiting for an interim result
    interim_waiter.wakeAll();
}

void MPIWorker::setResult(const QByteArray &result_data)
{
    QMutexLocker lkr(&data_mutex);
    
    current_status = MPIWORKER_IS_FINISHED;
    returned_data = result_data;
    current_progress = 100.0;
    
    interim_waiter.wakeAll();
    result_waiter.wakeAll();
}

void MPIWorker::setProgress(const QByteArray &progress_data)
{
    QMutexLocker lkr(&data_mutex);
    
    QDataStream ds(progress_data);
    ds >> current_progress;
    
    progress_waiter.wakeAll();
}

void MPIWorker::setAborted()
{
    QMutexLocker lkr(&data_mutex);
    
    current_status = MPIWORKER_IS_ABORTED | MPIWORKER_IS_FINISHED;
    returned_data = QByteArray();
    current_progress = 0.0;
    
    progress_waiter.wakeAll();
    interim_waiter.wakeAll();
    result_waiter.wakeAll();
}

/** This function is used by the backend node to send a reply to the master */
void MPIWorker::sendReply(MPI::Comm *mpicomm, int mpimaster, int message,
                          const QByteArray &arguments)
{
    BOOST_ASSERT( mpicomm != 0 );
    
    int envelope[2];
    
    envelope[0] = message;
    envelope[1] = arguments.count();
    
    mpicomm->Send( &envelope, 2, MPI::INT, mpimaster, 0 );
    
    if (arguments.count() > 0)
    {
        mpicomm->Send( arguments.constData(), arguments.count(), MPI::BYTE,
                       mpimaster, 0 );
    }
}

/** This runs a simulation on the back-end node. This returns whether or
    not to remain in the current MPI mode (SireMove::MPIWorker) */
bool MPIWorker::runSim_0(MPI::Comm *mpicomm, int mpimaster,
                         QDataStream &ds)
{
    //extract the arguments to this function
    System system;
    Moves moves;
    qint32 nmoves;
    bool record_stats;
    
    ds >> system >> moves >> nmoves >> record_stats;
    
    //now construct a simulation to run these moves
    Simulation simulation = Simulation::runBG(system, moves, nmoves, record_stats);
    
    //enter a loop, checking for new instructions, and waiting until the 
    //simulation has finished
    while (true)
    {
        if (mpicomm->Iprobe(mpimaster, 0))
        {
            //there is a message to be received
            int message;
            mpicomm->Recv(&message, 1, MPI::INT, mpimaster, 0);
            
            if (message == 0)
            {
                //we have been told to exit from this mode immediately
                simulation.abort();
                simulation.wait();
                return false;
            }
            
            QByteArray response;
            
            switch (message)
            {
                case MPIWORKER_ABORTED:
                    //abort this job
                    simulation.abort();
                    simulation.wait();
                    MPIWorker::sendReply(mpicomm, mpimaster, MPIWORKER_ABORTED,
                                         response);
                    return true;
                    
                case MPIWORKER_STOPPED:
                    //stop this job
                    simulation.stop();
                    simulation.wait();
                    
                    if (simulation.nCompleted() == 0)
                    {
                        MPIWorker::sendReply(mpicomm, mpimaster, MPIWORKER_ABORTED,
                                             response);
                        return true;
                    }
                    
                    break;
                    
                case MPIWORKER_PROGRESS:
                    //get the current progress
                    {
                        QDataStream ds( &response, QIODevice::WriteOnly );
                        ds << simulation.progress();
                    }

                    MPIWorker::sendReply(mpicomm, mpimaster, MPIWORKER_PROGRESS,
                                         response);

                    break;
                    
                case MPIWORKER_INTERIM:
                    //get the interim result
                    try
                    {
                        simulation.pause();
                        
                        QDataStream ds( &response, QIODevice::WriteOnly );
                        ds << tuple<System,Moves,qint32>( simulation.system(),
                                                          simulation.moves(),
                                                          simulation.nCompleted() );

                        simulation.resume();
                    }
                    catch(...)
                    {
                        simulation.resume();
                        throw;
                    }
                    
                    MPIWorker::sendReply(mpicomm, mpimaster, MPIWORKER_INTERIM,
                                         response);
                    
                    break;
            }
        }
        
        //now check to see if the simulation has finished
        simulation.wait(100);
        
        if (simulation.hasFinished())
        {
            if (simulation.isError())
                simulation.throwError();
        
            //return the result
            QByteArray response;

            {
                QDataStream ds( &response, QIODevice::WriteOnly );
                
                ds << tuple<System,Moves,qint32>( simulation.system(),
                                                  simulation.moves(),
                                                  simulation.nCompleted() );
            }
            
            MPIWorker::sendReply(mpicomm, mpimaster, MPIWORKER_FINISHED,
                                 response);
            
            return true;
        }
    
    } // end of event loop
    
    return true;
}

/** This function is used by the back-end node to act on the message that it
    has received */
bool MPIWorker::actOnMessage(MPI::Comm *mpicomm, int mpimaster,
                             const QByteArray &message)
{
    if (message.isEmpty())
        return true;

    QDataStream ds(message);
    
    QString message_type;
    ds >> message_type;
    
    if (message_type == QLatin1String("runSim_0"))
    {
        return MPIWorker::runSim_0(mpicomm, mpimaster, ds);
    }
    else
    {
        throw SireError::program_bug( QObject::tr(
            "Invalid message \"%1\" cannot be interpreted!")
                .arg(message_type), CODELOC );
    }
    
    return true;
}

/** This function is run by an MPI back-end node */
void MPIWorker::backendLoop(MPI::Comm *mpicomm, int mpimaster)
{
    BOOST_ASSERT( mpicomm != 0 );

    while (true)
    {
        try
        {
            //wait for an instruction
            int message_size = 0;
            mpicomm->Recv( &message_size, 1, MPI::INT, mpimaster, 0 );
        
            if (message_size <= 0)
            {
                //the backend has been told to shut down
                break;
            }
                
            //create enough space to received the message
            QByteArray message_data( message_size + 1, " " );
            
            mpicomm->Recv( message_data.data(), message_size, MPI::BYTE, mpimaster, 0 );
            
            //act on the message
            bool stay_in_mode = MPIWorker::actOnMessage(mpicomm, mpimaster, 
                                                        message_data);
                                                        
            if (not stay_in_mode)
                return;
        }
        catch( const SireError::exception &e )
        {
            MPIWorker::sendError(e);
        }
        catch( const std::exception &e )
        {
            MPIWorker::sendError( SireError::std_exception(e) );
        }
        catch(...)
        {
            MPIWorker::sendError( SireError::unknown_exception( QObject::tr(
                                    "An unknown error has occurred!"), CODELOC ) );
        }
    
    } // end of the event loop
}

/** This is run in the background thread of the MPIWorker - it is used
    to listen to communication from the back-end and to respond appropriately */
void MPIWorker::run()
{
    BOOST_ASSERT( nodedata != 0 );
 
    try
    {
        //lock the node - this blocks until only this worker has exclusive
        //access to this MPI node
        nodedata->lock();

        //signal the parent that we have started running
        start_mutex.lock();
        start_waiter.wakeAll();

        //send instructions to the node to tell it what to run
        int message_size = args_data.size();

        this->sendMessage( &message_size );
        this->sendMessage( args_data );
        
        bool keep_looping = true;
    
        do
        {
            //get the size of the incoming message
            int message_envelope[2];
        
            this->receiveMessage( message_envelope, 2 );
        
            mpicomm->Recv( message_envelope, 2, MPI::INT, mpirank, 0 );
        
            int message_type = message_envelope[0];
            int message_size = message_envelope[1];
        
            QByteArray message_data;
        
            if (message_size > 0)
            {
                //receive the message
                this->receiveMessage( &message_data, message_size );
            }
        
            switch( message_type )
            {
                case MPIWORKER_ERROR:
                    this->setError(message_data);
                    keep_looping = false;
                    break;
                
                case MPIWORKER_FINISHED:
                    this->setResult(message_data);
                    keep_looping = false;
                    break;
                
                case MPIWORKER_PROGRESS:
                    this->setProgress(message_data);
                    break;
                
                case MPIWORKER_INTERIM:
                    this->setInterim(message_data);
                    break;
                    
                case MPIWORKER_ABORTED:
                    this->setAborted();
                    keep_looping = false;
                    break;
            }
    
        } while (keep_looping);
        
        //we have finished using the node
        nodedata->unlock();
    }
    catch(...)
    {
        nodedata->unlock();
        throw;
    }
}
