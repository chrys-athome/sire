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

#ifndef SIREMOVE_THREADSIM_H
#define SIREMOVE_THREADSIM_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

#include "simulation.h"
#include "moves.h"
#include "mpisimworker.h"

#include "SireSystem/system.h"

SIRE_BEGIN_HEADER

namespace SireMove
{

/** This is the simulation handle that lets a simulation run in a background thread

    @author Christopher Woods
*/
class SIREMOVE_EXPORT ThreadSim : public LocalSim, private QThread
{
public:
    ThreadSim();
    ThreadSim(const MPISimWorker &worker);
              
    ~ThreadSim();
    
    void start();

protected:
    void run();

private:
    /** Mutex used to control starting of the background thread */
    QMutex start_mutex;
    
    /** Wait condition used to control starting of the background thread */
    QWaitCondition start_waiter;
};

}

SIRE_END_HEADER

#endif
