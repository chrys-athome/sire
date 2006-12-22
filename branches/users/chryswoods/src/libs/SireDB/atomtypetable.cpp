
#include "SireMol/qhash_siremol.h"

#include <QDebug>

#include "atomtypetable.h"

#include "SireMol/molecule.h"

#include "SireError/errors.h"
#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireDB;
using namespace SireMol;

/** Register this class so that it can be used with QVariant */
static const RegisterMetaType<AtomTypeTable> r_atomtypetable("SireDB::AtomTypeTable");

/** Serialise to a binary data stream */
QDataStream SIREDB_EXPORT &operator<<(QDataStream &ds, const AtomTypeTable &table)
{
    writeHeader(ds, r_atomtypetable, 1)
          << static_cast<const AtomTableT<AtomType>&>(table);

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREDB_EXPORT &operator>>(QDataStream &ds, AtomTypeTable &table)
{
    VersionID v = readHeader(ds, r_atomtypetable);

    if (v == 1)
    {
        ds >> static_cast<AtomTableT<AtomType>&>(table);
    }
    else
        throw version_error(v, "1", r_atomtypetable, CODELOC);

    return ds;
}

/** Construct an empty AtomTypeTable */
AtomTypeTable::AtomTypeTable() : AtomTableT<AtomType>()
{}

/** Construct a table to hold the parameters of the passed molecule */
AtomTypeTable::AtomTypeTable(const MoleculeInfo &molinfo)
              : AtomTableT<AtomType>(molinfo)
{}

/** Copy constructor. As this class uses implicitly shared data, this copy will be
    very fast and very memory efficient.  */
AtomTypeTable::AtomTypeTable(const AtomTypeTable &other)
              : AtomTableT<AtomType>(other)
{}

/** Destructor */
AtomTypeTable::~AtomTypeTable()
{}
