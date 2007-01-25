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

#ifndef SIREMM_LJTABLE_H
#define SIREMM_LJTABLE_H

#include "ljparameter.h"

#include "SireDB/atomtable.hpp"

SIRE_BEGIN_HEADER

namespace SireMM
{
class LJTable;
}

QDataStream& operator<<(QDataStream&, const SireMM::LJTable&);
QDataStream& operator>>(QDataStream&, SireMM::LJTable&);

namespace SireMM
{

using SireMol::MoleculeInfo;
using SireMol::CutGroupID;
using SireMol::CGAtomID;
using SireMol::ResNum;
using SireMol::ResNumAtomID;
using SireMol::AtomIndex;

/**
This is the table of Lennard Jones parameters of atoms in a SireMol::Molecule.

@author Christopher Woods
*/
class SIREMM_EXPORT LJTable : public SireDB::AtomTableT<LJParameter>
{
public:
    LJTable();
    LJTable(const MoleculeInfo &molinfo);

    LJTable(const LJTable &other);

    ~LJTable();

    LJTable* clone() const
    {
        return new LJTable(*this);
    }

    LJTable* create(const MoleculeInfo &molinfo) const
    {
        return new LJTable(molinfo);
    }

    static const char* typeName()
    {
        return "SireMM::LJTable";
    }

    const char* what() const
    {
        return LJTable::typeName();
    }

};

}

Q_DECLARE_METATYPE(SireMM::LJTable)

SIRE_END_HEADER

#endif
