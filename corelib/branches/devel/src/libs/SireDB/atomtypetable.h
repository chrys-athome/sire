/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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

#ifndef SIREDB_ATOMTYPETABLE_H
#define SIREDB_ATOMTYPETABLE_H

#include "atomtype.h"

#include "atomtable.hpp"

SIRE_BEGIN_HEADER

namespace SireDB
{
class AtomTypeTable;
}

QDataStream& operator<<(QDataStream&, const SireDB::AtomTypeTable&);
QDataStream& operator>>(QDataStream&, SireDB::AtomTypeTable&);

namespace SireDB
{

/**
This is a table of AtomTypes of atoms in a SireMol::Molecule.

@author Christopher Woods
*/
class SIREDB_EXPORT AtomTypeTable : public AtomTableT<AtomType>
{

public:
    AtomTypeTable();
    AtomTypeTable(const MoleculeInfo &molinfo);

    AtomTypeTable(const AtomTypeTable &other);

    ~AtomTypeTable();

    AtomTypeTable* clone() const
    {
        return new AtomTypeTable(*this);
    }

    AtomTypeTable* create(const MoleculeInfo &molinfo) const
    {
        return new AtomTypeTable(molinfo);
    }

    static const char* typeName()
    {
        return "SireDB::AtomTypeTable";
    }

    const char* what() const
    {
        return AtomTypeTable::typeName();
    }
};

}

Q_DECLARE_METATYPE(SireDB::AtomTypeTable)

SIRE_END_HEADER

#endif
