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

#ifndef SIREMATHS_ERRORS_H
#define SIREMATHS_ERRORS_H

#include "Siren/exception.h"

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireMaths
{

/** This class represents a general maths error */
class SIREMATHS_EXPORT math_error 
        : public Siren::ImplementsException<math_error,Siren::exception>
{
public:
    math_error();

    math_error(QString err, QString place = QString::null);

    math_error(const math_error &other);

    ~math_error() throw();
};

/** This class represents a domain error */
class SIREMATHS_EXPORT domain_error 
        : public Siren::ImplementsException<domain_error,Siren::exception>
{
public:
    domain_error();

    domain_error(QString err, QString place = QString::null);

    domain_error(const domain_error &other);

    ~domain_error() throw();
};

}

Q_DECLARE_METATYPE(SireMaths::math_error)
Q_DECLARE_METATYPE(SireMaths::domain_error)

SIRE_END_HEADER

#endif
