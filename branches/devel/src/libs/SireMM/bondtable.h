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
    
    BondTable(const Molecule &mol);
    BondTable(const Molecule &mol, const BondGeneratorBase &bondgenerator);
    BondTable(const MolBondInfo &bondinfo);
    
    BondTable(const BondTable &other);
    
    ~BondTable();

    BondTable* clone() const
    {
        return new BondTable(*this);
    }
    
    BondTable* create(const Molecule &molecule) const
    {
        return new BondTable(molecule);
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
