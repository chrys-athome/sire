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

#include <QMutex>

#include "moves.h"

#include "SireSystem/system.h"

namespace SireMove
{
class Simulation;
}

QDataStream& operator<<(QDataStream&, const SireMove::Simulation&);
QDataStream& operator>>(QDataStream&, SireMove::Simulation&);

namespace SireMove
{

using namespace SireSystem;

/** This class provides a way of following (and controlling) moves
    as they are being performed */
class SIREMOVE_EXPORT MoveMutex
{
public:
    MoveMutex();
    ~MoveMutex();

    void initialise(int nmoves, const System &system, 
                                const Moves &moves);
    
    bool isRunning();
    
    bool nextMove();
    
    int nMoves();
    int nCompleted();
    
    System system();
    Moves moves();
    
    void pause();
    void resume();
    void abort();
    
private:
    MoveMutex(const MoveMutex&)
    {}
    
    MoveMutex& operator=(const MoveMutex&)
    {
        return *this;
    }
    
    QMutex mutex;
    
    int nmoves;
    int ncompleted;
    
    const System *sim_system;
    const Moves *sim_moves;
};

/** This class makes it easier to manage the locking and unlocking of a simulation */
class MoveMutexLocker
{
public:
    MoveMutexLocker() : mutex(0)
    {}
    
    MoveMutexLocker(MoveMutex &movemutex) : mutex( &movemutex )
    {
        mutex->pause();
    }
     
    ~MoveMutexLocker()
    {
        if (mutex)
            mutex->resume();
    }
    
private:
    MoveMutex *mutex;
};

/** This the virtual base class provides the interface to the handle
    by which 'Simulation' actually controls a running simulation
    
    @author Christopher Woods
*/
class SIREMOVE_EXPORT SimHandle
{
public:
    SimHandle();
    
    virtual ~SimHandle();
    
    virtual System system()=0;
    virtual Moves moves()=0;

    virtual int nMoves()=0;
    virtual int nCompletedMoves()=0;
    
    virtual bool recordingStatistics()=0;
    
    virtual void start()=0;
    
    virtual void pause()=0;
    virtual void resume()=0;
    
    virtual void stop()=0;
    
    virtual bool isRunning()=0;
    virtual bool hasFinished()=0;
    
    virtual bool wait(unsigned long time)=0;
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
    Simulation(const System &system, const MovesBase &moves,
               int nmoves, bool record_stats = true);
    
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
    
    bool wait(unsigned long time = ULONG_MAX);

private:
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
