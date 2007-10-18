/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007  Christopher Woods
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

#ifndef SIRESYSTEM_ERRORS_H
#define SIRESYSTEM_ERRORS_H

#include "SireError/exception.h"

SIRE_BEGIN_HEADER

/**
This file contains the exceptions that can be thrown by the SireSystem library.

@author Christopher Woods
*/

namespace SireSystem
{

/** This is the base class of all SireSystem errors */
class SIRESYSTEM_EXPORT siresystem_error : public SireError::exception
{
public:
    siresystem_error() : exception()
    {}

    siresystem_error(QString err, QString place = QString::null) : exception(err,place)
    {}

    siresystem_error(const siresystem_error &other) : exception(other)
    {}

    ~siresystem_error() throw()
    {}

    const char* what() const throw()
    {
        return "SireSystem::siresystem_error";
    }
};


/** This exception is thrown when a request is made of
    a non-existant monitor

    @author Christopher Woods
*/
class SIREMOL_EXPORT missing_monitor : public siresystem_error
{
public:
    missing_monitor() : siresystem_error()
    {}

    missing_monitor(QString err, QString place = QString::null)
              : siresystem_error(err,place)
    {}

    missing_monitor(const missing_monitor &other) : siresystem_error(other)
    {}

    ~missing_monitor() throw()
    {}

    const char* what() const throw()
    {
        return "SireSystem::missing_monitor";
    }
};

}

Q_DECLARE_METATYPE(SireSystem::siresystem_error)
Q_DECLARE_METATYPE(SireSystem::missing_monitor)

SIRE_END_HEADER

#endif
