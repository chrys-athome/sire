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

#include "tablebase.h"

#include <QDebug>

#include "SireMol/molecule.h"
#include "SireMol/atomindex.h"

#include "SireError/errors.h"
#include "SireStream/datastream.h"

using namespace SireMol;
using namespace SireStream;
using namespace SireDB;

//Register TableBase as a pure virtual class
static const RegisterMetaType<TableBase> r_tablebase(MAGIC_ONLY, "SireDB::TableBase");

/** Serialise to a binary data stream */
QDataStream SIREDB_EXPORT &operator<<(QDataStream &ds, const SireDB::TableBase &table)
{
    writeHeader(ds, r_tablebase, 1) << table.molinfo;

    return ds;
}

QDataStream SIREDB_EXPORT &operator>>(QDataStream &ds, SireDB::TableBase &table)
{
    VersionID v = readHeader(ds, r_tablebase);

    if (v == 1)
    {
        ds >> table.molinfo;
    }
    else
        throw version_error(v, "1", r_tablebase, CODELOC);

    return ds;
}

/** Constructor */
TableBase::TableBase() : QSharedData()
{}

/** Construct a table to hold the parameters for the molecule
    described by 'moleculeinfo' */
TableBase::TableBase(const MoleculeInfo &moleculeinfo)
          : QSharedData(), molinfo(moleculeinfo)
{}

/** Copy constructor */
TableBase::TableBase(const TableBase &other)
          : QSharedData(), molinfo(other.molinfo)
{}

/** Destructor */
TableBase::~TableBase()
{}

/** Return whether or not this table is compatible with the molecule info
    'molinfo' (in other words, molinfo == this->info()) */
bool TableBase::isCompatibleWith(const MoleculeInfo &molinfo) const
{
    return molinfo == this->info();
}

/** Assert that this table is compatible with the molecule 'molecule'

    \throw SireError::incompatible_error
*/
void TableBase::assertCompatibleWith(const Molecule &molecule) const
{
    if ( not this->isCompatibleWith(molecule.info()) )
        throw SireError::incompatible_error( QObject::tr(
              "This table, which holds the parameters for the molecule \"%1\", "
              "is not compatible with the molecule \"%2\".")
                  .arg(molinfo.name(), molecule.name()), CODELOC );
}
