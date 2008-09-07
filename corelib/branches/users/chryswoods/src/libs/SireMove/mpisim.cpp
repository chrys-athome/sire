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

using namespace SireMove;
using namespace SireStream;

static QDataStream& operator>>(QDataStream &ds,
                               tuple<System,Moves,qint32> &t)
{
    SharedDataStream sds(ds);
    
    System system;
    Moves moves;
    qint32 nmoves = 0;
    
    sds >> system >> moves >> nmoves;
    
    t = tuple<System,Moves,qint32>(system, moves, nmoves);
    
    return ds;
}

/** Null constructor */
MPISim::MPISim() : SimHandle(), QThread(),
                   nmoves(0), ncompleted(0), record_stats(false),
                   sim_starting(false), has_started(false)
{}

/** Construct the MPI simulation to perform the moves in 'moves' on the 
    system 'system' - 'nmoves' will be performed, and statistics recorded
    if 'record_stats' is true */
MPISim::MPISim(const MPINode &node, const System &system, const MovesBase &moves,
               int num_moves, bool record_statistics)
       : SimHandle(), QThread(),
         sim_system(system), sim_moves(moves), 
         nmoves(num_moves), ncompleted(0), record_stats(record_statistics),
         sim_starting(false), has_started(false)
{}       

/** Destructor */
MPISim::~MPISim()
{
    if (QThread::isRunning())
    {
        this->abort();
        this->wait();
    }
}

/** Set an error condition */
void MPISim::setError(const SireError::exception &e)
{
    QMutexLocker lkr(&data_mutex);
    error_ptr.reset( e.clone() );
}

/** Return whether or not we are in an error condition */
bool MPISim::isError()
{
    QMutexLocker lkr(&data_mutex);
    return error_ptr.get() != 0;
}

/** Throw the exception that represents the error condition */
void MPISim::throwError()
{
    QMutexLocker lkr(&data_mutex);
    
    if (error_ptr.get() != 0)
    {
        boost::shared_ptr<SireError::exception> error = error_ptr;
        error_ptr.reset();
    
        error->throwSelf();
    }
}

/** Clear the error condition */
void MPISim::clearError()
{
    QMutexLocker lkr(&data_mutex);
    error_ptr.reset();
}

/** Actually run this simulation - this occurs in a background thread
    so that it can block waiting for status updates from the 
    MPINode doing the work */
void MPISim::run()
{
    try
    {
        data_mutex.lock();
        sim_starting = true;
        data_mutex.unlock();
    
        starter.wakeAll();

        //yes - the simulation is now running!
        QMutexLocker lkr(&run_mutex);

        int nremaining_moves = 0;

        //critical section
        {
            QMutexLocker lkr(&data_mutex);
    
            if (ncompleted >= nmoves)
            {
                ncompleted = nmoves;
                return;
            }
            
            sim_starting = false;
        
            nremaining_moves = nmoves - ncompleted;
            
            if (nremaining_moves <= 0)
            {
                //there is nothing left to do
                return;
            }

            //tell the node to run the simulation
            sim_result = mpinode.runSim(sim_system, sim_moves, 
                                        nremaining_moves, record_stats);
        }
    
        //wait until the worker has finished
        sim_result.wait();
    
        if (not sim_result.isAborted())
        {
            int njust_completed = sim_result.result().get<2>();
        
            if (njust_completed > 0)
            {
                QMutexLocker lkr(&data_mutex);
                ncompleted += njust_completed;
                sim_system = sim_result.result().get<0>();
                sim_moves = sim_result.result().get<1>();
            }
        }
    
        //clear the result as it is no longer needed
        data_mutex.lock();
        sim_result = MPIPromise< tuple<System,Moves,qint32> >();
        data_mutex.unlock();
    }
    catch(const SireError::exception &e)
    {
        this->setError(e);
    }
    catch(const std::exception &e)
    {
        this->setError( SireError::std_exception(e) );
    }
    catch(...)
    {
        this->setError( SireError::unknown_exception( QObject::tr(
            "An unknown error occurred!"), CODELOC ) );
    }
}
    
/** Return the latest copy of the system */
System MPISim::system()
{
    QMutexLocker lkr(&data_mutex);
    
    if (not sim_result.isNull())
    {
        return sim_result.interimResult().get<0>();
    }
    else
        return sim_system;
}

/** Return the latest copy of the moves */
Moves MPISim::moves()
{
    QMutexLocker lkr(&data_mutex);
    
    if (not sim_result.isNull())
    {
        return sim_result.interimResult().get<1>();
    }
    else
        return sim_moves;
}

/** Return the number of moves to be run in this simulation */
int MPISim::nMoves()
{
    QMutexLocker lkr(&data_mutex);
    return nmoves;
}

/** Return the total number of steps completed so far */
int MPISim::nCompleted()
{
    QMutexLocker lkr(&data_mutex);
    
    int njust_completed = 0;
    
    if (not sim_result.isNull())
    {
        njust_completed = sim_result.interimResult().get<2>();
    }
        
    return ncompleted + njust_completed;
}

/** Get the progress of the simulation */
double MPISim::progress()
{
    QMutexLocker lkr(&data_mutex);

    if (nmoves == 0)
        return 0;
    
    int njust_completed = 0;
    
    if (not sim_result.isNull())
    {
        njust_completed = sim_result.interimResult().get<2>();
    }
    
    return (100.0 * (ncompleted + njust_completed)) / nmoves;
}

/** Are we recording statistics during this simulation run? */
bool MPISim::recordingStatistics()
{
    QMutexLocker lkr(&data_mutex);
    return record_stats;
}

/** Start the MPI simulation */
void MPISim::start()
{
    QMutexLocker run_lkr(&run_mutex);
    QMutexLocker data_lkr(&data_mutex);
    
    if (nmoves == 0)
        //there is nothing worth starting
        return;
    
    if (sim_starting)
        //cannot start the simulation twice!
        return;

    sim_starting = false;

    QThread::start();
    
    starter.wait(&data_mutex);
    
    data_lkr.unlock();
    run_lkr.unlock();
}

/** Pause the running simulation */
void MPISim::pause()
{
    //does nothing as we can't pause and resume an MPI job
}

/** Resume the running simulation */
void MPISim::resume()
{
    //does nothing as we can't pause and resume an MPI job
}

/** Abort the currently running simulation - this resets
    the system back to where it was before the simulation
    started */
void MPISim::abort()
{
    if (not sim_result.isNull())
        sim_result.abort();
}

/** Stop the running simulation - this stops the simulation
    but keeps the system at the point that it reached */
void MPISim::stop()
{
    if (not sim_result.isNull())
        sim_result.stop();
}

/** Return whether or not the simulation is in the process of starting */
bool MPISim::isStarting()
{
    QMutexLocker lkr(&data_mutex);
    return sim_starting;
}

/** Return whether or not the simulation is running */
bool MPISim::isRunning()
{
    if (this->isStarting())
        return true;
    else
    {
        if (run_mutex.tryLock())
        {
            run_mutex.unlock();
            return false;
        }
        else
            return true;
    }
}

/** Return whether or not the simulation has started */
bool MPISim::hasStarted()
{
    //critical section
    {
        QMutexLocker lkr(&data_mutex);
        if (sim_starting or ncompleted > 0)
            return true;
    }
    
    return this->isRunning();
}

/** Return whether or not the simulation has finished */
bool MPISim::hasFinished()
{
    QMutexLocker lkr(&data_mutex);
    
    return ncompleted == nmoves and ncompleted > 0;
}

/** Wait until the simulation has finished, been stopped
    or been aborted */
void MPISim::wait()
{
    if (this->isStarting())
    {
        //wait until the simulation has started
        while (true)
        {
            //wait for 10 ms
            QThread::wait(10);
            
            if (this->isRunning() or this->hasFinished())
                break;
        }
    }

    run_mutex.lock();
    run_mutex.unlock();
}

/** Wait until the simulation has finished, or 'time' milliseconds
    has passed */
bool MPISim::wait(int time)
{
    if (this->isStarting())
    {
        //wait until the simulation has finished starting!
        while (true)
        {
            //wait for 10 ms
            QThread::wait(10);
            
            time -= 10;
            
            //have we ran out of time?
            if (time <= 0)
                return false;
            
            if (this->hasStarted() or this->hasFinished())
                break;
        }
    }

    if (run_mutex.tryLock(time))
    {
        run_mutex.unlock();
        return true;
    }
    else
        return false;
}
