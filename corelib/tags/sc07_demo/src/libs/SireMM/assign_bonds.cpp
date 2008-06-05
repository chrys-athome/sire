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
#include "SireMM/qhash_siremm.h"

#include "assign_bonds.h"

#include "SireStream/datastream.h"

using namespace SireMM;
using namespace SireDB;
using namespace SireMol;
using namespace SireStream;

static const RegisterMetaType<assign_bonds> r_assign_bonds;

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const assign_bonds &bonds)
{
    writeHeader(ds, r_assign_bonds, 0)
        << static_cast<const assign_internals<MolBondInfo>&>(bonds);

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, assign_bonds &bonds)
{
    VersionID v = readHeader(ds, r_assign_bonds);

    if (v == 0)
    {
        ds >> static_cast<assign_internals<MolBondInfo>&>(bonds);
    }
    else
        throw version_error(v, "1", r_assign_bonds, CODELOC);

    return ds;
}

/** Constructor */
assign_bonds::assign_bonds(const BondGeneratorBase &generator)
             : assign_internals<MolBondInfo>(generator)
{}

/** Assign the bonds using the requirements specified */
assign_bonds::assign_bonds(const using_database &db0,
                           const BondGeneratorBase &generator)
             : assign_internals<MolBondInfo>(generator)
{
    addDataBase(db0);
}

/** Assign the bonds using the requirements specified */
assign_bonds::assign_bonds(const using_database &db0, const using_database &db1,
                           const BondGeneratorBase &generator)
             : assign_internals<MolBondInfo>(generator)
{
    addDataBase(db0);
    addDataBase(db1);
}

/** Assign the bonds using the requirements specified */
assign_bonds::assign_bonds(const using_database &db0, const using_database &db1,
                           const using_database &db2,
                           const BondGeneratorBase &generator)
             : assign_internals<MolBondInfo>(generator)
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
}

/** Assign the bonds using the requirements specified */
assign_bonds::assign_bonds(const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3,
                           const BondGeneratorBase &generator)
             : assign_internals<MolBondInfo>(generator)
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
    addDataBase(db3);
}

/** Assign the bonds using the requirements specified */
assign_bonds::assign_bonds(const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3,
                           const using_database &db4,
                           const BondGeneratorBase &generator)
             : assign_internals<MolBondInfo>(generator)
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
    addDataBase(db3);
    addDataBase(db4);
}

/** Assign the bonds using the requirements specified */
assign_bonds::assign_bonds(const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3,
                           const using_database &db4, const using_database &db5,
                           const BondGeneratorBase &generator)
             : assign_internals<MolBondInfo>(generator)
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
    addDataBase(db3);
    addDataBase(db4);
    addDataBase(db5);
}

/** Assign the bonds using the requirements specified */
assign_bonds::assign_bonds(const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3,
                           const using_database &db4, const using_database &db5,
                           const using_database &db6,
                           const BondGeneratorBase &generator)
             : assign_internals<MolBondInfo>(generator)
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
    addDataBase(db3);
    addDataBase(db4);
    addDataBase(db5);
    addDataBase(db6);
}

/** Assign the bonds using the requirements specified */
assign_bonds::assign_bonds(const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3,
                           const using_database &db4, const using_database &db5,
                           const using_database &db6, const using_database &db7,
                           const BondGeneratorBase &generator)
             : assign_internals<MolBondInfo>(generator)
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
    addDataBase(db3);
    addDataBase(db4);
    addDataBase(db5);
    addDataBase(db6);
    addDataBase(db7);
}

/** Assign the bonds using the requirements specified */
assign_bonds::assign_bonds(const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3,
                           const using_database &db4, const using_database &db5,
                           const using_database &db6, const using_database &db7,
                           const using_database &db8,
                           const BondGeneratorBase &generator)
             : assign_internals<MolBondInfo>(generator)
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
    addDataBase(db3);
    addDataBase(db4);
    addDataBase(db5);
    addDataBase(db6);
    addDataBase(db7);
    addDataBase(db8);
}

/** Assign the bonds using the requirements specified */
assign_bonds::assign_bonds(const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3,
                           const using_database &db4, const using_database &db5,
                           const using_database &db6, const using_database &db7,
                           const using_database &db8, const using_database &db9,
                           const BondGeneratorBase &generator)
             : assign_internals<MolBondInfo>(generator)
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
    addDataBase(db3);
    addDataBase(db4);
    addDataBase(db5);
    addDataBase(db6);
    addDataBase(db7);
    addDataBase(db8);
    addDataBase(db9);
}

/** Assign the bonds using the requirements specified */
assign_bonds::assign_bonds(const QSet<Bond> &bonds,
                           const BondGeneratorBase &generator)
             : assign_internals<MolBondInfo>(bonds, generator)
{}

/** Assign the bonds using the requirements specified */
assign_bonds::assign_bonds(const QSet<Bond> &bonds,
                           const using_database &db0,
                           const BondGeneratorBase &generator)
             : assign_internals<MolBondInfo>(bonds, generator)
{
    addDataBase(db0);
}

/** Assign the bonds using the requirements specified */
assign_bonds::assign_bonds(const QSet<Bond> &bonds,
                           const using_database &db0, const using_database &db1,
                           const BondGeneratorBase &generator)
             : assign_internals<MolBondInfo>(bonds, generator)
{
    addDataBase(db0);
    addDataBase(db1);
}

/** Assign the bonds using the requirements specified */
assign_bonds::assign_bonds(const QSet<Bond> &bonds,
                           const using_database &db0, const using_database &db1,
                           const using_database &db2,
                           const BondGeneratorBase &generator)
             : assign_internals<MolBondInfo>(bonds, generator)
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
}

/** Assign the bonds using the requirements specified */
assign_bonds::assign_bonds(const QSet<Bond> &bonds,
                           const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3,
                           const BondGeneratorBase &generator)
             : assign_internals<MolBondInfo>(bonds, generator)
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
    addDataBase(db3);
}

/** Assign the bonds using the requirements specified */
assign_bonds::assign_bonds(const QSet<Bond> &bonds,
                           const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3,
                           const using_database &db4,
                           const BondGeneratorBase &generator)
             : assign_internals<MolBondInfo>(bonds, generator)
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
    addDataBase(db3);
    addDataBase(db4);
}

/** Assign the bonds using the requirements specified */
assign_bonds::assign_bonds(const QSet<Bond> &bonds,
                           const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3,
                           const using_database &db4, const using_database &db5,
                           const BondGeneratorBase &generator)
             : assign_internals<MolBondInfo>(bonds, generator)
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
    addDataBase(db3);
    addDataBase(db4);
    addDataBase(db5);
}

/** Assign the bonds using the requirements specified */
assign_bonds::assign_bonds(const QSet<Bond> &bonds,
                           const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3,
                           const using_database &db4, const using_database &db5,
                           const using_database &db6,
                           const BondGeneratorBase &generator)
             : assign_internals<MolBondInfo>(bonds, generator)
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
    addDataBase(db3);
    addDataBase(db4);
    addDataBase(db5);
    addDataBase(db6);
}

/** Assign the bonds using the requirements specified */
assign_bonds::assign_bonds(const QSet<Bond> &bonds,
                           const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3,
                           const using_database &db4, const using_database &db5,
                           const using_database &db6, const using_database &db7,
                           const BondGeneratorBase &generator)
             : assign_internals<MolBondInfo>(bonds, generator)
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
    addDataBase(db3);
    addDataBase(db4);
    addDataBase(db5);
    addDataBase(db6);
    addDataBase(db7);
}

/** Assign the bonds using the requirements specified */
assign_bonds::assign_bonds(const QSet<Bond> &bonds,
                           const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3,
                           const using_database &db4, const using_database &db5,
                           const using_database &db6, const using_database &db7,
                           const using_database &db8,
                           const BondGeneratorBase &generator)
             : assign_internals<MolBondInfo>(bonds, generator)
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
    addDataBase(db3);
    addDataBase(db4);
    addDataBase(db5);
    addDataBase(db6);
    addDataBase(db7);
    addDataBase(db8);
}

/** Assign the bonds using the requirements specified */
assign_bonds::assign_bonds(const QSet<Bond> &bonds,
                           const using_database &db0, const using_database &db1,
                           const using_database &db2, const using_database &db3,
                           const using_database &db4, const using_database &db5,
                           const using_database &db6, const using_database &db7,
                           const using_database &db8, const using_database &db9,
                           const BondGeneratorBase &generator)
             : assign_internals<MolBondInfo>(bonds, generator)
{
    addDataBase(db0);
    addDataBase(db1);
    addDataBase(db2);
    addDataBase(db3);
    addDataBase(db4);
    addDataBase(db5);
    addDataBase(db6);
    addDataBase(db7);
    addDataBase(db8);
    addDataBase(db9);
}

/** Copy constructor */
assign_bonds::assign_bonds(const assign_bonds &other)
             : assign_internals<MolBondInfo>(other)
{}

/** Destructor */
assign_bonds::~assign_bonds()
{}

/** Assign the parameters in the table 'param_table' using
    the database 'database' according to the requirements contained in this object.
    If 'overWrite()' is true then the parameters will be overwritten,
    otherwise only parameters for bonds that are currently missing
    parameters will be found. */
void assign_bonds::assignParameters( const Molecule &molecule,
                                     ParameterTable &param_table,
                                     ParameterDB &database,
                                     const MatchMRData &matchmr ) const
{
    this->assignInternals(molecule, param_table, database, matchmr);
}
