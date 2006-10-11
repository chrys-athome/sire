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
