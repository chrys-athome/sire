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

#ifndef SIREMOVE_SIMCONTROLLER_H
#define SIREMOVE_SIMCONTROLLER_H

#include <QMutex>

#include <boost/utility.hpp>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireSystem
{
class System;
}

namespace SireMove
{

namespace detail
{

/** Small exception class used by the simulation classes when
    a request is made of a simulation that is not running */
class sim_not_in_progress
{
public:
    sim_not_in_progress();
    ~sim_not_in_progress();
};

}

class Moves;

using SireSystem::System;

/** This class is used internally by the simulation classes to control
    a simulation that is in progress
    
    @author Christopher Woods
*/
class SIREMOVE_EXPORT SimController : public boost::noncopyable
{
public:
    SimController();
    ~SimController();

    bool nextMove();
    
    void initialise(const System &system, const Moves &moves,
                    int nmoves, bool record_stats);
                    
    void pause();
    void resume();
    
    void abort();
    void stop();
    
    bool aborted();
    
    void wait();
    
    int nMoves();
    int nCompleted();
    double progress();
    
    bool recordingStatistics();
    
    bool isRunning();
    bool hasFinished();
    bool hasStarted();
    
    System system();
    Moves moves();

private:
    /** Mutex used to start a simulation */
    QMutex start_mutex;

    /** Mutex that is held while the system is being simulated */
    QMutex run_mutex;
    
    /** Mutex that serialises access to the data in this controller */
    QMutex data_mutex;

    /** Pointer to the system being simulated */
    const System *sim_system;
    
    /** Pointer to the moves being applied to the system */
    const Moves *sim_moves;
    
    /** The number of moves being applied to the system */
    int nmoves;
    
    /** The number of completed moves on the system */
    int ncompleted;
    
    /** Whether or not statistics are being collected */
    bool record_stats;
    
    /** Whether or not the simulation has been aborted */
    bool sim_aborted;
};

}

SIRE_END_HEADER

#endif
