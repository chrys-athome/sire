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

#ifndef SIREMM_BONDTABLE_H
#define SIREMM_BONDTABLE_H

#include "SireCAS/expression.h"

#include "bondtable.hpp"
#include "bondgenerator.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class Molecule;
}

namespace SireMM
{
class BondTable;
}

class QDataStream;
QDataStream& operator<<(QDataStream&, const SireMM::BondTable&);
QDataStream& operator>>(QDataStream&, SireMM::BondTable&);

namespace SireMM
{

using SireCAS::Expression;
using SireMol::Molecule;

/** This class is the predominant BondTable that is used in Sire.
    This class holds the set of bonds in the molecule and maps
    them to the SireCAS::Expression objects that contain the 
    formula that is used to evaluate their energy.
    
    @author Christopher Woods
*/
class SIREDB_EXPORT BondTable : public BondTableT<Expression>
{

friend QDataStream& ::operator<<(QDataStream&, const BondTable&);
friend QDataStream& ::operator>>(QDataStream&, BondTable&);

public:
    BondTable();
    
    BondTable(const MoleculeInfo &molinfo);
    BondTable(const Molecule &mol, const BondGeneratorBase &bondgenerator);
    BondTable(const MolBondInfo &bondinfo);
    
    BondTable(const BondTable &other);
    
    ~BondTable();

    BondTable* clone() const
    {
        return new BondTable(*this);
    }
    
    BondTable* create(const MoleculeInfo &molinfo) const
    {
        return new BondTable(molinfo);
    }
    
    void add(const TableBase &other);
    
    static const char* typeName()
    {
        return "SireMM::BondTable";
    }

    const char* what() const
    {
        return BondTable::typeName();
    }
    
    
    
};

}

Q_DECLARE_METATYPE(SireMM::BondTable)

SIRE_END_HEADER

#endif
