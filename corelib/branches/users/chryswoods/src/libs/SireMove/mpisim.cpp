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

#include "mpisim.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include "SireError/errors.h"

#include <QDebug>

using namespace SireMove;
using namespace SireStream;

MPISim::MPISim();

MPISim::MPISim(const MPISimWorker &worker);

MPISim::~MPISim();

System MPISim::system()
{
    QMutexLocker lkr(&data_mutex);
    
    if (data_is_local)
        return sim_worker.system();
        
    else
        return mpipromise.interimResult<MPISimWorker>().system();
}

Moves MPISim::moves();

MPISimWorker MPISim::worker();
MPISimWorker MPISim::initialWorker();

int MPISim::nMoves();
int MPISim::nCompleted();

double MPISim::progress();

bool MPISim::recordStatistics();

void MPISim::run()
{
    start_mutex.lock();

    //start the job
    {
        QMutexLocker lkr(&data_mutex);

        //start the job on the remote node, and get a handle to
        //the running job
        mpipromise = mpinode.start(sim_worker);
        sim_worker = MPISimWorker();

        //the data for the simulation now resides on the remote host
        data_is_local = false;
    }

    //tell the parent thread that the job has started
    start_waiter.wakeAll();
    start_mutex.unlock();
    
    //wait until the job has finished
    mpipromise.wait();
    
    try
    {
        QMutexLocker lkr(&data_mutex);
        sim_worker = mpipromise.result<MPISimWorker>();
        mpipromise = MPIPromise();

        //the data for the simulation is now back on the this node
        data_is_local = true;
    }
    catch(const SireError::exception &e)
    {
        this->setError(e);
    }
    catch(...)
    {
        this->setError( SireError::program_bug( QObject::tr(
            "It should not be possible to have an unknown error here!"), CODELOC ) );
    }
}

void MPISim::start()
{
    QThread::wait();
    
    start_mutex.lock();
    QThread::start();
    start_waiter.wait(&start_mutex);
    start_mutex.unlock();
}

void MPISim::stop()
{
    data_mutex.lock();
    
    if (mpipromise.isRunning())
        mpipromise.stop();
        
    data_mutex.unlock();
    
    QThread::wait();
}

void MPISim::abort()
{
    this->stop();
    
    QMutexLocker lkr(&data_mutex);
    sim_worker = SireStream::loadType<MPISimWorker>(initial_state);
}


bool MPISim::isRunning();
bool MPISim::hasStarted();
bool MPISim::hasFinished();

bool MPISim::isError();
void MPISim::throwError();
void MPISim::clearError();

void MPISim::wait();
bool MPISim::wait(int time);
