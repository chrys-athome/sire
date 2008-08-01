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

#include <QTime>

#include "simcontroller.h"
#include "moves.h"

#include "SireSystem/system.h"

#include <QDebug>

using namespace SireMove;

detail::sim_not_in_progress::sim_not_in_progress()
{}

detail::sim_not_in_progress::~sim_not_in_progress()
{}

/** Constructor */
SimController::SimController()
              : sim_system(0), sim_moves(0), nmoves(0), 
                ncompleted(0), record_stats(false), sim_aborted(false)
{}

/** Destructor */
SimController::~SimController()
{
    //make sure that the simulation has finished
    this->wait();
}

/** Wait until the simulation has finished running */
void SimController::wait()
{
    while ( this->isRunning() )
    {
        run_mutex.lock();
        run_mutex.unlock();
    }
}

/** Wait until the simulation has finished or for 'time' milliseconds,
    whichever comes sooner. Returns whether or not the simulation
    has finished */
bool SimController::wait(int time)
{
    QTime t;
    t.start();

    while ( this->isRunning() )
    {
        bool got_lock = run_mutex.tryLock( time - t.elapsed() );
        
        if (got_lock)
            run_mutex.unlock();
            
        if (t.elapsed() > time)
            return this->isRunning();
    }
    
    return true;
}

/** This function is called by the Moves object to initialise this controller */
void SimController::initialise(const System &system, const Moves &moves,
                               int num_moves, bool record_statistics)
{
    //only allow a single simulation to start using this controller
    //at a time
    QMutexLocker lkr(&start_mutex);

    //wait until any previous simulation has finished
    this->wait();

    {
        QMutexLocker lkr(&data_mutex);
    
        //get pointers to the system and moves
        sim_system = &system;
        sim_moves = &moves;
    
        nmoves = num_moves;
        record_stats = record_statistics;
        sim_aborted = false;
    }

    //lock the run mutex as the simulation is now running!
    run_mutex.lock();
}
                
/** Return whether or not there is another step of the simulation to run.
    This also increments the number of steps that have already run */
bool SimController::nextMove()
{
    qDebug() << "SimController::nextMove()";

    if (this->hasFinished())
        return false;
        
    //the simulation is still running - unlock the run_mutex as the simulation
    //is between moves
    run_mutex.unlock();
    
    {
        QMutexLocker lkr(&data_mutex);
        
        if (ncompleted == nmoves - 1)
        {
            //we have just finished the simulation!
            ncompleted = nmoves;
            sim_system = 0;
            sim_moves = 0;
            ncompleted = nmoves;
            return false;
        }
        else
            ++ncompleted;
    }
    
    //lock the run_mutex as we are going to run the next step of the simulation
    run_mutex.lock();
    
    return true;
}
                
/** Pause the simulation. */
void SimController::pause()
{
    run_mutex.lock();
}

/** Resume the simulation */
void SimController::resume()
{
    run_mutex.unlock();
}

/** Abort the running simulation - this stops the simulation
    and tells it to return to the state it was in before it started */
void SimController::abort()
{
    QMutexLocker lkr(&data_mutex);
    
    if (sim_system != 0)
    {
        //only allow the simulation to finish its current move
        nmoves = ncompleted + 1;
        sim_aborted = true;
    }
}

/** Return whether or not the simulation has been aborted */
bool SimController::aborted()
{
    QMutexLocker lkr(&data_mutex);
    return sim_aborted;
}

/** Stop the simulation - this stops the simulation, but leaves it
    in the state it was in when it stopped */
void SimController::stop()
{
    QMutexLocker lkr(&data_mutex);
    
    if (sim_system != 0)
        //only allow the simulation to finish its current move
        nmoves = ncompleted + 1;
}

/** Return the number of steps being run as part of this simulation */
int SimController::nMoves()
{
    QMutexLocker lkr(&data_mutex);
    return nmoves;
}

/** Return the number of steps that have already been completed */
int SimController::nCompleted()
{
    QMutexLocker lkr(&data_mutex);
    return ncompleted;
}

/** Return the progress of this simulation (percentage of completed steps) */
double SimController::progress()
{
    QMutexLocker lkr(&data_mutex);

    if (nmoves == 0)
        return 0;
    else
        return (100.0*ncompleted) / nmoves;
}

/** Return whether or not simulation statistics are being recorded */
bool SimController::recordingStatistics()
{
    QMutexLocker lkr(&data_mutex);
    return record_stats;
}

/** Return whether or not the simulation is currently running */
bool SimController::isRunning()
{
    QMutexLocker lkr(&data_mutex);
    return sim_system != 0;
}

/** Return whether or not the simulation has finished */
bool SimController::hasFinished()
{
    QMutexLocker lkr(&data_mutex);
    return ncompleted == nmoves;
}

/** Return whether or not the simulation has started
    (this also returns true if the simulation has finished) */
bool SimController::hasStarted()
{
    QMutexLocker lkr(&data_mutex);
    return sim_system != 0 or ncompleted > 0;
}

/** Return a copy of the current state of the system */
System SimController::system()
{
    QMutexLocker run_lkr(&run_mutex);
    QMutexLocker data_lkr(&data_mutex);
    
    if (sim_system == 0)
    {
        //the simulation has finished or has not started!
        throw detail::sim_not_in_progress();
    }
    
    return *sim_system;
}

/** Return a copy of the current state of the moves */
Moves SimController::moves()
{
    QMutexLocker run_lkr(&run_mutex);
    QMutexLocker data_lkr(&data_mutex);
    
    if (sim_moves == 0)
    {
        //the simulation has finished or has not started!
        throw detail::sim_not_in_progress();
    }
    
    return *( sim_moves->read().clone() );
}
