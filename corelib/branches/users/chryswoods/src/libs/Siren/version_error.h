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

#include "Siren/exception.h"

#ifndef SIREN_VERSION_ERROR_H
#define SIREN_VERSION_ERROR_H

SIREN_BEGIN_HEADER

namespace Siren
{

/** This exception is thrown whenever there is an error with the version number of
    the binary data streaming protocol.

    @author Christopher Woods
*/
class SIREN_EXPORT version_error : public ImplementsException<version_error,exception>
{
public:
    version_error();

    version_error(const QString &error, const QString place=QString::null);

    version_error(VERSION_ID wrongid, 
                  const detail::RegisterMetaTypeBase &info,
                  QString place=QString::null);

    version_error(VERSION_ID wrongid, QString supported_ids,
                  const Class &c, QString place=QString::null);

    version_error(VERSION_ID wrongid, 
                  const Class &c, QString place=QString::null);

    version_error(VERSION_ID wrongid, QString supported_ids,
                  const detail::RegisterMetaTypeBase &info,
                  QString place=QString::null);

    version_error(const version_error &other);
    
    ~version_error() throw();
};

}

Q_DECLARE_METATYPE(Siren::version_error)

SIREN_END_HEADER

#endif
