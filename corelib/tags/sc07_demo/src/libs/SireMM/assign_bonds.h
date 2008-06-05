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

#ifndef SIREMM_ASSIGN_BONDS_H
#define SIREMM_ASSIGN_BONDS_H

#include "assign_internals.hpp"
#include "molbondinfo.h"
#include "bondgenerator.h"

SIRE_BEGIN_HEADER

namespace SireMM
{
class assign_bonds;
}

QDataStream& operator<<(QDataStream&, const SireMM::assign_bonds&);
QDataStream& operator>>(QDataStream&, SireMM::assign_bonds&);

namespace SireMM
{


using SireMol::Bond;

/**
This is to Bond as assign_atoms is to Atom - namely this is the class used to control the assignment of bond parameters.
	
@author Christopher Woods
*/
class SIREMM_EXPORT assign_bonds : public assign_internals<MolBondInfo>
{

friend QDataStream& ::operator<<(QDataStream&, const assign_bonds&);
friend QDataStream& ::operator>>(QDataStream&, assign_bonds&);

public:
    assign_bonds(const BondGeneratorBase &generator = BondGenerator());
  
    assign_bonds(const using_database &db0,
                 const BondGeneratorBase &generator = BondGenerator());
  
    assign_bonds(const using_database &db0, const using_database &db1,
                 const BondGeneratorBase &generator = BondGenerator());
  
    assign_bonds(const using_database &db0, const using_database &db1,
                 const using_database &db2,
                 const BondGeneratorBase &generator = BondGenerator());
  
    assign_bonds(const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3,
                 const BondGeneratorBase &generator = BondGenerator());
  
    assign_bonds(const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3,
                 const using_database &db4,
                 const BondGeneratorBase &generator = BondGenerator());
  
    assign_bonds(const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3,
                 const using_database &db4, const using_database &db5,
                 const BondGeneratorBase &generator = BondGenerator());
  
    assign_bonds(const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3,
                 const using_database &db4, const using_database &db5,
                 const using_database &db6,
                 const BondGeneratorBase &generator = BondGenerator());
  
    assign_bonds(const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3,
                 const using_database &db4, const using_database &db5,
                 const using_database &db6, const using_database &db7,
                 const BondGeneratorBase &generator = BondGenerator());
  
    assign_bonds(const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3,
                 const using_database &db4, const using_database &db5,
                 const using_database &db6, const using_database &db7,
                 const using_database &db8,
                 const BondGeneratorBase &generator = BondGenerator());
  
    assign_bonds(const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3,
                 const using_database &db4, const using_database &db5,
                 const using_database &db6, const using_database &db7,
                 const using_database &db8, const using_database &db9,
                 const BondGeneratorBase &generator = BondGenerator());
  
    assign_bonds(const QSet<Bond> &bonds,
                 const BondGeneratorBase &generator = BondGenerator());
    
    assign_bonds(const QSet<Bond> &bonds,
                 const using_database &db0,
                 const BondGeneratorBase &generator = BondGenerator());
    
    assign_bonds(const QSet<Bond> &bonds,
                 const using_database &db0, const using_database &db1,
                 const BondGeneratorBase &generator = BondGenerator());
  
    assign_bonds(const QSet<Bond> &bonds,
                 const using_database &db0, const using_database &db1,
                 const using_database &db2,
                 const BondGeneratorBase &generator = BondGenerator());
  
    assign_bonds(const QSet<Bond> &bonds,
                 const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3,
                 const BondGeneratorBase &generator = BondGenerator());
  
    assign_bonds(const QSet<Bond> &bonds,
                 const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3,
                 const using_database &db4,
                 const BondGeneratorBase &generator = BondGenerator());
  
    assign_bonds(const QSet<Bond> &bonds,
                 const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3,
                 const using_database &db4, const using_database &db5,
                 const BondGeneratorBase &generator = BondGenerator());
  
    assign_bonds(const QSet<Bond> &bonds,
                 const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3,
                 const using_database &db4, const using_database &db5,
                 const using_database &db6,
                 const BondGeneratorBase &generator = BondGenerator());
  
    assign_bonds(const QSet<Bond> &bonds,
                 const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3,
                 const using_database &db4, const using_database &db5,
                 const using_database &db6, const using_database &db7,
                 const BondGeneratorBase &generator = BondGenerator());
  
    assign_bonds(const QSet<Bond> &bonds,
                 const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3,
                 const using_database &db4, const using_database &db5,
                 const using_database &db6, const using_database &db7,
                 const using_database &db8,
                 const BondGeneratorBase &generator = BondGenerator());
  
    assign_bonds(const QSet<Bond> &bonds,
                 const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3,
                 const using_database &db4, const using_database &db5,
                 const using_database &db6, const using_database &db7,
                 const using_database &db8, const using_database &db9,
                 const BondGeneratorBase &generator = BondGenerator());
    
    assign_bonds(const assign_bonds &other);

    ~assign_bonds();

    assign_bonds* clone() const
    {
        return new assign_bonds(*this);
    }

    static const char* typeName()
    {
        return "SireDB::assign_bonds";
    }
    
    const char* what() const
    {
        return assign_bonds::typeName();
    }

    void assignParameters( const Molecule &molecule,
                           ParameterTable &param_table, 
                           ParameterDB &database,
                           const MatchMRData &matchmr = MatchMRData() ) const;
};

}

Q_DECLARE_METATYPE(SireMM::assign_bonds)

SIRE_END_HEADER

#endif
