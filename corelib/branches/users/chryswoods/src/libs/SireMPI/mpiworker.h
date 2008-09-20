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
class MPIError;
}

QDataStream& operator<<(QDataStream&, const SireMPI::MPIError&);
QDataStream& operator>>(QDataStream&, SireMPI::MPIError&);

namespace SireError
{
class exception;
}

namespace SireMPI
{

/** This is the base class of all of the workers that are used
    to actually perform work on backend MPI nodes. This
    class is explicitly shared and thread-safe
    
    @author Christopher Woods
*/
class SIREMPI_EXPORT MPIWorker
{
public:
    virtual ~MPIWorker();

    static const char* typeName()
    {
        return "SireMPI::MPIWorker";
    }

    virtual const char* what() const=0;
    
    virtual MPIWorker* clone() const=0;

    /** This function is called on the node to perform the work.
        It blocks until the work is complete */
    virtual void run()=0;

    /** Call this function to abort the work. This blocks until 
        the work has stopped */
    virtual void abort()=0;
    
    /** Call this function to stop the work. This blocks until 
        the work has stopped */
    virtual void stop()=0;

    /** Return the progress as a percentage */
    virtual double progress()=0;

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
    
    void run();
    void stop();
    void abort();
    double progress();

    const QByteArray& errorData() const;

private:
    /** This is a binary representation of the error */
    QByteArray error_data;
};

}

Q_DECLARE_METATYPE( SireMPI::MPIError );

SIRE_END_HEADER

#endif
