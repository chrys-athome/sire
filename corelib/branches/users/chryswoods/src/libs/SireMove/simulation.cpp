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

#include "SireMPI/mpinode.h"

#include "simulation.h"
#include "threadsim.h"
#include "mpisim.h"

#include "SireStream/streamdata.hpp"
#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include "SireError/errors.h"

#include <QDebug>

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
LocalSim::LocalSim() : SimHandle()
{}

/** Construct from the passed worker */
LocalSim::LocalSim(const MPISimWorker &worker)
         : SimHandle(), sim_worker(worker)
{
    //save the initial state
    initial_state = SireStream::save(sim_worker);
}

/** Destructor */
LocalSim::~LocalSim()
{
    this->stop();
    this->wait();
}

/** Return a copy of the system being simulated */
System LocalSim::system()
{
    if (this->isError())
        this->throwError();

    return sim_worker.system();
}

/** Return a copy of the moves being applied to the system */
MovesPtr LocalSim::moves()
{
    if (this->isError())
        this->throwError();

    return sim_worker.moves();
}

/** Return a copy of the worker performing the simulation */
MPISimWorker LocalSim::worker()
{
    return sim_worker;
}

/** Return a copy of the worker in its initial state before
    the simulation started */
MPISimWorker LocalSim::initialWorker()
{
    return SireStream::loadType<MPISimWorker>(initial_state);
}

/** Return the total number of moves to be applied to the system
    during this simulation */
int LocalSim::nMoves()
{
    return sim_worker.nMoves();
}

/** Return the number of moves that have been completed */
int LocalSim::nCompleted()
{
    return sim_worker.nCompleted();
}

/** Return the current progress of the simulation (percentage of 
    completed moves) */
double LocalSim::progress()
{
    return sim_worker.progress();
}

/** Return whether or not statistics are being recorded during
    this simulation */
bool LocalSim::recordStatistics()
{
    return sim_worker.recordStatistics();
}

/** Return whether or not we are in an error condition */
bool LocalSim::isError()
{
    QMutexLocker lkr(&data_mutex);
    return not error_data.isEmpty();
}

/** Throw the exception that represents the error condition */
void LocalSim::throwError()
{
    QMutexLocker lkr(&data_mutex);
    
    if (not error_data.isEmpty())
        SireError::exception::unpackAndThrow(error_data);
}

/** Internal function used to set the error */
void LocalSim::setError(const SireError::exception &e)
{
    QMutexLocker lkr(&data_mutex);
    error_data = e.pack();
}

/** Clear any error condition */
void LocalSim::clearError()
{
    QMutexLocker lkr(&data_mutex);
    error_data.clear();
}

/** Function used to run the job */
void LocalSim::runJob()
{
    if (sim_worker.hasFinished())
        return;

    data_mutex.lock();
    stop_running = false;
    error_data = QByteArray();
    data_mutex.unlock();
    
    try
    {
        while (not sim_worker.hasFinished())
        {
            sim_worker.runChunk();
        
            //now do we need to stop?
            QMutexLocker lkr2(&data_mutex);
            
            if (stop_running)
                break;
        }
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
                            "An unknown error occured."), CODELOC ) );
    }
}

/** Start running the simulation */
void LocalSim::start()
{
    QMutexLocker run_lkr(&run_mutex);
    this->runJob();
}

/** Stop the simulation */
void LocalSim::stop()
{
    //stop the job
    data_mutex.lock();
    stop_running = true;
    data_mutex.unlock();
    
    //wait until the job stops
    run_mutex.lock();
    run_mutex.unlock();
}

/** Abort the simulation */
void LocalSim::abort()
{
    //stop the job
    this->stop();
    
    //prevent the job from running
    QMutexLocker lkr(&run_mutex);
    
    QMutexLocker lkr2(&data_mutex);
    sim_worker = SireStream::loadType<MPISimWorker>(initial_state);
}

/** Is the simulation running? */
bool LocalSim::isRunning()
{
    if (run_mutex.tryLock())
    {
        run_mutex.unlock();
        return false;
    }
    else
        return true;
}

/** Has the simulation started? */
bool LocalSim::hasStarted()
{
    return this->isRunning() or this->progress() > 0;
}

/** Has the simulation finished? */
bool LocalSim::hasFinished()
{
    return sim_worker.hasFinished();
}

/** Wait until the simulation has finished, or 'time' milliseconds
    has passed */
bool LocalSim::wait(int time)
{
    if (run_mutex.tryLock(time))
    {
        run_mutex.unlock();
        return true;
    }
    else
        return false;
}

/** Wait until the simulation has finished */
void LocalSim::wait()
{
    run_mutex.lock();
    run_mutex.unlock();
}

/////////
///////// Implementation of Simulation
/////////

static const RegisterMetaType<Simulation> r_sim;

/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, const Simulation &sim)
{
    writeHeader(ds, r_sim, 1);
    
    MPISimWorker worker = sim.d->worker();
    ds << SireStream::save(worker);
        
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, Simulation &sim)
{
    VersionID v = readHeader(ds, r_sim);
    
    if (v == 1)
    {
        QByteArray data;
        ds >> data;
        
        MPISimWorker worker = SireStream::loadType<MPISimWorker>(data);
        
        sim.d.reset( new LocalSim(worker) );
    }
    else
        throw version_error( v, "1", r_sim, CODELOC );
        
    return ds;
}

/** Null constructor */
Simulation::Simulation() : d( new LocalSim() )
{}

/** Private null constructor */
Simulation::Simulation(bool)
{}

/** Copy constructor */
Simulation::Simulation(const Simulation &other)
           : d( other.d )
{}

/** Destructor */
Simulation::~Simulation()
{}

/** Run a simulation described by the worker 'worker' in the current thread,
    returning a handle to the simulation once its done. */
Simulation Simulation::run(const MPISimWorker &worker)
{
    Simulation sim(true);
    
    sim.d.reset( new LocalSim(worker) );
    
    sim.start();
    
    return sim;
}

/** Run a simulation described by the worker 'worker' in a background thread
    and return a handle to the simulation (which may still be running) */
Simulation Simulation::runBG(const MPISimWorker &worker)
{
    Simulation sim(true);
    
    sim.d.reset( new ThreadSim(worker) );
    
    sim.start();
    
    return sim;
}

/** Run a simulation described by the worker 'worker' on the MPI node 'node',
    and return a handle to the simulation (which may still be running) */
Simulation Simulation::run(const MPINode &node, const MPISimWorker &worker)
{
    Simulation sim(true);
    
    sim.d.reset( new MPISim(node, worker) );
    
    sim.start();
    
    return sim;
}

/** Run a simulation consisting of nmoves moves on the system 'system' using
    the 'moves' object to actually perform the moves */
Simulation Simulation::run(const System &system, const Moves &moves,
                           int nmoves, bool record_stats, int chunk_size)
{
    return Simulation::run( MPISimWorker(system, moves, nmoves, 
                                         record_stats, chunk_size) );
}

/** Run a simulation consisting of nmoves moves on the system 'system' using
    the 'moves' object to actually perform the moves, in a background thread */
Simulation Simulation::runBG(const System &system, const Moves &moves,
                             int nmoves, bool record_stats, int chunk_size)
{
    return Simulation::runBG( MPISimWorker(system, moves, nmoves, 
                                           record_stats, chunk_size) );
}

/** Run a simulation consisting of nmoves moves on the system 'system' using
    the 'moves' object to actually perform the moves, on the MPI node 'mpinode' */
Simulation Simulation::run(const MPINode &node, const System &system,
                           const Moves &moves, int nmoves, bool record_stats,
                           int chunk_size)
{
    return Simulation::run( node, MPISimWorker(system, moves, nmoves, 
                                               record_stats, chunk_size) );
}

/** Run the simulation 'other' in the current thread and return a handle to the 
    simulation once it has completed */
Simulation Simulation::run(const Simulation &other)
{
    return Simulation::run( const_cast<Simulation*>(&other)->worker() );
}

/** Run the simulation 'other' in a background thread and return a handle to the
    simulation immediately */
Simulation Simulation::runBG(const Simulation &other)
{
    return Simulation::runBG( const_cast<Simulation*>(&other)->worker() );
}

/** Run the simulation 'other' on an MPI node and return a handle to the simulation */
Simulation Simulation::run(const MPINode &node, const Simulation &other)
{
    return Simulation::run( node, const_cast<Simulation*>(&other)->worker() );
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
MovesPtr Simulation::moves()
{
    return d->moves();
}

/** Return the worker performing the work */
MPISimWorker Simulation::worker()
{
    return d->worker();
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
bool Simulation::recordStatistics()
{
    return d->recordStatistics();
}

/** Start the simulation */
void Simulation::start()
{
    d->start();
}

/** Stop the current simulation. This will stop the simulation
    at the next sane point. This blocks until the simulation has
    stopped. */
void Simulation::stop()
{
    d->stop();
}

/** Abort the current simulation. This will abort the simulation
    and return the system to where it was before the simulation
    started. This blocks until the simulation has aborted. */
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
