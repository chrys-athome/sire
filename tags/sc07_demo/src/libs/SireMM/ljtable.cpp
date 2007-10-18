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

#include "ljtable.h"

#include "SireMol/molecule.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMM;
using namespace SireDB;
using namespace SireMol;

/** Register this as a concrete class */
static const RegisterMetaType<LJTable> r_ljtable;

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const LJTable &table)
{
    writeHeader(ds, r_ljtable, 1)
            << static_cast<const AtomTableT<LJParameter>&>(table);

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, LJTable &table)
{
    VersionID v = readHeader(ds, r_ljtable);

    if (v == 1)
    {
        ds >> static_cast<AtomTableT<LJParameter>&>(table);
    }
    else
        throw version_error(v, "1", r_ljtable, CODELOC);

    return ds;
}

/** Null Constructor */
LJTable::LJTable() : AtomTableT<LJParameter>()
{}

/** Construct a table to hold the ljs for the Molecule
    described by 'molinfo' */
LJTable::LJTable(const MoleculeInfo &molinfo)
            : AtomTableT<LJParameter>(molinfo)
{}

/** Copy constructor - this is fast as implicit sharing is used */
LJTable::LJTable(const LJTable &other)
            : AtomTableT<LJParameter>(other)
{}

/** Destructor */
LJTable::~LJTable()
{}
