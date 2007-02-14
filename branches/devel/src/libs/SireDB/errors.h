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

namespace SireDB
{

/** This is the base class of all SireDB errors */
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
        return "SireDB::siredb_error";
    }
};

/** This exception is thrown whenever there is an database error */
class SIREDB_EXPORT db_error : public SireDB::siredb_error
{
public:
    db_error() : siredb_error()
    {}

    db_error(QString err, QString place=QString::null) 
              : siredb_error(err,place)
    {}
    
    db_error(const db_error &other) : siredb_error(other)
    {}
    
    ~db_error() throw()
    {}
    
    const char* what() const throw()
    {
        return "SireDB::db_error";
    }
};

/** This exception is thrown whenever there is a problem with a Match object (selection) */
class SIREDB_EXPORT invalid_match : public SireDB::siredb_error
{
public:
    invalid_match() : siredb_error()
    {}

    invalid_match(QString err, QString place=QString::null) 
              : siredb_error(err,place)
    {}
    
    invalid_match(const invalid_match &other) : siredb_error(other)
    {}
    
    ~invalid_match() throw()
    {}
    
    const char* what() const throw()
    {
        return "SireDB::invalid_match";
    }
};

/** This exception is thrown whenever an attempt is made to retrieve a parameter 
    that has not been assigned. */
class SIREDB_EXPORT missing_parameter : public SireDB::siredb_error
{
public:
    missing_parameter() : siredb_error()
    {}

    missing_parameter(QString err, QString place=QString::null) 
              : siredb_error(err,place)
    {}
    
    missing_parameter(const missing_parameter &other) : siredb_error(other)
    {}
    
    ~missing_parameter() throw()
    {}
    
    const char* what() const throw()
    {
        return "SireDB::missing_parameter";
    }
};


}

Q_DECLARE_METATYPE(SireDB::siredb_error)
Q_DECLARE_METATYPE(SireDB::db_error)
Q_DECLARE_METATYPE(SireDB::invalid_match)
Q_DECLARE_METATYPE(SireDB::missing_parameter)

SIRE_END_HEADER

#endif
