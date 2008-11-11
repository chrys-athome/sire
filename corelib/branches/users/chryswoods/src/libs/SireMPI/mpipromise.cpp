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

#include <QWaitCondition>

#include "mpipromise.h"
#include "mpiworker.h"
#include "mpinode.h"
#include "mpifrontends.h"

#include "SireError/errors.h"

#include <QDebug>

using namespace SireMPI;

using boost::shared_ptr;

namespace SireMPI
{
namespace detail
{

/** Private implementation of MPIPromise */
class MPIPromisePvt
{
public:
    MPIPromisePvt(const MPIWorker &worker, const MPINode &mpinode)
            : current_progress(0)
    {
        node = node;
        initial_worker_data = SireStream::save(worker);
    }

    ~MPIPromisePvt()
    {}
    
    /** Mutex to serialise access to the data of this object */
    QMutex datamutex;

    /** Condition used to wait for progress updates */
    QWaitCondition progress_waiter;
    
    /** Condition used to wait for interim results */
    QWaitCondition interim_waiter;
    
    /** Condition used to wait for the final results */
    QWaitCondition result_waiter;
    
    /** The node on which the job is running */
    MPINodePtr node;
    
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

using namespace SireMPI::detail;

////////////
//////////// Implementation of MPIPromise
////////////

/** Create a null promise */
MPIPromise::MPIPromise()
{}

/** Private constructor used by MPINode */
MPIPromise::MPIPromise(const MPIWorker &worker, const MPINode &node)
           : d( new MPIPromisePvt(worker, node) )
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

/** Return the node on which the job is running - this will be null
    if the job isn't running */
MPINode MPIPromise::node()
{
    if (this->isNull())
        return MPINode();

    QMutexLocker lkr( &(d->datamutex) );
    
    if (d->node.isNull())
        return MPINode();
        
    else
        return *(d->node);
}

/** Return the initial state (the input work). This can be used
    to restore the state if there was an error or if the job
    was aborted */
QByteArray MPIPromise::initialData()
{
    if (this->isNull())
        return QByteArray();

    QMutexLocker lkr( &(d->datamutex) );
    
    if (d->initial_worker_data.isEmpty())
        throw SireError::invalid_state( QObject::tr(
            "This is a null MPIPromise, with no associated work!"), CODELOC );
        
    return d->initial_worker_data;
}

/** Return whether or not we are in an error state */
bool MPIPromise::isError()
{
    if (this->isNull())
        return false;

    QMutexLocker lkr( &(d->datamutex) );
    
    return not d->error_data.isEmpty();
}

/** Return whether or not this work has been stopped */
bool MPIPromise::isStopped()
{
    if (this->isNull())
        return false;

    QMutexLocker lkr( &(d->datamutex) );
    
    return d->current_progress < 100 and (not d->final_worker_data.isEmpty());
}

/** Return whether or not this work has been aborted
    (stopped and reset to input) */
bool MPIPromise::isAborted()
{
    if (this->isNull())
        return false;

    QMutexLocker lkr( &(d->datamutex) );
    return d->node.isNull() and d->final_worker_data.isEmpty();
}

/** Abort the job */
void MPIPromise::abort()
{
    if (this->isNull())
        return;

    QMutexLocker lkr( &(d->datamutex) );

    if (not d->node.isNull())
    {
        MPINode node = *(d->node);
        node.abort();
        d->result_waiter.wait( &(d->datamutex) );
    }
}

/** Stop the job */
void MPIPromise::stop()
{
    if (this->isNull())
        return;

    QMutexLocker lkr( &(d->datamutex) );

    if (not d->node.isNull())
    {
        MPINode node = *(d->node);
        node.stop();
        d->result_waiter.wait( &(d->datamutex) );
    }
}

/** Wait until the job has stopped running (either because it is done,
    an error has occured, it has been stopped or it has been aborted) */
void MPIPromise::wait()
{
    if (this->isNull())
        return;

    QMutexLocker lkr( &(d->datamutex) );
    
    if (not d->node.isNull())
    {
        d->result_waiter.wait( &(d->datamutex) );
    }
}

/** Wait until the job has stopped running (either because it is done,
    an error has occured, it has been stopped or it has been aborted), or
    until 'ms' milliseconds have passed, whichever comes sooner. Returns
    whether or not the job has stopped */
bool MPIPromise::wait(int ms)
{
    if (this->isNull())
        return true;

    QMutexLocker lkr( &(d->datamutex) );
    
    if (not d->node.isNull())
    {
        return d->result_waiter.wait( &(d->datamutex), ms );
    }
    else
        return true;
}

/** Return whether or not the job is still running */
bool MPIPromise::isRunning()
{
    if (this->isNull())
        return false;

    QMutexLocker lkr( &(d->datamutex) );
    return not d->node.isNull();
}

/** Return an interim result - this blocks until an interim result
    is available */
QByteArray MPIPromise::interimData()
{
    if (this->isNull())
        return QByteArray();

    QMutexLocker lkr( &(d->datamutex) );
    
    if (not d->error_data.isEmpty())
        //throw the error!
        SireError::exception::unpackAndThrow(d->error_data);
        
    else if (d->final_worker_data.isEmpty())
    {
        //the job hasn't finished yet
        if (d->node.isNull())
            throw SireError::invalid_state( QObject::tr(
                "There is no interim result as this work associated with this "
                "promise is not running on any nodes!"), CODELOC );
           
        //tell the node to give us an interim result
        getFrontEnd( *(d->node) ).requestInterimResult();
                        
        //wait for the result
        d->interim_waiter.wait( &(d->datamutex) );
    }

    //ok, we now either have a result, or an error
    if (not d->error_data.isEmpty())
        SireError::exception::unpackAndThrow(d->error_data);
        
    if (d->interim_worker_data.isEmpty())
        //the job has been aborted, or just didn't get anywhere
        return d->initial_worker_data;
    
    else
        return d->interim_worker_data;
}

/** Return the final result of the work - this blocks until the result
    is available (or until an error occurs!) */
QByteArray MPIPromise::finalData()
{
    if (this->isNull())
        return QByteArray();

    QMutexLocker lkr( &(d->datamutex) );
    
    if (not d->error_data.isEmpty())
        //throw the error!
        SireError::exception::unpackAndThrow(d->error_data);
        
    else if (d->final_worker_data.isEmpty())
    {
        //the job hasn't finished yet
        if (d->node.isNull())
            throw SireError::invalid_state( QObject::tr(
                "There is no interim result as this work associated with this "
                "promise is not running on any nodes!"), CODELOC );
                
        //wait for the result to arrive
        d->result_waiter.wait( &(d->datamutex) );
    }

    //ok, we now either have a result, or an error
    if (not d->error_data.isEmpty())
        SireError::exception::unpackAndThrow(d->error_data);
        
    if (d->final_worker_data.isEmpty())
        //the job must have been aborted, or just didn't
        //get anywhere
        return d->initial_worker_data;
    
    else    
        return d->final_worker_data;
}

/** Return the progress of the calculation */
double MPIPromise::progress()
{
    if (this->isNull())
        return 0;

    QMutexLocker lkr( &(d->datamutex) );
    
    if (d->current_progress >= 100 or d->node.isNull())
        return d->current_progress;
        
    else
    {
        //tell the node to get the progress
        getFrontEnd( *(d->node) ).requestProgress();
        
        //wait until the progress has been obtained
        d->progress_waiter.wait( &(d->datamutex) );
        
        return d->current_progress;
    }
}

/** This function is called by the node to set the progress
    of the calculation */
void MPIPromise::setProgress(double progress)
{
    if (this->isNull())
        return;

    QMutexLocker lkr( &(d->datamutex) );
    d->current_progress = progress;
    d->progress_waiter.wakeAll();
}

/** Set the interim result */
void MPIPromise::_pvt_setInterimData(const QByteArray &worker_data, double progress)
{
    if (this->isNull())
        return;

    QMutexLocker lkr( &(d->datamutex) );
    d->interim_worker_data = worker_data;
    d->current_progress = progress;
    
    d->progress_waiter.wakeAll();
    d->interim_waiter.wakeAll();
}

/** Set the final result */
void MPIPromise::_pvt_setFinalData(const QByteArray &worker_data)
{
    if (this->isNull())
        return;

    QMutexLocker lkr( &(d->datamutex) );
    d->error_data = QByteArray();
    d->interim_worker_data = worker_data;
    d->final_worker_data = worker_data;
    d->current_progress = 100;
    
    //the job has finished - we don't need the node any more
    d->node = MPINodePtr();
    
    d->interim_waiter.wakeAll();
    d->progress_waiter.wakeAll();

    d->result_waiter.wakeAll();
}

/** Oh no - the work has ended in error! */
void MPIPromise::_pvt_setError(const MPIError &error)
{
    if (this->isNull())
        return;

    QMutexLocker lkr( &(d->datamutex) );
    d->interim_worker_data = QByteArray();
    d->final_worker_data = QByteArray();
    
    d->error_data = error.errorData();
    d->current_progress = 0;
    
    //the job has finished - we don't need the node any more
    d->node = MPINodePtr();
    
    d->interim_waiter.wakeAll();
    d->progress_waiter.wakeAll();
    
    d->result_waiter.wakeAll();
}

/** Tell the promise that the job has been stopped, and supply the most
    up to date result (together with the percent progress of that result) */
void MPIPromise::_pvt_setStopped(const QByteArray &worker_data, double progress)
{
    if (this->isNull())
        return;

    QMutexLocker lkr( &(d->datamutex) );

    d->node = MPINodePtr();
    
    d->current_progress = progress;
    d->error_data = QByteArray();
    
    if (d->current_progress == 0)
    {
        d->interim_worker_data = QByteArray();
        d->final_worker_data = QByteArray();
    }
    else
    {
        d->interim_worker_data = worker_data;
        d->final_worker_data = worker_data;
    }
    
    d->progress_waiter.wakeAll();
    d->interim_waiter.wakeAll();

    d->result_waiter.wakeAll();
}

/** Tell the promise that the job has been aborted */
void MPIPromise::setAborted()
{
    if (this->isNull())
        return;

    QMutexLocker lkr( &(d->datamutex) );
    
    d->node = MPINodePtr();
    
    d->current_progress = 0;
    
    d->error_data = QByteArray();
    d->interim_worker_data = QByteArray();
    d->final_worker_data = QByteArray();
    
    d->progress_waiter.wakeAll();
    d->interim_waiter.wakeAll();
    
    d->result_waiter.wakeAll();
}

/** This private function is called by the node to set the final result */
void MPIPromise::setFinalData(const QByteArray &worker_data)
{
    if (this->isNull())
        return;

    if (SireStream::getDataHeader(worker_data).dataType() 
                                == QLatin1String(MPIError::typeName()))
    {
        this->_pvt_setError( SireStream::loadType<MPIError>(worker_data) );
    }
    else
    {
        this->_pvt_setFinalData(worker_data);
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
        this->_pvt_setError( SireStream::loadType<MPIError>(worker_data) );
    }
    else
    {
        this->_pvt_setInterimData(worker_data, progress);
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
        this->_pvt_setError( SireStream::loadType<MPIError>(worker_data) );
    }
    else
    {
        this->_pvt_setStopped(worker_data, progress);
    }
}

