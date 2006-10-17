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
