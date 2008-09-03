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

#ifndef SIREMOVE_MPISIM_H
#define SIREMOVE_MPISIM_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

#include "simulation.h"
#include "moves.h"

#include "mpinode.h"

#include "SireSystem/system.h"

SIRE_BEGIN_HEADER

namespace SireMove
{

/** This is a simulation handle that can be used to run a simulation on
    a remote node using MPI
    
    @author Christopher Woods
*/
class SIREMOVE_EXPORT MPISim : public SimHandle, private QThread
{
public:
    MPISim();

    MPISim(const MPINode &node, const System &system, const MovesBase &moves,
           int nmoves, bool record_stats);
    
    ~MPISim();
    
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
    
    void wait();
    bool wait(int time);

private:
    void run();

    bool isStarting();
    void setError();

    /** The node that will be used to run this simulation */
    MPINode mpinode;

    /** Mutex locked while the simulation is running */
    QMutex run_mutex;

    /** Mutex protecting access to the data of this simulation */
    QMutex data_mutex;
    
    /** Condition used while the simulation is starting */
    QWaitCondition starter;

    /** The handle used to get the result of the running simulation */
    MPIPromise< tuple<System,Moves,qint32> > sim_result;
    
    /** Pointer to a possible error condition */
    boost::shared_ptr<SireError::exception> error_ptr;
    
    /** Is the simulation in the process of being started */
    bool sim_starting;
    
    /** Has the simulation started? */
    bool has_started;
};

}

#endif
