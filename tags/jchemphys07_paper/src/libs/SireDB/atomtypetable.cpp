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

#include "SireMol/qhash_siremol.h"

#include <QDebug>

#include "atomtypetable.h"

#include "SireMol/molecule.h"

#include "SireError/errors.h"
#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireDB;
using namespace SireMol;

/** Register this class so that it can be used with QVariant */
static const RegisterMetaType<AtomTypeTable> r_atomtypetable;

/** Serialise to a binary data stream */
QDataStream SIREDB_EXPORT &operator<<(QDataStream &ds, const AtomTypeTable &table)
{
    writeHeader(ds, r_atomtypetable, 1)
          << static_cast<const AtomTableT<AtomType>&>(table);

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREDB_EXPORT &operator>>(QDataStream &ds, AtomTypeTable &table)
{
    VersionID v = readHeader(ds, r_atomtypetable);

    if (v == 1)
    {
        ds >> static_cast<AtomTableT<AtomType>&>(table);
    }
    else
        throw version_error(v, "1", r_atomtypetable, CODELOC);

    return ds;
}

/** Construct an empty AtomTypeTable */
AtomTypeTable::AtomTypeTable() : AtomTableT<AtomType>()
{}

/** Construct a table to hold the parameters of the passed molecule */
AtomTypeTable::AtomTypeTable(const MoleculeInfo &molinfo)
              : AtomTableT<AtomType>(molinfo)
{}

/** Copy constructor. As this class uses implicitly shared data, this copy will be
    very fast and very memory efficient.  */
AtomTypeTable::AtomTypeTable(const AtomTypeTable &other)
              : AtomTableT<AtomType>(other)
{}

/** Destructor */
AtomTypeTable::~AtomTypeTable()
{}
