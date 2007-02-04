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

#include "majversion.h"

#include "SireError/errors.h"

#include "SireStream/datastream.h"

using namespace SireBase;
using namespace SireStream;

static const RegisterMetaType<MajVersion> r_majver;

/** Serialise to a binary data stream */
QDataStream SIREBASE_EXPORT &operator<<(QDataStream &ds, const MajVersion &majver)
{
    writeHeader(ds, r_majver, 1) << majver.vers;
    
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREBASE_EXPORT &operator>>(QDataStream &ds, MajVersion &majver)
{
    VersionID v = readHeader(ds, r_majver);
    
    if (v == 1)
    {
        ds >> majver.vers;
    }
    else
        throw version_error(v, "1", r_majver, CODELOC);
    
    return ds;
}

Incremint MajVersion::shared_id_incremint;

MajVersion::MajVersion(Incremint *i)
           : incremint(i)
{
    if (!incremint)
        throw SireError::program_bug( QObject::tr(
                "You cannot create a MajVersion with a null Incremint pointer!"), 
                        CODELOC );

    vers = incremint->increment();
}

MajVersion::MajVersion(const MajVersion &other)
           : incremint(other.incremint), vers(other.vers)
{}

MajVersion::~MajVersion()
{}

void MajVersion::increment()
{
    vers = incremint->increment();
}
