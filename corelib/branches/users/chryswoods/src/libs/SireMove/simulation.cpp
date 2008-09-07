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

#include <QMutex>
#include <QWaitCondition>

#include "simulation.h"
#include "threadsim.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include "SireError/errors.h"

using namespace SireMove;
using namespace SireSystem;
using namespace SireStream;

/////////
///////// Implementation of SimHandle
/////////

/** Constructor */
SimHandle::SimHandle() : boost::noncopyable()
{}

/** Destructor */
SimHandle::~SimHandle()
{}

/////////
///////// Implementation of LocalSim
/////////

/** Constructor */
LocalSim::LocalSim() : SimHandle(),
                       nmoves(0), ncompleted(0), 
                       record_stats(false)
{}

/** Construct to run the specified simulation */
LocalSim::LocalSim(const System &system, const MovesBase &moves,
                   int nmvs, int ncomp, bool record_statistics)
         : SimHandle(),
           sim_system(system), sim_moves(moves), nmoves(nmvs),
           ncompleted(ncomp), record_stats(record_statistics)
{
    if (nmoves < 0)
        nmoves = 0;

    if (ncompleted > nmoves)
        ncompleted = nmoves;
}

/** Destructor */
LocalSim::~LocalSim()
{
    this->abort();
    this->wait();
}

/** Return a copy of the system being simulated */
System LocalSim::system()
{
    try
    {
        return controller.system();
    }
    catch(detail::sim_not_in_progress)
    {
        QMutexLocker lkr(&data_mutex);
        
        if (error_ptr.get() != 0)
        {
            boost::shared_ptr<SireError::exception> ptr = error_ptr;
            error_ptr.reset();
            
            ptr->throwSelf();
        }
        
        return sim_system;
    }
}

/** Return a copy of the moves being applied to the system */
Moves LocalSim::moves()
{
    try
    {
        return controller.moves();
    }
    catch(detail::sim_not_in_progress)
    {
        QMutexLocker lkr(&data_mutex);

        if (error_ptr.get() != 0)
        {
            boost::shared_ptr<SireError::exception> ptr = error_ptr;
            error_ptr.reset();
            
            ptr->throwSelf();
        }

        return sim_moves;
    }
}

/** Return the total number of moves to be applied to the system
    during this simulation */
int LocalSim::nMoves()
{
    QMutexLocker lkr(&data_mutex);
    return nmoves;
}

/** Return the number of moves that have been completed */
int LocalSim::nCompleted()
{
    int ncurrent_completed = controller.nCompleted();
    
    QMutexLocker lkr(&data_mutex);

    return qMin(ncurrent_completed + ncompleted, nmoves);
}

/** Return the current progress of the simulation (percentage of 
    completed moves) */
double LocalSim::progress()
{
    int ncurrent_completed = controller.nCompleted();
    
    QMutexLocker lkr(&data_mutex);
    if (nmoves == 0)
        return 0;
    else
    {
        ncurrent_completed = qMin(ncurrent_completed + ncompleted, nmoves);
        return (100.0 * ncurrent_completed) / nmoves;
    }
}

/** Return whether or not statistics are being recorded during
    this simulation */
bool LocalSim::recordingStatistics()
{
    QMutexLocker lkr(&data_mutex);
    return record_stats;
}

/** Set an error condition */
void LocalSim::setError(const SireError::exception &e)
{
    QMutexLocker lkr(&data_mutex);
    error_ptr.reset( e.clone() );
}

/** Return whether or not we are in an error condition */
bool LocalSim::isError()
{
    QMutexLocker lkr(&data_mutex);
    return error_ptr.get() != 0;
}

/** Throw the exception that represents the error condition */
void LocalSim::throwError()
{
    QMutexLocker lkr(&data_mutex);
    
    if (error_ptr.get() != 0)
    {
        boost::shared_ptr<SireError::exception> error = error_ptr;
        error_ptr.reset();
    
        error->throwSelf();
    }
}

/** Clear any error condition */
void LocalSim::clearError()
{
    QMutexLocker lkr(&data_mutex);
    error_ptr.reset();
}

/** Start running the simulation */
void LocalSim::start()
{
    try
    {
        QMutexLocker lkr(&run_mutex);

        int nremaining_moves = 0;
        MovesBase *moves = 0;

        //critical section
        {
            QMutexLocker lkr(&data_mutex);
    
            if (ncompleted >= nmoves)
            {
                ncompleted = nmoves;
                data_mutex.unlock();
                return;
            }
        
            nremaining_moves = nmoves - ncompleted;
            moves = &(sim_moves.edit());
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
    }
    catch( const SireError::exception &e )
    {
        this->setError(e);
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

/** Pause the simulation */
void LocalSim::pause()
{
    controller.pause();
}

/** Resume the simulation */
void LocalSim::resume()
{
    controller.resume();
}

/** Stop the simulation */
void LocalSim::stop()
{
    controller.stop();
}

/** Abort the simulation */
void LocalSim::abort()
{
    controller.abort();
}

/** Is the simulation running? */
bool LocalSim::isRunning()
{
    return controller.isRunning();
}

/** Has the simulation started? */
bool LocalSim::hasStarted()
{
    //critical section
    {
        QMutexLocker lkr(&data_mutex);
        if (ncompleted > 0)
            return true;
    }
    
    return controller.hasStarted();
}

/** Has the simulation finished? */
bool LocalSim::hasFinished()
{
    QMutexLocker lkr(&data_mutex);
    return ncompleted == nmoves and ncompleted > 0;
}

/** Wait until the simulation has finished, or 'time' milliseconds
    has passed */
bool LocalSim::wait(int time)
{
    return controller.wait(time);
}

/** Wait until the simulation has finished */
void LocalSim::wait()
{
    controller.wait();
}

/////////
///////// Implementation of Simulation
/////////

static const RegisterMetaType<Simulation> r_sim;

/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, const Simulation &sim)
{
    writeHeader(ds, r_sim, 1);
    
    SharedDataStream sds(ds);
    
    SimHandle *d = ( const_cast<Simulation*>(&sim) )->d.get();
    
    d->pause();
    
    sds << d->system() << d->moves()
        << quint64( d->nMoves() ) << quint64( d->nCompleted() )
        << d->recordingStatistics();
        
    d->resume();
        
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, Simulation &sim)
{
    VersionID v = readHeader(ds, r_sim);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        System system;
        Moves moves;
        quint64 nmoves, ncompleted;
        bool record_stats;
        
        sds >> system >> moves >> nmoves >> ncompleted >> record_stats;
        
        sim = Simulation( system, moves, nmoves, ncompleted, record_stats );
    }
    else
        throw version_error( v, "1", r_sim, CODELOC );
        
    return ds;
}

/** Null constructor */
Simulation::Simulation() : d( new LocalSim() )
{}

/** Construct a simulation of the system 'system' that involves nmoves
    of the moves in 'moves', also specifying whether or not statistics
    should be collected during this simulation */
Simulation::Simulation(const System &system, const MovesBase &moves,
                       int nmoves, bool record_stats)
           : d( new LocalSim(system, moves, nmoves, 0, record_stats) )
{}

/** Private constructor used to restore a simulation that has been read
    from a datastream */
Simulation::Simulation(const System &system, const MovesBase &moves,
                       int nmoves, int ncompleted, bool record_stats)
           : d( new LocalSim(system, moves, nmoves, ncompleted, record_stats) )
{}

/** Copy constructor */
Simulation::Simulation(const Simulation &other)
           : d( other.d )
{}

/** Destructor */
Simulation::~Simulation()
{}

/** Run a simulation consisting of nmoves moves on the system 'system' using
    the 'moves' object to actually perform the moves */
Simulation Simulation::run(const System &system, const MovesBase &moves,
                           int nmoves, bool record_stats)
{
    Simulation sim(system, moves, nmoves, record_stats);
    sim.start();
    
    return sim;
}

/** Run a simulation consisting of nmoves moves on the system 'system' using
    the 'moves' object to actually perform the moves, in a background thread */
Simulation Simulation::runBG(const System &system, const MovesBase &moves,
                             int nmoves, bool record_stats)
{
    Simulation sim;
    sim.d.reset( new ThreadSim(system, moves, nmoves, record_stats) );
    
    sim.start();
    
    return sim;
}

/** Copy assignment operator */
Simulation& Simulation::operator=(const Simulation &other)
{
    d = other.d;
    return *this;
}

/** Comparison operator */
bool Simulation::operator==(const Simulation &other) const
{
    return d.get() == other.d.get();
}

/** Comparison operator */
bool Simulation::operator!=(const Simulation &other) const
{
    return d.get() != other.d.get();
}

/** Return the current version of the system being simulated
      - this may be the system at a point *during* a live simulation! */
System Simulation::system()
{
    return d->system();
}

/** Return the current status of the moves used in this simulation.
    This may be the moves as they are at a point *during* a live simulation! */ 
Moves Simulation::moves()
{
    return d->moves();
}

/** Return the number of moves to be completed during this simulation */
int Simulation::nMoves()
{
    return d->nMoves();
}

/** Return the number of moves that have been completed */
int Simulation::nCompleted()
{
    return d->nCompleted();
}

/** Return the progress of the simulation (percentage completed) */
double Simulation::progress()
{
    return d->progress();
}

/** Return whether or not statistics will be recorded during
    this simulation */
bool Simulation::recordingStatistics()
{
    return d->recordingStatistics();
}

/** Start the simulation */
void Simulation::start()
{
    d->start();
}

/** Temporarily pause the simulation */
void Simulation::pause()
{
    d->pause();
}

/** Resume a paused simulation - it is an error to try
    to resume a simulation that has not been paused */
void Simulation::resume()
{
    d->resume();
}

/** Stop the current simulation. This will stop the simulation
    at the next sane point. This will not block, so the simulation
    may take some time to stop. Use Simulation::wait() to block
    until the simulation has finished */
void Simulation::stop()
{
    d->stop();
}

/** Abort the current simulation. This will abort the simulation
    and return the system to where it was before the simulation
    started. This will not block, so the simulation
    may take some time to stop. Use Simulation::wait() to block
    until the simulation has finished */
void Simulation::abort()
{
    d->abort();
}

/** Return whether or not the current simulation is running */
bool Simulation::isRunning()
{
    return d->isRunning();
}

/** Return whether or not the simulation has started */
bool Simulation::hasStarted()
{
    return d->hasStarted();
}

/** Return whether or not the current simulation has finished */
bool Simulation::hasFinished()
{
    return d->hasFinished();
}

/** Return whether or not the simulation is in an error state */
bool Simulation::isError()
{
    return d->isError();
}

/** Throw the exception associated with the error state (this does
    nothing if isError() is false) */
void Simulation::throwError()
{
    d->throwError();
}

/** Clear any error state */
void Simulation::clearError()
{
    d->clearError();
}

/** Wait until the simulation has finished, stopped or been aborted
    (or 'time' milliseconds, whichever comes first). This returns whether 
    or not the simulation has finished. */
bool Simulation::wait(int time)
{
    return d->wait(time);
}

/** Wait indefinitely until the simulation has finished, stopped or been
    aborted */
void Simulation::wait()
{
    d->wait();
}
