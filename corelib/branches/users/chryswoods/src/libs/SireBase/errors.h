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

#ifndef SIREBASE_ERRORS_H
#define SIREBASE_ERRORS_H

#include "Siren/exception.h"

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireBase
{

/** This exception is thrown when a request is made of a non-existant property */
class SIREBASE_EXPORT missing_property
        : public Siren::ImplementsException<missing_property,Siren::exception>
{
public:
    missing_property();

    missing_property(QString err, QString place = QString::null);

    missing_property(const missing_property &other);

    ~missing_property() throw();
};

/** This exception is thrown when a request is made to duplicate a
    property when this would be inappropriate */
class SIREBASE_EXPORT duplicate_property
        : public Siren::ImplementsException<duplicate_property,Siren::exception>
{
public:
    duplicate_property();

    duplicate_property(QString err, QString place = QString::null);

    duplicate_property(const duplicate_property &other);

    ~duplicate_property() throw();
};

}

Q_DECLARE_METATYPE(SireBase::missing_property)
Q_DECLARE_METATYPE(SireBase::duplicate_property)

SIRE_END_HEADER

#endif
