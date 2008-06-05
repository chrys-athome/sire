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

#ifndef SIRECLUSTER_ERRORS_H
#define SIRECLUSTER_ERRORS_H

#include "SireError/exception.h"

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireCluster
{

/** This is the base class of all SireCluster errors */
class SIRECAS_EXPORT sirecluster_error : public SireError::exception
{
public:
    sirecluster_error() : exception()
    {}

    sirecluster_error(QString err, QString place = QString::null)
                  : exception(err,place)
    {}

    sirecluster_error(const sirecluster_error &other) : exception(other)
    {}

    ~sirecluster_error() throw()
    {}

    const char* what() const throw()
    {
        return "SireCluster::sirecluster_error";
    }
};

/** This class is thrown when an attempt is made to do something
    that is not valid with an already active processor. */
class already_active : public sirecluster_error
{
public:
    already_active() : sirecluster_error()
    {}

    already_active(QString err, QString place = QString::null)
              : sirecluster_error(err,place)
    {}

    already_active(const already_active &other) : sirecluster_error(other)
    {}

    ~already_active() throw()
    {}

    const char* what() const throw()
    {
        return "SireCluster::already_active";
    }
};

}

Q_DECLARE_METATYPE(SireCluster::sirecluster_error)
Q_DECLARE_METATYPE(SireCluster::already_active)

SIRE_END_HEADER

#endif
