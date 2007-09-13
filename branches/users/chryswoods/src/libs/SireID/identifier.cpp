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

#include "identifier.h"

#include "SireError/errors.h"

#include "SireStream/datastream.h"

using namespace SireID;
using namespace SireID::detail;

using namespace SireStream;

void Identifier_T_Base::throwNullIDError() const
{
    throw SireError::nullptr_error( QObject::tr(
            "Cannot query a null Identifier!"), CODELOC );
}

void Identifier_T_Base::throwVersionError(VersionID v, 
                                          const QString &supported_versions,
                                          const RegisterMetaTypeBase &r_type) const
{
    throw SireError::version_error(v, vers, r_type, CODELOC);
}

static const RegisterMetaType<Identifier> r_id;

/** Serialise to a binary datastream */
QDataStream SIREID_EXPORT &operator<<(QDataStream &ds, const Identifier &id)
{
    return id.save(ds, r_id);
}

/** Deserialise from a binary datastream */
QDataStream SIREID_EXPORT &operator>>(QDataStream &ds, Identifier &id)
{
    return id.load(ds, r_id);
}

/** Return the hash of this ID */
uint SIREID_EXPORT qHash(const Identifier &id)
{
    return id.hash();
}
