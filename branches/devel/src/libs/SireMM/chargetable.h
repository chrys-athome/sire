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
