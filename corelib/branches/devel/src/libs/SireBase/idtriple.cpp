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

#include "idtriple.h"

#include "SireError/errors.h"
#include "SireStream/datastream.h"

using namespace SireBase;
using namespace SireStream;

static const RegisterMetaType<IDTriple> r_idtriple;

/** Serialise to a binary datastream */
QDataStream SIREBASE_EXPORT &operator<<(QDataStream &ds, const IDTriple &idtriple)
{
    writeHeader(ds, r_idtriple, 1)
        << idtriple.idnum << idtriple.versn;

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREBASE_EXPORT &operator>>(QDataStream &ds, IDTriple &idtriple)
{
    VersionID v = readHeader(ds, r_idtriple);

    if (v == 1)
    {
        ds >> idtriple.idnum >> idtriple.versn;
    }
    else
        throw version_error(v, "1", r_idtriple, CODELOC);

    return ds;
}

/** Private static incremint that is used when no
    other one is supplied by the user */
Incremint IDTriple::shared_triple_incremint;

/** Constructor - optionally provide the Incremint that is used
    to get the ID version number */
IDTriple::IDTriple(Incremint *idint)
         : id_incremint(idint)
{
    if (not idint)
        throw SireError::program_bug( QObject::tr(
                  "You *must* supply a valid Incremint to the "
                  "IDTriple constructor!"), CODELOC );

    this->incrementID();
}

/** Copy constructor */
IDTriple::IDTriple(const IDTriple &other)
              : major_incremint(other.major_incremint),
                id_incremint(other.id_incremint),
                idnum(other.idnum), versn(other.versn)
{}

/** Destructor */
IDTriple::~IDTriple()
{}

/** Assignment operator */
IDTriple& IDTriple::operator=(const IDTriple &other)
{
    major_incremint = other.major_incremint;
    id_incremint = other.id_incremint;
    idnum = other.idnum;
    versn = other.versn;

    return *this;
}
