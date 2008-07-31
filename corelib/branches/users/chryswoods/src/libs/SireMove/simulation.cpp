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

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMove;
using namespace SireSystem;
using namespace SireStream;

/////////
///////// Implementation of MoveMutex
/////////

/** Constructor */
MoveMutex::MoveMutex() : mutex( QMutex::Recursive ), nmoves(0), ncompleted(0)
{}

/** Destructor */
MoveMutex::~MoveMutex()
{
    //make sure we have finished
    mutex.lock();
    mutex.unlock();
}

/** Initialise with 'nmoves' moves of simulation */
void MoveMutex::initialise(int nmvs, const System &system,
                                     const Moves &moves)
{
    QMutexLocker lkr(&mutex);

    if (nmvs <= 0)
    {
        nmvs = 0;
        ncompleted = 0;
        sim_system = 0;
        sim_moves = 0;
    }
    else
    {
        nmoves = nmvs;
        ncompleted = 0;
        sim_system = &system;
        sim_moves = &moves;
    }
}

/** Increment the move counter and return whether or not there
    is another move to perform */
bool MoveMutex::nextMove()
{
    if (ncompleted == nmoves)
        return false;

    //have we completed any moves yet?
    if (ncompleted > 0)
        //release the mutex - this gives a chance to stop or
        //abort this simulation
        mutex.unlock();

    mutex.lock();

    if (ncompleted < nmoves-1)
    {
        //there are more moves to run
        ++ncompleted;
        return true;
    }
    else
    {
        ncompleted = nmoves;
        sim_system = 0;
        sim_moves = 0;
        
        mutex.unlock();
    
        return false;
    }
}

/** Return whether the simulation is in progress */
bool MoveMutex::isRunning()
{
    QMutexLocker lkr(&mutex);
    return sim_system != 0;
}

/** Return the number of moves to run */
int MoveMutex::nMoves()
{
    QMutexLocker lkr(&mutex);
    return nmoves;
}

/** Return the number of completed moves */
int MoveMutex::nCompleted()
{
    QMutexLocker lkr(&mutex);
    return ncompleted;
}

/** Return a copy of the system being simulated, in its current form */
System MoveMutex::system()
{
    QMutexLocker lkr(&mutex);
    
    if (sim_system)
        return *sim_system;
    else
        return System();
}

/** Return a copy of the moves object that is used to perform the moves */
Moves MoveMutex::moves()
{
    QMutexLocker lkr(&mutex);
    
    if (sim_moves)
        return *sim_moves;
    else
        return Moves();
}

/** Pause the simulation */
void MoveMutex::pause()
{
    mutex.lock();
}

/** Resume the simulation */
void MoveMutex::resume()
{
    mutex.unlock();
}

/** Abort the current simulation */
void MoveMutex::abort()
{
    QMutexLocker lkr(&mutex);
    nmoves = ncompleted;
}

/////////
///////// Implementation of SimHandle
/////////

/** Constructor */
SimHandle::SimHandle()
{}

/** Destructor */
SimHandle::~SimHandle()
{}

/////////
///////// Implementation of LocalSim
/////////

/** This is a simple class that provides a simulation that
    runs within the current thread */
class LocalSim : public SimHandle
{
public:
    LocalSim();

    LocalSim(const System &system, const MovesBase &moves,
             int nmoves, int ncomp, bool record_stats);
    
    ~LocalSim();
    
    System system();
    Moves moves();

    int nMoves();
    int nCompletedMoves();
    
    bool recordingStatistics();
    
    void start();
    
    void pause();
    void resume();
    
    void stop();
    
    bool isRunning();
    bool hasFinished();
    
    bool wait(unsigned long time);

private:
    MoveMutex sim_mutex;

    System sim_system;
    Moves sim_moves;
    
    int nmoves;
    int ncompleted;
    
    bool record_stats;
};

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
{}

/** Return a copy of the system being simulated */
System LocalSim::system()
{
    MoveMutexLocker lkr(&sim_mutex);
    
    if (sim_mutex.isRunning())
        return sim_mutex.system();
    else
        return sim_system;
}

/** Return a copy of the moves being applied to the system */
Moves LocalSim::moves()
{
    MoveMutexLocker lkr(&sim_mutex);
    
    if (sim_mutex.isRunning())
        return sim_mutex.moves();
    else
        return sim_moves;
}

/** Return the total number of moves to be applied to the system
    during this simulation */
int LocalSim::nMoves()
{
    return nmoves;
}

/** Return the number of moves that have been completed */
int LocalSim::nCompletedMoves()
{
    return move_mutex.nCompletedMoves();
}

/** Return whether or not statistics are being recorded during
    this simulation */
bool LocalSim::recordingStatistics()
{
    return record_stats;
}

/** Start running the simulation */
void LocalSim::start()
{
    move_mutex.wait();

    if (ncompleted < nmoves)
    {
        sim_system = sim_moves.edit()
                              .move(sim_system, nmoves - ncompleted, record_stats,
                                    move_mutex);
    }
    
    ncompleted = nmoves;
}

/** Pause the simulation */
void LocalSim::pause()
{
    move_mutex.pause();
}

/** Resume the simulation */
void LocalSim::resume()
{
    move_mutex.resume();
}

/** Stop the simulation */
void LocalSim::stop()
{
    move_mutex.abort();
}

/** Is the simulation running? */
bool LocalSim::isRunning()
{
    return move_mutex.isRunning();
}

/** Has the simulation finished? */
bool LocalSim::hasFinished()
{
    return move_mutex.hasFinished();
}

/** Wait until the simulation has finished */
bool LocalSim::wait(unsigned long time)
{
    move_mutex.wait();
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
        << quint64( d->nMoves() ) << quint64( d->nCompletedMoves() )
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
int Simulation::nCompletedMoves()
{
    return d->nCompletedMoves();
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
    at the next sane point, and blocks until the simulation
    has stopped */
void Simulation::stop()
{
    d->stop();
}

/** Return whether or not the current simulation is running */
bool Simulation::isRunning()
{
    return d->isRunning();
}

/** Return whether or not the current simulation has finished */
bool Simulation::hasFinished()
{
    return d->hasFinished();
}

/** Wait until the simulation has finished (or 'time' milliseconds,
    whichever comes first). This returns whether or not the 
    simulation has finished. */
bool Simulation::wait(unsigned long time)
{
    return d->wait(time);
}
