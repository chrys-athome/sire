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
#include "qhash_siremm.h"

#include "dihedraltable.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMM;
using namespace SireMol;
using namespace SireCAS;

static const RegisterMetaType<DihedralTable> r_dihedraltable;

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const DihedralTable &table)
{
    writeHeader(ds, r_dihedraltable, 1)
        << static_cast<const DihedralTableT<Expression>&>(table);

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, DihedralTable &table)
{
    VersionID v = readHeader(ds, r_dihedraltable);

    if (v == 1)
    {
        ds >> static_cast<DihedralTableT<Expression>&>(table);
    }
    else
        throw version_error(v, "1", r_dihedraltable, CODELOC);

    return ds;
}

/** Empty constructor */
DihedralTable::DihedralTable() : DihedralTableT<Expression>()
{}

/** Construct a DihedralTable to hold the expressions for the
    dihedrals in 'dihedralinfo' */
DihedralTable::DihedralTable(const MolDihedralInfo &dihedralinfo)
              : DihedralTableT<Expression>(dihedralinfo)
{}

/** Construct a DihedralTable that holds the dihedrals for the molecule 'mol'. Note that
    no dihedrals will be contained in this object initially. */
DihedralTable::DihedralTable(const MoleculeInfo &molinfo)
              : DihedralTableT<Expression>(molinfo)
{}

/** Construct a DihedralTable to hold the dihedrals in the molecule 'mol' that
    were generated using the dihedral generator 'generator' */
DihedralTable::DihedralTable(const Molecule &mol, const DihedralGeneratorBase &generator)
          : DihedralTableT<Expression>( generator.generate(mol) )
{}

/** Copy constructor */
DihedralTable::DihedralTable(const DihedralTable &other) : DihedralTableT<Expression>(other)
{}

/** Destructor */
DihedralTable::~DihedralTable()
{}

/** Add the contents of the other table 'other' to this table. This will throw
    an exception if this table is of the wrong type, or if it is incompatible
    with this table.

    The parameters of 'other' will overwrite the parameters in this table.

    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void DihedralTable::add(const TableBase &other)
{
    //is 'other' a DihedralTable?
    if (not other.isA<DihedralTable>())
        throw SireError::invalid_cast( QObject::tr(
                    "Cannot add a %1 to a DihedralTable!").arg(other.what()), CODELOC );

    const DihedralTable &other_dihedral = other.asA<DihedralTable>();

    //are these tables compatible?
    if (info().info() != other_dihedral.info().info())
        throw SireError::incompatible_error( QObject::tr(
                    "Cannot add the table as the molecules are incompatible."),
                                  CODELOC );

    //add each dihedral parameter from 'other' to this table
    for (MolDihedralInfo::const_iterator it = other_dihedral.info().begin();
         it.isValid();
         ++it)
    {
        const Dihedral &dihedral = it.key();

        if (other_dihedral.assignedParameter(dihedral))
            //set the parameter for this dihedral
            this->setParameter( dihedral, other_dihedral[dihedral] );
        else
            //just add the dihedral
            this->addDihedral(dihedral);
    }
}
