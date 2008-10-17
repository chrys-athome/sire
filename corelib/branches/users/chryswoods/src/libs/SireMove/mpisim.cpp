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

#include "mpisim.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include "SireError/errors.h"

#include <QDebug>

using namespace SireMove;
using namespace SireStream;

/** Null constructor */
MPISim::MPISim() : nmoves(0), record_stats(false), data_is_local(true)
{}

/** Internal function used to set the worker - the data_mutex must
    be held while you call this function */
void MPISim::_pvt_setWorker(const MPISimWorker &worker)
{
    sim_worker = worker;
    nmoves = worker.nMoves();
    record_stats = worker.recordStatistics();
    data_is_local = true;
}

/** Construct to work on the passed worker */
MPISim::MPISim(const MPINode &node, const MPISimWorker &worker) 
       : SimHandle(), mpinode(node)
{
    if (node.isNull())
        throw SireError::invalid_arg( QObject::tr(
            "You cannot start an MPI simulation on the null MPI node!"),
                CODELOC );

    initial_state = SireStream::save(worker);
    
    QMutexLocker lkr(&data_mutex);

    this->_pvt_setWorker(worker);
}

/** Destructor */
MPISim::~MPISim()
{
    mpipromise.stop();
    mpipromise.wait();
}

/** Return the initial worker from before the simulation started */
MPISimWorker MPISim::initialWorker()
{
    return SireStream::loadType<MPISimWorker>(initial_state);
}

/** Set the error state from the passed exception. You must hold the
    data mutex while calling this function */
void MPISim::_pvt_setError(const SireError::exception &e)
{
    error_data = SireStream::save(e);
}

/** This internal function is used to transfer the job from 
    the MPI subsystem back to this node and the Simulation subsystem.
    You must hold the data_mutex while calling this function */
void MPISim::_pvt_makeDataLocal()
{
    if (data_is_local)
        return;
    
    try
    {
        this->_pvt_setWorker( mpipromise.result<MPISimWorker>() );
        mpipromise = MPIPromise();
    }
    catch(const SireError::exception &e)
    {
        this->_pvt_setError(e);
    }
    catch(...)
    {
        this->_pvt_setError( SireError::program_bug( QObject::tr(
            "It should not be possible to have an unknown error here!"), CODELOC ) );
    }
}

/** Return the latest version of the worker, or throws an exception if an
    error has occured */
MPISimWorker MPISim::worker()
{
    QMutexLocker lkr(&data_mutex);
    
    if (not data_is_local)
    {
        if (not mpipromise.isRunning())
            this->_pvt_makeDataLocal();
    }
    
    if (not error_data.isEmpty())
        SireError::exception::unpackAndThrow(error_data);
    
    if (data_is_local)
        return sim_worker;
        
    else
        return mpipromise.interimResult<MPISimWorker>();
}

/** Return the system - this returns the latest version of the system that is 
    available, or throws an exception if an error has occured */
System MPISim::system()
{
    return this->worker().system();
}

/** Return the moves - this returns the latest version of the system that is 
    available, or throws an exception if an error has occured */
MovesPtr MPISim::moves()
{
    return this->worker().moves();
}

/** Return the number of moves to perform in total */
int MPISim::nMoves()
{
    QMutexLocker lkr(&data_mutex);
    return nmoves;
}

/** Return the number of moves completed so far */
int MPISim::nCompleted()
{
    QMutexLocker lkr(&data_mutex);
    
    if (data_is_local)
        return sim_worker.nCompleted();
        
    else
    {
        double current_progress = mpipromise.progress();
        return int( current_progress * nmoves );
    }
}

/** Return the percentage progress of the simulation */
double MPISim::progress()
{
    QMutexLocker lkr(&data_mutex);
    
    if (data_is_local)
        return sim_worker.progress();
    
    else
        return mpipromise.progress();
}

/** Return whether or not we are recording statistics */
bool MPISim::recordStatistics()
{
    QMutexLocker lkr(&data_mutex);
    return record_stats;
}

/** Start the job on the remote node */
void MPISim::start()
{
    //only one job may be started at once
    QMutexLocker lkr(&start_mutex);

    //wait for any previous job to finish
    this->wait();

    QMutexLocker lkr2(&data_mutex);

    //ensure that the data from the last job is back on this node
    this->_pvt_makeDataLocal();

    //start the job on the remote node, and get a handle to
    //the running job
    mpipromise = mpinode.start(sim_worker);
    sim_worker = MPISimWorker();

    //the data for the simulation now resides on the remote host
    data_is_local = false;
}

/** Stop the job - this blocks until the job has stopped and the
    result is back on this node */
void MPISim::stop()
{
    mpipromise.stop();
    this->wait();
}

/** Abort this job - this blocks until the job has fully stopped */
void MPISim::abort()
{
    this->stop();
    
    QMutexLocker lkr(&data_mutex);
    this->_pvt_setWorker( SireStream::loadType<MPISimWorker>(initial_state) );
}

/** Return whether or not the job is running */
bool MPISim::isRunning()
{
    QMutexLocker lkr(&data_mutex);
    
    if (data_is_local)
        return false;
        
    else
    {
        if (not mpipromise.isRunning())
        {
            this->_pvt_makeDataLocal();
            return false;
        }
        else
            return true;
    }
}

/** Return whether or not the job has started */
bool MPISim::hasStarted()
{
    QMutexLocker lkr(&data_mutex);
    
    if (data_is_local)
        return sim_worker.nCompleted() > 0;
        
    else
        return true;
}

/** Return whether or not the job has finished */
bool MPISim::hasFinished()
{
    QMutexLocker lkr(&data_mutex);
    
    if (not data_is_local)
    {
        if (not mpipromise.isRunning())
            this->_pvt_makeDataLocal();
    }
    
    if (data_is_local)
        return sim_worker.hasFinished();
    else
        return false;
}

/** Return whether or not we are in an error state */
bool MPISim::isError()
{
    QMutexLocker lkr(&data_mutex);
    return not error_data.isEmpty();
}

/** Throw an exception if we are in an error state that represents
    that state */
void MPISim::throwError()
{
    QMutexLocker lkr(&data_mutex);
    
    if (not error_data.isEmpty())
        SireError::exception::unpackAndThrow(error_data);
}

/** Clear the error state */
void MPISim::clearError()
{
    QMutexLocker lkr(&data_mutex);
    error_data = QByteArray();
}

/** Wait until the job has finished. */
void MPISim::wait()
{
    mpipromise.wait();
}

/** Wait until the job has finished, or 'time' milliseconds have passed. 
    Returns whether or not the job has stopped */
bool MPISim::wait(int time)
{
    return mpipromise.wait(time);
}
