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

#include "mpiworker.h"

#include "SireStream/datastream.h"
#include "SireStream/streamdata.hpp"

#include "SireError/errors.h"

using namespace SireMPI;
using namespace SireStream;

////////////
//////////// Implementation of MPIWorker
////////////

static const RegisterMetaType<MPIWorker> r_mpiworker( MAGIC_ONLY,
                                                      "SireMPI::MPIWorker" );

/** Serialise to a binary datastream */
QDataStream SIREMPI_EXPORT &operator<<(QDataStream &ds, const MPIWorker &worker)
{
    writeHeader(ds, r_mpiworker, 1);
    
    ds << worker.current_progress;
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMPI_EXPORT &operator>>(QDataStream &ds, MPIWorker &worker)
{
    VersionID v = readHeader(ds, r_mpiworker);
    
    if (v == 1)
    {
        ds >> worker.current_progress;
    }
    else
        throw version_error( v, "1", r_mpiworker, CODELOC );
        
    return ds;
}

/** Constructor */
MPIWorker::MPIWorker() : current_progress(0)
{}

/** Copy constructor */
MPIWorker::MPIWorker(const MPIWorker &other)
          : current_progress(other.current_progress)
{}

/** Destructor */
MPIWorker::~MPIWorker()
{}

/** Copy assignment operator */
MPIWorker& MPIWorker::operator=(const MPIWorker &other)
{
    if (this != &other)
    {
        current_progress = other.current_progress;
    }
    
    return *this;
}

/** Return the current progress of the job */
double MPIWorker::progress() const
{
    return current_progress;
}

////////////
//////////// Implementation of MPIError
////////////

static const RegisterMetaType<MPIError> r_mpierror;

/** Serialise to a binary datastream */
QDataStream SIREMPI_EXPORT &operator<<(QDataStream &ds, const MPIError &mpierror)
{
    writeHeader(ds, r_mpierror, 1);

    ds << mpierror.error_data
       << static_cast<const MPIWorker&>(mpierror);

    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMPI_EXPORT &operator>>(QDataStream &ds, MPIError &mpierror)
{
    VersionID v = readHeader(ds, r_mpierror);
    
    if (v == 1)
    {
        ds >> mpierror.error_data
           >> static_cast<MPIWorker&>(mpierror);
    }
    else
        throw version_error(v, "1", r_mpierror, CODELOC);
        
    return ds;
}

/** Null constructor */
MPIError::MPIError() : MPIWorker()
{}

/** Construct from the passed standard exception */
MPIError::MPIError(const std::exception &e) : MPIWorker()
{
    error_data = SireStream::save( SireError::std_exception(e) );
}

/** Construct for the passed Sire exception */
MPIError::MPIError(const SireError::exception &e) : MPIWorker()
{
    error_data = SireStream::save(e);
}

/** Copy constructor */
MPIError::MPIError(const MPIError &other) : MPIWorker(other),
                                            error_data(other.error_data)
{}

/** Destructor */
MPIError::~MPIError()
{}

/** Does nothing */
void MPIError::runChunk()
{}

/** Errors are complete! */
bool MPIError::hasFinished() const
{
    return true;
}

/** Return the data that represents this error */
const QByteArray& MPIError::errorData() const
{
    return error_data;
}
