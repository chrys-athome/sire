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

#include "detail/mpidetail.h"   // CONDITIONAL_INCLUDE

#include "SireError/errors.h"

using namespace SireMove;
using namespace SireMove::detail;

////////
//////// Implementation of MPIPromiseBase
////////

/** Null constructor */
MPIPromiseBase::MPIPromiseBase()
{}

/** Copy constructor */
MPIPromiseBase::MPIPromiseBase(const MPIPromiseBase &other)
               : worker_ptr(other.worker_ptr)
{}

/** Private constructor used to pass the MPIWorker that is doing the 
    work to this promise */
MPIPromiseBase::MPIPromiseBase(const boost::shared_ptr<detail::MPIWorker> &worker)
               : worker_ptr(worker)
{}

/** Destructor */
MPIPromiseBase::~MPIPromiseBase()
{}

/** Is this is a null promise? */
bool MPIPromiseBase::isNull() const
{
    return worker_ptr.get() == 0;
}

/** Abort the process */
void MPIPromiseBase::abort()
{
    if (worker_ptr)
        worker_ptr->abort();
}

/** Stop the job */
void MPIPromiseBase::stop()
{
    if (worker_ptr)
        worker_ptr->stop();
}

/** Return the progress of the job (as a percentage) */
double MPIPromiseBase::progress()
{
    if (worker_ptr)
        return worker_ptr->progress();
    else
        return 0;
}

/** Return whether or not the job has finished */
bool MPIPromiseBase::hasFinished()
{
    if (worker_ptr)
        return worker_ptr->hasFinished();
    else
        return false;
}

/** Return whether or not an error occurred during the running
    of the job */
bool MPIPromiseBase::isError()
{
    if (worker_ptr)
        return worker_ptr->isError();
    else
        return false;
}

/** Return whether or not the job was aborted */
bool MPIPromiseBase::isAborted()
{
    if (worker_ptr)
        return worker_ptr->isAborted();
    else
        return false;
}

/** Throw an error that represents an aborted job */
void MPIPromiseBase::throwJobAbortedError()
{
    throw SireError::invalid_state( QObject::tr(
        "You cannot read the result of an aborted job!"), CODELOC );
}


/** Wait until the job completes */
void MPIPromiseBase::wait()
{
    if (not worker_ptr)
        throw SireError::nullptr_error( QObject::tr(
            "You cannot wait on a null promise!"), CODELOC );
            
    worker_ptr->wait();
}

/** Wait until the job completes, or until 'ms' milliseconds have passed.
    This returns whether or not the job has completed */
bool MPIPromiseBase::wait(int ms)
{
    if (not worker_ptr)
        throw SireError::nullptr_error( QObject::tr(
            "You cannot wait on a null promise!"), CODELOC );
            
    return worker_ptr->wait(ms);
}

/** Return the raw data for the result - this blocks until the work
    has finished, an error occured, or the work is aborted  */
QByteArray MPIPromiseBase::resultData()
{
    if (not worker_ptr)
        throw SireError::nullptr_error( QObject::tr(
            "You cannot get the result of a null promise!"), CODELOC );
    
    return worker_ptr->result();
}

/** Return the interim raw data for the result. This block until some
    result is available, or an error occurred, or the work is aborted */
QByteArray MPIPromiseBase::interimResultData()
{
    if (not worker_ptr)
        throw SireError::nullptr_error( QObject::tr(
            "You cannot get the interim result of a null promise!"), CODELOC );
    
    return worker_ptr->interimResult();
}
