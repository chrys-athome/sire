/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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

#ifndef SIREDB_ERRORS_H
#define SIREDB_ERRORS_H

#include "SireError/exception.h"

SIRE_BEGIN_HEADER

/** This file contains the exceptions that can be thrown by the SireDB library.

    @author Christopher Woods
*/

namespace SireDB
{

/** This is the base class of all SireMM errors */
class SIREDB_EXPORT siredb_error : public SireError::exception
{
public:
    siredb_error() : exception()
    {}
    
    siredb_error(QString err, QString place = QString::null) : exception(err,place)
    {}
    
    siredb_error(const siredb_error &other) : exception(other)
    {}
    
    ~siredb_error() throw()
    {}
    
    const char* what() const throw()
    {
        return "SireIO::siredb_error";
    }
};

/** This exception is thrown when a non-recoverable error occurs
    while reading or writing from a database

    @author Christopher Woods
*/
class SIREDB_EXPORT database_error : public siredb_error
{
public:
    database_error() : siredb_error()
    {}
    
    database_error(QString err, QString place = QString::null) 
              : siredb_error(err,place)
    {}
    
    database_error(const database_error &other) : siredb_error(other)
    {}
    
    ~database_error() throw()
    {}
    
    const char* what() const throw()
    {
        return "SireDB::database_error";
    }
};

}

Q_DECLARE_METATYPE(SireDB::database_error)

SIRE_END_HEADER

#endif
