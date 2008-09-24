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

#include "mpipromise.h"
#include "mpiworker.h"

#include "detail/mpidetail.h"   // CONDITIONAL_INCLUDE

#include "SireError/errors.h"

#include <QDebug>

using namespace SireMPI;
using namespace SireMPI::detail;

using boost::shared_ptr;

namespace SireMPI
{

namespace detail
{

/** Private implementation of MPIPromise */
class MPIPromiseData
{
public:
    MPIPromiseData();
    MPIPromiseData(const MPIWorker &worker, const MPINode &node);
    
    ~MPIPromiseData();
    
    QByteArray input();
    QByteArray interimResult();
    QByteArray result();
    
    void abort();
    void stop();
    
    void wait();
    bool wait(int ms);
    
    bool isRunning();
    
    bool isError();
    
    bool isStopped();
    bool isAborted();
    
    double progress();
    
    void setProgress(double progress_percentage);
    
    void setInterimData(const QByteArray &worker_data, double progress);
    void setFinalData(const QByteArray &worker_data);

    void setError(const MPIError &error);

    void setStopped(const QByteArray &worker_data, double progress);

    void setAborted();
    
    MPINode node();
    
private:
    /** Mutex to serialise access to the data of this object */
    QMutex data_mutex;

    /** Condition used to wait for progress updates */
    QWaitCondition progress_waiter;
    
    /** Condition used to wait for interim results */
    QWaitCondition interim_waiter;
    
    /** Condition used to wait for the final results */
    QWaitCondition result_waiter;
    
    /** The node on which the job is running */
    MPINode mpinode;
    
    /** Binary representation of the worker in its initial state */
    QByteArray initial_worker_data;
    
    /** Binary representation of the worker in an intermediate state */
    QByteArray interim_worker_data;
    
    /** Binary representation of the worker in its final state */
    QByteArray final_worker_data;
    
    /** Binary representation of any error */
    QByteArray error_data;
    
    /** The progress of the calculation */
    double current_progress;
};

} // end of namespace detail

} // end of namespace SireMPI

////////////
//////////// Implementation of MPIPromiseData
////////////

/** Constructor */
MPIPromiseData::MPIPromiseData() : current_progress(0)
{}

/** Construct the promise for the specified work on the specified node */
MPIPromiseData::MPIPromiseData(const MPIWorker &worker,
                               const MPINode &node)
               : mpinode(node), current_progress(0)
{
    qDebug() << CODELOC;

    //get a binary representation of the worker
    initial_worker_data = SireStream::save(worker);

    qDebug() << CODELOC;
}

/** Destructor */
MPIPromiseData::~MPIPromiseData()
{}

/** Return the node on which the job is running - this will be null
    if the job isn't running */
MPINode MPIPromiseData::node()
{
    QMutexLocker lkr(&data_mutex);
    return mpinode;
}

/** Return the initial state (the input work). This can be used
    to restore the state if there was an error or if the job
    was aborted */
QByteArray MPIPromiseData::input()
{
    QMutexLocker lkr(&data_mutex);
    
    if (initial_worker_data.isEmpty())
        throw SireError::invalid_state( QObject::tr(
            "This is a null MPIPromise, with no associated work!"), CODELOC );
        
    return initial_worker_data;
}

/** Return whether or not we are in an error state */
bool MPIPromiseData::isError()
{
    QMutexLocker lkr(&data_mutex);
    
    return not error_data.isEmpty();
}

/** Return whether or not this work has been stopped */
bool MPIPromiseData::isStopped()
{
    QMutexLocker lkr(&data_mutex);
    
    return current_progress < 100 and (not final_worker_data.isEmpty());
}

/** Return whether or not this work has been aborted
    (stopped and reset to input) */
bool MPIPromiseData::isAborted()
{
    QMutexLocker lkr(&data_mutex);
    return mpinode.isNull() and final_worker_data.isEmpty();
}

/** Abort the job */
void MPIPromiseData::abort()
{
    QMutexLocker lkr(&data_mutex);

    if (not mpinode.isNull())
    {
        mpinode.abort();
        result_waiter.wait(&data_mutex);
    }
}

/** Stop the job */
void MPIPromiseData::stop()
{
    QMutexLocker lkr(&data_mutex);
    
    if (not mpinode.isNull())
    {
        mpinode.stop();
        result_waiter.wait(&data_mutex);
    }
}

/** Wait until the job has stopped running (either because it is done,
    an error has occured, it has been stopped or it has been aborted) */
void MPIPromiseData::wait()
{
    qDebug() << CODELOC;

    QMutexLocker lkr(&data_mutex);

    qDebug() << CODELOC;
    
    if (not mpinode.isNull())
    {
        qDebug() << CODELOC;
        result_waiter.wait(&data_mutex);
    }
}

/** Wait until the job has stopped running (either because it is done,
    an error has occured, it has been stopped or it has been aborted), or
    until 'ms' milliseconds have passed, whichever comes sooner. Returns
    whether or not the job has stopped */
bool MPIPromiseData::wait(int ms)
{
    QMutexLocker lkr(&data_mutex);
    
    if (not mpinode.isNull())
    {
        return result_waiter.wait(&data_mutex, ms);
    }
    else
        return true;
}

/** Return whether or not the job is still running */
bool MPIPromiseData::isRunning()
{
    QMutexLocker lkr(&data_mutex);
    return not mpinode.isNull();
}

/** Return an interim result - this blocks until an interim result
    is available */
QByteArray MPIPromiseData::interimResult()
{
    QMutexLocker lkr(&data_mutex);
    
    if (not error_data.isEmpty())
        //throw the error!
        SireError::exception::unpackAndThrow(error_data);
        
    else if (final_worker_data.isEmpty())
    {
        //the job hasn't finished yet
        if (mpinode.isNull())
            throw SireError::invalid_state( QObject::tr(
                "There is no interim result as this work associated with this "
                "promise is not running on any nodes!"), CODELOC );
                
        //tell the node to give us an interim result
        mpinode.getInterimResult();
        
        //wait for the result
        interim_waiter.wait(&data_mutex);
    }

    //ok, we now either have a result, or an error
    if (not error_data.isEmpty())
        SireError::exception::unpackAndThrow(error_data);
        
    if (interim_worker_data.isEmpty())
        //the job has been aborted, or just didn't get anywhere
        return initial_worker_data;
    
    else
        return interim_worker_data;
}

/** Return the final result of the work - this blocks until the result
    is available (or until an error occurs!) */
QByteArray MPIPromiseData::result()
{
    QMutexLocker lkr(&data_mutex);
    
    if (not error_data.isEmpty())
        //throw the error!
        SireError::exception::unpackAndThrow(error_data);
        
    else if (final_worker_data.isEmpty())
    {
        //the job hasn't finished yet
        if (mpinode.isNull())
            throw SireError::invalid_state( QObject::tr(
                "There is no interim result as this work associated with this "
                "promise is not running on any nodes!"), CODELOC );
                
        //wait for the result to arrive
        result_waiter.wait(&data_mutex);
    }

    //ok, we now either have a result, or an error
    if (not error_data.isEmpty())
        SireError::exception::unpackAndThrow(error_data);
        
    if (final_worker_data.isEmpty())
        //the job must have been aborted, or just didn't
        //get anywhere
        return initial_worker_data;
    
    else    
        return final_worker_data;
}

/** Return the progress of the calculation */
double MPIPromiseData::progress()
{
    QMutexLocker lkr(&data_mutex);
    
    if (current_progress >= 100 or mpinode.isNull())
        return current_progress;
        
    else
    {
        //tell the node to get the progress
        mpinode.getProgress();
        
        //wait until the progress has been obtained
        progress_waiter.wait( &data_mutex );
        
        return current_progress;
    }
}

/** This function is called by the node to set the progress
    of the calculation */
void MPIPromiseData::setProgress(double progress)
{
    QMutexLocker lkr(&data_mutex);
    current_progress = progress;
    progress_waiter.wakeAll();
}

/** Set the interim result */
void MPIPromiseData::setInterimData(const QByteArray &worker_data, double progress)
{
    QMutexLocker lkr(&data_mutex);
    interim_worker_data = worker_data;
    current_progress = progress;
    
    progress_waiter.wakeAll();
    interim_waiter.wakeAll();
}

/** Set the final result */
void MPIPromiseData::setFinalData(const QByteArray &worker_data)
{
    QMutexLocker lkr(&data_mutex);
    error_data = QByteArray();
    interim_worker_data = worker_data;
    final_worker_data = worker_data;
    current_progress = 100;
    
    //the job has finished - we don't need the node any more
    mpinode = MPINode();
    
    interim_waiter.wakeAll();
    progress_waiter.wakeAll();
    result_waiter.wakeAll();
}

/** Oh no - the work has ended in error! */
void MPIPromiseData::setError(const MPIError &error)
{
    QMutexLocker lkr(&data_mutex);
    interim_worker_data = QByteArray();
    final_worker_data = QByteArray();
    
    error_data = error.errorData();
    current_progress = 0;
    
    //the job has finished - we don't need the node any more
    mpinode = MPINode();
    
    interim_waiter.wakeAll();
    progress_waiter.wakeAll();
    result_waiter.wakeAll();
}

/** Tell the promise that the job has been stopped, and supply the most
    up to date result (together with the percent progress of that result) */
void MPIPromiseData::setStopped(const QByteArray &worker_data, double progress)
{
    QMutexLocker lkr(&data_mutex);

    mpinode = MPINode();
    
    current_progress = progress;
    error_data = QByteArray();
    
    if (current_progress == 0)
    {
        interim_worker_data = QByteArray();
        final_worker_data = QByteArray();
    }
    else
    {
        interim_worker_data = worker_data;
        final_worker_data = worker_data;
    }
    
    progress_waiter.wakeAll();
    interim_waiter.wakeAll();
    result_waiter.wakeAll();
}

/** Tell the promise that the job has been aborted */
void MPIPromiseData::setAborted()
{
    QMutexLocker lkr(&data_mutex);
    
    mpinode = MPINode();
    
    current_progress = 0;
    
    error_data = QByteArray();
    interim_worker_data = QByteArray();
    final_worker_data = QByteArray();
    
    progress_waiter.wakeAll();
    interim_waiter.wakeAll();
    result_waiter.wakeAll();
}


////////////
//////////// Implementation of MPIPromise
////////////

/** Create a null promise */
MPIPromise::MPIPromise()
{}

/** Private constructor used by MPINode */
MPIPromise::MPIPromise(const MPIWorker &worker, const MPINode &node)
           : d( new MPIPromiseData(worker, node) )
{}

/** Copy constructor */
MPIPromise::MPIPromise(const MPIPromise &other) : d(other.d)
{}

/** Destructor */
MPIPromise::~MPIPromise()
{}

/** Copy assignment operator */
MPIPromise& MPIPromise::operator=(const MPIPromise &other)
{
    d = other.d;
    return *this;
}

/** Comparison operator */
bool MPIPromise::operator==(const MPIPromise &other) const
{
    return d.get() == other.d.get();
}

/** Comparison operator */
bool MPIPromise::operator!=(const MPIPromise &other) const
{
    return d.get() != other.d.get();
}

/** Return whether or not this is the null promise */
bool MPIPromise::isNull() const
{
    return d.get() == 0;
}

/** Return the node on which this job is running - this will be a null
    node if this job isn't running */
MPINode MPIPromise::node()
{
    if (this->isNull())
        return MPINode();
    
    else
        return d->node();
}

/** Abort this job - this blocks until the job is aborted */
void MPIPromise::abort()
{
    if (not this->isNull())
        d->abort();
}

/** Stop this job - this blocks until the job has stopped */
void MPIPromise::stop()
{
    if (not this->isNull())
        d->stop();
}

/** Wait until the job has stopped running (either because it is done,
    an error has occured, it has been stopped or it has been aborted) */
void MPIPromise::wait()
{
    qDebug() << CODELOC;

    if (not this->isNull())
    {
        qDebug() << CODELOC;
        d->wait();
    }
}

/** Wait until the job has stopped running (either because it is done,
    an error has occured, it has been stopped or it has been aborted), or
    until 'ms' milliseconds have passed, whichever comes sooner. Returns
    whether or not the job has stopped */
bool MPIPromise::wait(int ms)
{
    if (not this->isNull())
        return d->wait(ms);
    
    else
        return true;
}

/** Return whether or not the job is still running */
bool MPIPromise::isRunning()
{
    if (not this->isNull())
        return d->isRunning();

    else
        return false;
}

/** Return whether or not we are in an error state */
bool MPIPromise::isError()
{
    if (not this->isNull())
        return d->isError();

    else
        return false;
}

/** Return whether or not this work has been stopped */
bool MPIPromise::isStopped()
{
    if (not this->isNull())
        return d->isStopped();

    else
        return false;
}

/** Return whether or not this work has been aborted
    (stopped and reset to input) */
bool MPIPromise::isAborted()
{
    if (not this->isNull())
        return d->isAborted();

    else
        return false;
}

/** Return the progress of the calculation. This blocks until it 
    receives a progress report from the calculation */
double MPIPromise::progress()
{
    if (not this->isNull())
        return d->progress();

    else
        return 0;
}

/** This private function is called by the node to set the progress
    of the calculation */
void MPIPromise::setProgress(double progress)
{
    if (not this->isNull())
        d->setProgress(progress);
}

/** This private function is called by the node to set the final result */
void MPIPromise::setFinalData(const QByteArray &worker_data)
{
    if (this->isNull())
        return;

    if (SireStream::getDataHeader(worker_data).dataType() 
                                == QLatin1String(MPIError::typeName()))
    {
        d->setError( SireStream::loadType<MPIError>(worker_data) );
    }
    else
    {
        d->setFinalData(worker_data);
    }
}

/** This private function is used to set the interim result */
void MPIPromise::setInterimData(const QByteArray &worker_data, double progress)
{
    if (this->isNull())
        return;

    if (SireStream::getDataHeader(worker_data).dataType() 
                                == QLatin1String(MPIError::typeName()))
    {
        d->setError( SireStream::loadType<MPIError>(worker_data) );
    }
    else
    {
        d->setInterimData(worker_data, progress);
    }
}

/** Private function used by the node to tell the promise that the job has 
    been stopped, and supply the most up to date result (together with the 
    percent progress of that result) */
void MPIPromise::setStopped(const QByteArray &worker_data, double progress)
{
    if (this->isNull())
        return;

    if (SireStream::getDataHeader(worker_data).dataType() 
                                == QLatin1String(MPIError::typeName()))
    {
        d->setError( SireStream::loadType<MPIError>(worker_data) );
    }
    else
    {
        d->setStopped(worker_data, progress);
    }
}

/** Private function used by the node to tell the promise that the j
    ob has been aborted */
void MPIPromise::setAborted()
{
    if (this->isNull())
        return;

    d->setAborted();
}

/** Return the final result of the work - this blocks until the result
    is available (or until an error occurs!) */
QByteArray MPIPromise::finalData()
{
    if (not this->isNull())
        return d->result();

    else 
        return QByteArray();
}

/** Return an interim result - this blocks until an interim result
    is available */
QByteArray MPIPromise::interimData()
{
    if (not this->isNull())
        return d->interimResult();

    else 
        return QByteArray();
}

/** Return the initial state (the input work). This can be used
    to restore the state if there was an error or if the job
    was aborted */
QByteArray MPIPromise::initialData()
{
    if (not this->isNull())
        return d->input();

    else 
        return QByteArray();
}
