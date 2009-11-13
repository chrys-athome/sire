/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2009  Christopher Woods
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

#ifndef SIREN_MAGIC_ERROR_H
#define SIREN_MAGIC_ERROR_H

#include <QObject>

#include "Siren/exception.h"

SIREN_BEGIN_HEADER

namespace Siren
{

/** This exception is thrown whenever there is an error with the magic number of
    the binary data streaming protocol.

    @author Christopher Woods
*/
class SIREN_EXPORT magic_error : public ConcreteException<magic_error,exception>
{
public:
    magic_error();
    magic_error(QString err, QString place = QString::null);

    magic_error(CLASS_UID wrongid, const detail::RegisterMetaTypeBase &info,
                QString place=QString::null);
                
    magic_error(const magic_error &other);

    ~magic_error() throw();
};

}

Q_DECLARE_METATYPE(Siren::magic_error);

SIREN_END_HEADER

#endif
