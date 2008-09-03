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

MPINodeData::MPINodeData() : rank(0), is_busy(false), is_master(false)
{}

MPINodeData::MPINodeData(const MPINodes &communicator, 
                         int processor_rank, bool ismaster)
            : parent(communicator.d), rank(processor_rank),
              uid( QUuid::createUuid() ), 
              is_busy(false),
              is_master(ismaster)
{
    BOOST_ASSERT( rank >= 0 );
}

MPINodeData::MPINodeData(const MPINodeData &other)
            : parent(other.parent), rank(other.rank), uid(other.uid),
              is_busy(false), is_master(other.is_master)
{}

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
    return d->rank;
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
bool MPIWorker::isFinished()
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

/** Return the interim result of the calculation - this blocks
    until an interim (or indeed final) result is available */
QByteArray MPIWorker::interimResult()
{
    QMutexLocker lkr(&data_mutex);

    if ( current_status & MPIWORKER_HAS_FINISHED )
    {
        return this->processResult();
    }
    
    //send a request to the back-end for an interim result
    BOOST_ASSERT( nodedata != 0 );
    
    MPI::Comm *mpicomm = nodedata->mpicomm;
    int mpirank = nodedata->mpirank;
    
    int message = MPIWORKER_INTERIM;
    
    mpicomm.Isend( &message, 1, MPI::INT, mpirank, 0 );
    
    //wait for a result
    interim_waiter.wait( &data_mutex );
    
    return this->processResult();
}

/** Return the result of the work - this blocks until the work has finished,
    or there is an error, or the work is aborted */
QByteArray MPIWorker::result()
{
    QMutexLocker lkr(&data_mutex);
    
    if (not current_status & MPIWORKER_HAS_FINISHED)
    {
        //wait for a result
        result_waiter.wait( &data_mutex );
    }
    
    return this->processResult();
}

/** Abort the work if it is still running. This blocks until the 
    process has aborted */
void MPIWorker::abort()
{
    QMutexLocker lkr(&data_mutex);
    
    if (not current_status & MPIWORKER_IS_FINISHED)
    {
        int message = MPIWORKER_ABORTED;

        BOOST_ASSERT( nodedata != 0 );
        MPI::Comm *mpicomm = nodedata->mpicomm;
        int mpirank = nodedata->mpirank;
        
        mpicomm->Isend(&message, 1, MPI::INT, mpirank, 0);
        
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
    
    BOOST_ASSERT( nodedata != 0 );
    
    MPI::Comm *mpicomm = nodedata->mpicomm;
    int mpirank = nodedata->mpirank;
    
    mpicomm->Isend( &message, 1, MPI::INT, mpirank, 0 );
    
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

/** This runs a simulation on the back-end node */
void MPIWorker::runSim_0(MPI::Comm *mpicomm, int mpimaster,
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
            
            QByteArray response;
            
            switch (message)
            {
                case MPIWORKER_ABORTED:
                    //abort this job
                    simulation.abort();
                    simulation.wait();
                    MPIWorker::sendReply(mpicomm, mpimaster, MPIWORKER_ABORTED,
                                         response);
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
                    {
                        simulation.pause();
                        
                        QDataStream ds( &response, QIODevice::WriteOnly );
                        ds << tuple<System,Moves,qint32>( simulation.system(),
                                                          simulation.moves(),
                                                          simulation.nCompleted() );

                        simulation.resume();
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
            
            return;
        }
    
    } // end of event loop
}

/** This function is used by the back-end node to act on the message that it
    has received */
void MPIWorker::actOnMessage(MPI::Comm *mpicomm, int mpimaster,
                             const QByteArray &message)
{
    if (message.isEmpty())
        return;

    QDataStream ds(message);
    
    QString message_type;
    ds >> message_type;
    
    if (message_type == QLatin1String("runSim_0"))
    {
        MPIWorker::runSim_0(mpicomm, mpimaster, ds);
    }
    else
    {
        throw SireError::program_bug( QObject::tr(
            "Invalid message \"%1\" cannot be interpreted!")
                .arg(message_type), CODELOC );
    }
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
            MPIWorker::actOnMessage(mpicomm, mpimaster, message_data);
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

        MPI::Comm *mpicomm = nodedata->mpicomm;
        int mpirank = nodedata->mpirank;

        //send instructions to the node to tell it what to run
        int message_size = args_data.size();
        mpicomm->Send( &message_size, 1, MPI::INT, mpirank, 0 );
        mpicomm->Send( args_data.constData(), message_size, MPI::BYTE, mpirank, 0 );
        
        bool keep_looping = true;
    
        do
        {
            //get the size of the incoming message
            int message_envelope[2];
        
            mpicomm->Recv( message_envelope, 2, MPI::INT, mpirank, 0 );
        
            int message_type = message_envelope[0];
            int message_size = message_envelope[1];
        
            QByteArray message_data;
        
            if (message_size > 0)
            {
                //allocate enough space
                message_data = QByteArray( message_size + 1, " " );
        
                //receive the message
                mpicomm->Recv( message_data.data(), message_size, MPI::BYTE, 0 );
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
