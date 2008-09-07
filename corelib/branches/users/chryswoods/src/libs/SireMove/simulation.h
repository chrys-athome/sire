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
#include "simcontroller.h"

#include "SireSystem/system.h"

namespace SireMove
{
class Simulation;
}

QDataStream& operator<<(QDataStream&, const SireMove::Simulation&);
QDataStream& operator>>(QDataStream&, SireMove::Simulation&);

namespace SireMove
{

class MPINode;

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

    virtual int nMoves()=0;
    virtual int nCompleted()=0;
    virtual double progress()=0;
    
    virtual bool recordingStatistics()=0;
    
    virtual void start()=0;
    
    virtual void pause()=0;
    virtual void resume()=0;

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

    LocalSim(const System &system, const MovesBase &moves,
             int nmoves, int ncomp, bool record_stats);
    
    ~LocalSim();
    
    System system();
    Moves moves();

    int nMoves();
    int nCompleted();
    double progress();
    
    bool recordingStatistics();
    
    void start();
    
    void pause();
    void resume();
    
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

    /** The simulation controller */
    SimController controller;

    /** Mutex to protect access to the data of this simulation */
    QMutex data_mutex;

    /** Mutex to ensure that only one copy of this simulation is 
        running at a time */
    QMutex run_mutex;

    /** A copy of the system being simulated */
    System sim_system;
    
    /** A copy of the moves being applied to the system */
    Moves sim_moves;
    
    /** Pointer to any error that has occurred during the simulation */
    boost::shared_ptr<SireError::exception> error_ptr;
    
    /** The number of moves to run */
    int nmoves;
    
    /** The number of moves completed */
    int ncompleted;
    
    /** Whether or not statistics are being collected during this
        simulation */
    bool record_stats;
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
                          int nmoves, bool record_stats=true);
    
    static Simulation runBG(const System &system, const MovesBase &moves,
                            int nmoves, bool record_stats=true);
    
    static Simulation run(const MPINode &node, const System &system,
                          const MovesBase &moves, int nmoves,
                          bool record_stats=true);
    
    System system();
    Moves moves();

    int nMoves();
    int nCompleted();
    double progress();
    
    bool recordingStatistics();
    
    void start();
    
    void pause();
    void resume();
    
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
    Simulation(const System &system, const MovesBase &moves,
               int nmoves, bool record_stats = true);

    Simulation(const System &system, const MovesBase &moves,
               int nmoves, int ncompleted, bool record_stats);

    /** Explicitly shared pointer to the object that is actually
        running the simulation */
    boost::shared_ptr<SimHandle> d;
};

}

Q_DECLARE_METATYPE( SireMove::Simulation )

SIRE_EXPOSE_CLASS( SireMove::Simulation )

#endif
