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

using namespace SireMove;

    MPISim();

    MPISim(const MPINode &node, const System &system, const MovesBase &moves,
           int nmoves, bool record_stats);
    
    ~MPISim();

/** Actually run this simulation - this occurs in a background thread
    so that it can block waiting for status updates from the 
    MPINode doing the work */
void MPISim::run()
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
        
        nremaining_moves = nmoves - ncompleted;
        sim_starting = false;
    }
    
    //tell the node to run the simulation
    sim_result = mpinode.runSim(sim_system, sim_moves, nremaining_moves, record_stats);
    
    //wait until the worker has finished
    sim_result.wait();
    
    if (sim_result.isError())
    {
        sim_result.throwError();
    }
    else if (not sim_result.isEmpty())
    {
        int njust_completed = sim_result.value().get<2>();
        
        if (njust_completed > 0)
        {
            QMutexLocker lkr(&data_mutex);
            ncompleted += njust_completed;
            sim_system = sim_result.value().get<0>();
            sim_moves = sim_result.value().get<1>();
        }
    }
    
    //clear the result as it is no longer needed
    sim_result.clear();
}
    
/** Return the latest copy of the system */
System MPISim::system()
{
    QMutexLocker lkr(&data_mutex);
    
    if (not sim_result.isNull())
    {
        sim_result.getCurrentValue();
        return sim_result.value().get<0>();
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
        sim_result.getCurrentValue();
        return sim_result.value().get<1>();
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
    
    sim_result.getCurrentValue();
    return ncompleted + sim_result.value().get<2>();
}

/** Get the progress of the simulation */
double MPISim::progress()
{
    QMutexLocker lkr(&data_mutex);
    
    if (not sim_result.isNull())
        return sim_result.progress();
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
    mpinode.pause();
}

/** Resume the running simulation */
void MPISim::resume()
{
    mpinode.resume();
}

/** Abort the currently running simulation - this resets
    the system back to where it was before the simulation
    started */
void MPISim::abort()
{
    mpinode.abort();
}

/** Stop the running simulation - this stops the simulation
    but keeps the system at the point that it reached */
void MPISim::stop()
{
    mpinode.stop();
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
            
            //has it started yet?
            if (controller.hasStarted())
                break;
                
            //was the simulation aborted?
            else if (controller.aborted())
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
            
            //have we started yet?
            else if (controller.hasStarted())
                break;
                
            //was the simulation aborted?
            else if (controller.aborted())
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
