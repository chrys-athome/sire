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

#include "using_relationships.hpp"
#include "using_parameters.hpp"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireDB;

static const RegisterMetaType<AssignInstruction> r_instruc(MAGIC_ONLY,
                                                      "SireDB::AssignInstruction");

/** Serialise to a binary data stream */
QDataStream SIREDB_EXPORT &operator<<(QDataStream &ds, const AssignInstruction&)
{
    writeHeader(ds, r_instruc, 0);
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREDB_EXPORT &operator>>(QDataStream &ds, AssignInstruction&)
{
    VersionID v = readHeader(ds, r_instruc);

    if (v != 0)
        throw version_error(v, "0", r_instruc, CODELOC);

    return ds;
}

static const RegisterMetaType<using_database> r_database(MAGIC_ONLY,
                                                  "SireDB::using_database");

/** Serialise to a binary data stream */
QDataStream SIREDB_EXPORT &operator<<(QDataStream &ds, const SireDB::using_database &dbs)
{
    writeHeader(ds, r_database, 1)
            << static_cast<const QStringList&>(dbs)
            << static_cast<const AssignInstruction&>(dbs);

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREDB_EXPORT &operator>>(QDataStream &ds, SireDB::using_database &dbs)
{
    VersionID v = readHeader(ds, r_database);

    if (v == 1)
    {
        ds >> static_cast<QStringList&>(dbs)
           >> static_cast<AssignInstruction&>(dbs);
    }
    else
        throw version_error(v, "1", r_database, CODELOC);

    return ds;
}

static const RegisterMetaType<using_parameters_base> r_params;

/** Serialise to a binary data stream */
QDataStream SIREDB_EXPORT &operator<<(QDataStream &ds,
                                      const SireDB::using_parameters_base &params)
{
    writeHeader(ds, r_params, 1)
          << static_cast<const using_database&>(params);

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREDB_EXPORT &operator>>(QDataStream &ds,
                                      SireDB::using_parameters_base &params)
{
    VersionID v = readHeader(ds, r_params);

    if (v == 1)
    {
        ds >> static_cast<using_database&>(params);
    }
    else
        throw version_error(v, "1", r_params, CODELOC);

    return ds;

}

static const RegisterMetaType<using_relationships_base> r_relate;

/** Serialise to a binary data stream */
QDataStream SIREDB_EXPORT &operator<<(QDataStream &ds,
                                      const SireDB::using_relationships_base &relate)
{
    writeHeader(ds, r_relate, 1)
          << static_cast<const using_database&>(relate);

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREDB_EXPORT &operator>>(QDataStream &ds,
                                      SireDB::using_relationships_base &relate)
{
    VersionID v = readHeader(ds, r_relate);

    if (v == 1)
    {
        ds >> static_cast<using_database&>(relate);
    }
    else
        throw version_error(v, "1", r_relate, CODELOC);

    return ds;
}
