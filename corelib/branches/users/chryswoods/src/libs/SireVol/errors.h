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

#ifndef SIREVOL_ERRORS_H
#define SIREVOL_ERRORS_H

#include "Siren/exception.h"

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireVol
{

/** This exception is thrown when an attempt is made
    to interface or switch between incompatible spaces

    @author Christopher Woods
*/
class SIREVOL_EXPORT incompatible_space
        : public Siren::ImplementsException<incompatible_space,Siren::exception>
{
public:
    incompatible_space();

    incompatible_space(QString err, QString place = QString::null);

    incompatible_space(const incompatible_space &other);

    ~incompatible_space() throw();
};

}

Q_DECLARE_METATYPE(SireVol::incompatible_space)

SIRE_END_HEADER

#endif
