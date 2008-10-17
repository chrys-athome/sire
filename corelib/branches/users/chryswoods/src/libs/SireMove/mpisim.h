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

#include "SireMPI/mpinode.h"
#include "SireMPI/mpipromise.h"

#include "SireSystem/system.h"

SIRE_BEGIN_HEADER

namespace SireMove
{

using SireMPI::MPINode;
using SireMPI::MPIPromise;

/** This class is used to run a simulation on a remote node connected
    to this node via MPI
    
    @author Christopher Woods
*/
class SIREMOVE_EXPORT MPISim : public SimHandle
{
public:
    MPISim();

    MPISim(const MPINode &node, const MPISimWorker &worker);

    ~MPISim();
    
    System system();
    MovesPtr moves();

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

private:
    void _pvt_setWorker(const MPISimWorker &worker);
    void _pvt_setError(const SireError::exception &e);
    void _pvt_makeDataLocal();
    
    QMutex start_mutex;
    QMutex data_mutex;
    
    MPISimWorker sim_worker;
    
    MPINode mpinode;
    MPIPromise mpipromise;
    
    QByteArray initial_state;
    QByteArray error_data;
    
    int nmoves;
    
    bool record_stats;
    bool data_is_local;
};

}

SIRE_END_HEADER

#endif
