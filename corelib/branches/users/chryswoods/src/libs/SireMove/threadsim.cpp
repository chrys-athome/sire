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

#include "threadsim.h"

#include "SireError/errors.h"

#include <QDebug>

using namespace SireMove;
using namespace SireSystem;

/** Constructor */
ThreadSim::ThreadSim() : LocalSim(), QThread()
{}

/** Construct to run the specified worker */
ThreadSim::ThreadSim(const MPISimWorker &worker)
          : LocalSim(worker), QThread()
{}
          
/** Destructor - wait until we have finished */
ThreadSim::~ThreadSim()
{
    LocalSim::stop();
    LocalSim::wait();
}

/** This is what is run by the background thread */
void ThreadSim::run()
{
    //only one thread may be running this job at a time
    QMutexLocker lkr(&run_mutex);

    //tell the parent thread that we have started
    start_mutex.lock();
    start_waiter.wakeAll();
    start_mutex.unlock();

    LocalSim::runJob();
}

/** Start the simulation in the background thread */
void ThreadSim::start()
{
    //Start the background thread, and block until the background
    //thread has definitely started!
    start_mutex.lock();
    QThread::start();
    start_waiter.wait(&start_mutex);
    start_mutex.unlock();
}
