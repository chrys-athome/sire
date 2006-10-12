#ifndef SIREMM_ANGLETABLE_H
#define SIREMM_ANGLETABLE_H

#include "SireCAS/expression.h"

#include "angletable.hpp"
#include "anglegenerator.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class Molecule;
class MoleculeInfo;
}

namespace SireMM
{
class AngleTable;
}

class QDataStream;
QDataStream& operator<<(QDataStream&, const SireMM::AngleTable&);
QDataStream& operator>>(QDataStream&, SireMM::AngleTable&);

namespace SireMM
{

using SireCAS::Expression;
using SireMol::Molecule;
using SireMol::MoleculeInfo;

/** This class is the predominant AngleTable that is used in Sire.
    This class holds the set of angles in the molecule and maps
    them to the SireCAS::Expression objects that contain the 
    formula that is used to evaluate their energy.
    
    @author Christopher Woods
*/
class SIREDB_EXPORT AngleTable : public AngleTableT<Expression>
{

friend QDataStream& ::operator<<(QDataStream&, const AngleTable&);
friend QDataStream& ::operator>>(QDataStream&, AngleTable&);

public:
    AngleTable();
    
    AngleTable(const MoleculeInfo &mol);
    AngleTable(const Molecule &molecule, const AngleGeneratorBase &anglegenerator);
    AngleTable(const MolAngleInfo &angleinfo);
    
    AngleTable(const AngleTable &other);
    
    ~AngleTable();

    AngleTable* clone() const
    {
        return new AngleTable(*this);
    }
    
    AngleTable* create(const MoleculeInfo &molinfo) const
    {
        return new AngleTable(molinfo);
    }
    
    void add(const TableBase &other);
    
    static const char* typeName()
    {
        return "SireMM::AngleTable";
    }

    const char* what() const
    {
        return AngleTable::typeName();
    }
    
    
    
};

}

Q_DECLARE_METATYPE(SireMM::AngleTable)

SIRE_END_HEADER

#endif
