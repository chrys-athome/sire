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

#ifndef SIREMOVE_SIMULATION_H
#define SIREMOVE_SIMULATION_H

#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>

#include <QMutex>

#include "moves.h"
#include "mpisimworker.h"

#include "SireSystem/system.h"

namespace SireMove
{
class Simulation;
}

QDataStream& operator<<(QDataStream&, const SireMove::Simulation&);
QDataStream& operator>>(QDataStream&, SireMove::Simulation&);

namespace SireMPI
{
class MPINode;
}

namespace SireMove
{

using SireMPI::MPINode;

using namespace SireSystem;

/** This the virtual base class provides the interface to the handle
    by which 'Simulation' actually controls a running simulation
    
    @author Christopher Woods
*/
class SIREMOVE_EXPORT SimHandle : public boost::noncopyable
{
public:
    SimHandle();
    
    virtual ~SimHandle();
    
    virtual System system()=0;
    virtual Moves moves()=0;

    virtual MPISimWorker worker()=0;
    virtual MPISimWorker initialWorker()=0;

    virtual int nMoves()=0;
    virtual int nCompleted()=0;
    virtual double progress()=0;
    
    virtual bool recordStatistics()=0;
    
    virtual void start()=0;
    
    virtual void abort()=0;
    virtual void stop()=0;
    
    virtual bool isRunning()=0;
    virtual bool hasStarted()=0;
    virtual bool hasFinished()=0;
    
    virtual bool isError()=0;
    virtual void throwError()=0;
    virtual void clearError()=0;
    
    virtual void wait()=0;
    virtual bool wait(int time)=0;
};

/** This is a simple class that provides a simulation that
    runs within the current thread */
class LocalSim : public SimHandle
{
public:
    LocalSim();

    LocalSim(const MPISimWorker &worker);
    
    ~LocalSim();
    
    System system();
    Moves moves();

    MPISimWorker worker();
    MPISimWorker initialWorker();

    int nMoves();
    int nCompleted();
    
    double progress();
    
    bool recordStatistics();
    
    void start();
    
    void abort();
    void stop();
    
    bool isRunning();
    bool hasStarted();
    bool hasFinished();
    
    bool isError();
    void throwError();
    void clearError();
    
    void wait();
    bool wait(int time);

protected:
    void setError(const SireError::exception &e);
    void runJob();

    /** Mutex to protect access to the data of this simulation */
    QMutex data_mutex;

    /** Mutex to ensure that only a single job is running at a time */
    QMutex run_mutex;

    /** The worker used to do the work */
    MPISimWorker sim_worker;
    
    /** The initial data for the simulation, packed into a binary format.
        This is used to restore the simulation if it is aborted */
    QByteArray initial_state;
    
    /** A binary representation of any error that has occured during 
        the simulation */
    QByteArray error_data;

    /** Whether or not to stop running */
    bool stop_running;
};

/** This class provides a user controllable handle to a running
    simulation. 
    
    @author Christopher Woods
*/
class SIREMOVE_EXPORT Simulation
{

friend QDataStream& ::operator<<(QDataStream&, const Simulation&);
friend QDataStream& ::operator>>(QDataStream&, Simulation&);

public:
    Simulation();
    
    Simulation(const Simulation &other);
    
    ~Simulation();
    
    Simulation& operator=(const Simulation &other);
    
    bool operator==(const Simulation &other) const;
    bool operator!=(const Simulation &other) const;
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<Simulation>() );
    }
    
    const char* what() const
    {
        return Simulation::typeName();
    }
    
    Simulation* clone() const
    {
        return new Simulation(*this);
    }

    static Simulation run(const System &system, const MovesBase &moves,
                          int nmoves, bool record_stats=true,
                          int chunk_size=100);
    
    static Simulation runBG(const System &system, const MovesBase &moves,
                            int nmoves, bool record_stats=true,
                            int chunk_size=100);
    
    static Simulation run(const MPINode &node, const System &system,
                          const MovesBase &moves, int nmoves,
                          bool record_stats=true,
                          int chunk_size=100);

    static Simulation run(const Simulation &other);
    static Simulation runBG(const Simulation &other);
    static Simulation run(const MPINode &node, const Simulation &other);
    
    static Simulation run(const MPISimWorker &worker);
    static Simulation runBG(const MPISimWorker &worker);
    static Simulation run(const MPINode &node, const MPISimWorker &worker);
    
    System system();
    Moves moves();

    MPISimWorker worker();

    int nMoves();
    int nCompleted();
    double progress();
    
    bool recordStatistics();
    
    void start();
    
    void abort();
    void stop();
    
    bool isRunning();
    bool hasStarted();
    bool hasFinished();
    
    bool isError();
    void throwError();
    void clearError();
    
    void wait();
    bool wait(int time);

private:
    /** Explicitly shared pointer to the object that is actually
        running the simulation */
    boost::shared_ptr<SimHandle> d;
};

}

Q_DECLARE_METATYPE( SireMove::Simulation )

SIRE_EXPOSE_CLASS( SireMove::Simulation )

#endif
