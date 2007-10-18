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

#ifndef SIREMM_CHARGETABLE_H
#define SIREMM_CHARGETABLE_H

#include "chargeparameter.h"

#include "SireDB/atomtable.hpp"

SIRE_BEGIN_HEADER

namespace SireMM
{
class ChargeTable;
}

QDataStream& operator<<(QDataStream&, const SireMM::ChargeTable&);
QDataStream& operator>>(QDataStream&, SireMM::ChargeTable&);

namespace SireMM
{

using SireMol::MoleculeInfo;
using SireMol::CutGroupID;
using SireMol::CGAtomID;
using SireMol::ResNum;
using SireMol::ResNumAtomID;
using SireMol::AtomIndex;

/**
This is the table of partial charge parameters of atoms in a SireMol::Molecule.

@author Christopher Woods
*/
class SIREMM_EXPORT ChargeTable : public SireDB::AtomTableT<ChargeParameter>
{
public:
    ChargeTable();
    ChargeTable(const MoleculeInfo &molinfo);

    ChargeTable(const ChargeTable &other);

    ~ChargeTable();

    ChargeTable* clone() const
    {
        return new ChargeTable(*this);
    }

    ChargeTable* create(const MoleculeInfo &molinfo) const
    {
        return new ChargeTable(molinfo);
    }

    static const char* typeName()
    {
        return "SireMM::ChargeTable";
    }

    const char* what() const
    {
        return ChargeTable::typeName();
    }
};

}

Q_DECLARE_METATYPE(SireMM::ChargeTable)

SIRE_END_HEADER

#endif
