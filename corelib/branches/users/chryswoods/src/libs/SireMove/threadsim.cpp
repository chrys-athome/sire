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

#include "threadsim.h"
#include "simcontroller.h"

#include "SireError/errors.h"

#include <QDebug>

using namespace SireMove;
using namespace SireSystem;

/** Constructor */
ThreadSim::ThreadSim() : LocalSim(), QThread(), sim_starting(false)
{}

/** Construct to run the specified simulation */
ThreadSim::ThreadSim(const System &system, const MovesBase &moves,
                     int nmoves, bool record_stats)
          : LocalSim(system, moves, nmoves, 0, record_stats), QThread(),
            sim_starting(false)
{}
          
/** Destructor - wait until we have finished */
ThreadSim::~ThreadSim()
{
    LocalSim::abort();
    LocalSim::wait();
}

/** Start the simulation in the background thread */
void ThreadSim::start()
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

/** This is what is run by the background thread */
void ThreadSim::run()
{
    try
    {
        data_mutex.lock();
        sim_starting = true;
        data_mutex.unlock();
    
        starter.wakeAll();

        QMutexLocker lkr(&run_mutex);

        Moves old_moves = sim_moves;

        int nremaining_moves = 0;
        MovesBase *moves = 0;
    
        //critical section
        {
            QMutexLocker lkr(&data_mutex);
    
            if (ncompleted >= nmoves)
            {
                ncompleted = nmoves;
                return;
            }
        
            nremaining_moves = nmoves - ncompleted;
            moves = &(sim_moves.edit());
            sim_starting = false;
        }

        System new_system = moves->move(sim_system, nremaining_moves, record_stats,
                                        controller);

        if (not controller.aborted())
        {
            int njust_completed = controller.nCompleted();

            //critical section
            {
                QMutexLocker lkr(&data_mutex);
                ncompleted += njust_completed;
                sim_system = new_system;
            }
        }
        else
        {
            QMutexLocker lkr(&data_mutex);
            sim_moves = old_moves;
        }
    }
    catch( const SireError::exception &e )
    {
        this->setError( e );
    }
    catch( const std::exception &e )
    {
        this->setError( SireError::std_exception(e) );
    }
    catch(...)
    {
        this->setError( SireError::unknown_exception( QObject::tr(
            "An unknown error occurred!"), CODELOC ) );
    }
}

/** Return whether or not the simulation is starting up */
bool ThreadSim::isStarting()
{
    QMutexLocker lkr(&data_mutex);
    
    return sim_starting;
}

/** Is the simulation running? */
bool ThreadSim::isRunning()
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

/** Has the simulation started? */
bool ThreadSim::hasStarted()
{
    //critical section
    {
        QMutexLocker lkr(&data_mutex);

        if (sim_starting or ncompleted > 0)
            return true;
    }
    
    return controller.hasStarted();
}

/** Wait until the simulation has finished, or 'time' milliseconds
    has passed */
bool ThreadSim::wait(int time)
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

/** Wait until the simulation has finished */
void ThreadSim::wait()
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
