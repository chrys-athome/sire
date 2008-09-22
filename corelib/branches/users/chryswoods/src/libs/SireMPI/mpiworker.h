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

#ifndef SIREMPI_MPIWORKER_H
#define SIREMPI_MPIWORKER_H

#include <QUuid>
#include <QMutex>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireMPI
{
class MPIWorker;
class MPIError;
}

QDataStream& operator<<(QDataStream&, const SireMPI::MPIWorker&);
QDataStream& operator>>(QDataStream&, SireMPI::MPIWorker&);

QDataStream& operator<<(QDataStream&, const SireMPI::MPIError&);
QDataStream& operator>>(QDataStream&, SireMPI::MPIError&);

namespace SireError
{
class exception;
}

namespace SireMPI
{

/** This is the base class of all workers. A worker is class that
    can perform a job in the current thread in chunks. All of the 
    input and output for the job is stored in the worker. This
    allows the worker to be moved between threads or across
    nodes, and for the work to be checkpointed.
        
    @author Christopher Woods
*/
class SIREMPI_EXPORT MPIWorker
{

friend QDataStream& ::operator<<(QDataStream&, const MPIWorker&);
friend QDataStream& ::operator>>(QDataStream&, MPIWorker&);

public:
    virtual ~MPIWorker();

    static const char* typeName()
    {
        return "SireMPI::MPIWorker";
    }

    virtual const char* what() const=0;
    
    virtual MPIWorker* clone() const=0;

    /** Perform a chunk of the work in the current thread */
    virtual void runChunk()=0;

    /** Is the work finished? */
    virtual bool hasFinished() const=0;

    double progress() const;

    template<class T>
    bool isA() const
    {
        return dynamic_cast<const T*>(this) != 0;
    }
    
    template<class T>
    const T& asA() const
    {
        return dynamic_cast<const T&>(*this);
    }
    
    template<class T>
    T& asA()
    {
        return dynamic_cast<T&>(*this);
    }

protected:
    MPIWorker();
    
    MPIWorker(const MPIWorker &other);

    MPIWorker& operator=(const MPIWorker &other);
    
    void setProgress(double progress);

private:
    /** The current progress of the work */
    double current_progress;
};

/** This MPIWorker is sent back when an has occured. It just contains
    the error which will be thrown when the result is obtained 
    
    @author Christopher Woods
*/
class SIREMPI_EXPORT MPIError : public MPIWorker
{

friend QDataStream& ::operator<<(QDataStream&, const MPIError&);
friend QDataStream& ::operator>>(QDataStream&, MPIError&);

public:
    MPIError();
    MPIError(const std::exception &e);
    MPIError(const SireError::exception &e);

    MPIError(const MPIError &other);
    
    ~MPIError();
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<MPIError>() );
    }
    
    const char* what() const
    {
        return MPIError::typeName();
    }
    
    MPIError* clone() const
    {
        return new MPIError(*this);
    }
    
    void runChunk();

    bool hasFinished() const;

    const QByteArray& errorData() const;

private:
    /** This is a binary representation of the error */
    QByteArray error_data;
};

}

Q_DECLARE_METATYPE( SireMPI::MPIError );

SIRE_EXPOSE_CLASS( SireMPI::MPIWorker )
SIRE_EXPOSE_CLASS( SireMPI::MPIError )

SIRE_END_HEADER

#endif
