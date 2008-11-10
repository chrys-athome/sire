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

#ifndef SIREMPI_MPIPROMISE_H
#define SIREMPI_MPIPROMISE_H

#include "mpinode.h"

#include "SireStream/streamdata.hpp"

#include <boost/shared_ptr.hpp>

SIRE_BEGIN_HEADER

namespace SireMPI
{

class MPIWorker;
class MPINode;
class MPIError;

namespace detail
{
class MPINodePvt;
}

/** This class provides a handle to a result of a calculation
    that is being performed on another node that communicates
    to this node via MPI. This handle provides the functions
    that allow you to wait for the result, query it before 
    it arrives, test for an error, get intermediate results,
    abort the job and, of course, get the final answer!
    
    @author Christopher Woods
*/
class SIREMPI_EXPORT MPIPromise
{

friend class MPIFrontEnd;
friend class MPINode;

public:
    MPIPromise();
    
    MPIPromise(const MPIPromise &other);
    
    ~MPIPromise();

    MPIPromise& operator=(const MPIPromise &other);
    
    bool operator==(const MPIPromise &other) const;
    bool operator!=(const MPIPromise &other) const;

    MPINode node();

    void abort();
    void stop();
    
    void wait();
    bool wait(int ms);
    
    bool isNull() const;
    
    bool isRunning();
    
    bool isError();
    
    bool isStopped();
    bool isAborted();

    double progress();

    /** Return the initial state (the input work). This can be used
        to restore the state if there was an error or if the job
        was aborted 
    */
    template<class T>
    T input()
    {
        #ifndef SIRE_SKIP_INLINE_FUNCTIONS
        return SireStream::loadType<T>( this->initialData() );
        #endif
    }

    /** Return an interim result - this blocks until an interim result
        is available (or until an error occurs! - in which case the exception
        representing this error is thrown). If the job is aborted, then the
        original input is returned, or if the job is stopped, then the 
        last state of the job before being stopped is returned.
    */
    template<class T>
    T interimResult()
    {
        #ifndef SIRE_SKIP_INLINE_FUNCTIONS
        return SireStream::loadType<T>( this->interimData() );
        #endif
    }
    
    /** Return the final result of the work - this blocks until the result
        is available (or until an error occurs! - in which case the exception
        representing this error is thrown). If the job is aborted, then the
        original input is returned, or if the job is stopped, then the 
        last state of the job before being stopped is returned.
    */
    template<class T>
    T result()
    {
        #ifndef SIRE_SKIP_INLINE_FUNCTIONS
        return SireStream::loadType<T>( this->finalData() );
        #endif
    }

    /** Return the final result of the work - this blocks until the result
        is available (or until an error occurs! - in which case the exception
        representing this error is thrown)
    */
    template<class T>
    T operator()()
    {
        #ifndef SIRE_SKIP_INLINE_FUNCTIONS
        return this->result<T>();
        #endif
    }

protected:
    MPIPromise(const MPIWorker &worker, const MPINode &node);

    void setProgress(double progress); // called by MPIFrontEnd

    void setFinalData(const QByteArray &worker_data); // called by MPIFrontEnd
    void setInterimData(const QByteArray &worker_data, 
                        double progress); // called by MPIFrontEnd
                         

    void setStopped(const QByteArray &worker_data, 
                    double progress); // called by MPIFrontEnd

    void setAborted(); // called by MPIFrontEnd

    QByteArray finalData();
    QByteArray interimData();
    QByteArray initialData();

private:
    void _pvt_setFinalData(const QByteArray &worker_data);
    void _pvt_setInterimData(const QByteArray &worker_data, double progress);
    void _pvt_setStopped(const QByteArray &worker_data, double progress);
    void _pvt_setError(const MPIError &error);

    /** PIMPL pointer for this object */
    boost::shared_ptr<detail::MPIPromisePvt> d;
};

}

SIRE_EXPOSE_CLASS( SireMPI::MPIPromise )

SIRE_END_HEADER

#endif
