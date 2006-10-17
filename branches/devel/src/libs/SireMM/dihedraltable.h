#ifndef SIREMM_DIHEDRALTABLE_H
#define SIREMM_DIHEDRALTABLE_H

#include "SireCAS/expression.h"

#include "dihedraltable.hpp"
#include "dihedralgenerator.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class Molecule;
}

namespace SireMM
{
class DihedralTable;
}

class QDataStream;
QDataStream& operator<<(QDataStream&, const SireMM::DihedralTable&);
QDataStream& operator>>(QDataStream&, SireMM::DihedralTable&);

namespace SireMM
{

using SireCAS::Expression;

using SireMol::Molecule;
using SireMol::MoleculeInfo;

/** This class is the predominant DihedralTable that is used in Sire.
    This class holds the set of dihedrals in the molecule and maps
    them to the SireCAS::Expression objects that contain the
    formula that is used to evaluate their energy.

    @author Christopher Woods
*/
class SIREDB_EXPORT DihedralTable : public DihedralTableT<Expression>
{

friend QDataStream& ::operator<<(QDataStream&, const DihedralTable&);
friend QDataStream& ::operator>>(QDataStream&, DihedralTable&);

public:
    DihedralTable();

    DihedralTable(const MoleculeInfo &mol);
    DihedralTable(const Molecule &mol, const DihedralGeneratorBase &dihedralgenerator);
    DihedralTable(const MolDihedralInfo &dihedralinfo);

    DihedralTable(const DihedralTable &other);

    ~DihedralTable();

    DihedralTable* clone() const
    {
        return new DihedralTable(*this);
    }

    DihedralTable* create(const MoleculeInfo &molinfo) const
    {
        return new DihedralTable(molinfo);
    }

    void add(const TableBase &other);

    static const char* typeName()
    {
        return "SireMM::DihedralTable";
    }

    const char* what() const
    {
        return DihedralTable::typeName();
    }



};

}

Q_DECLARE_METATYPE(SireMM::DihedralTable)

SIRE_END_HEADER

#endif
