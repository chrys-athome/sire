
#include "SireMol/qhash_siremol.h"

#include "ljtable.h"

#include "SireMol/molecule.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMM;
using namespace SireDB;
using namespace SireMol;

/** Register this as a concrete class */
static const RegisterMetaType<LJTable> r_ljtable;

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const LJTable &table)
{
    writeHeader(ds, r_ljtable, 1)
            << static_cast<const AtomTableT<LJParameter>&>(table);

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, LJTable &table)
{
    VersionID v = readHeader(ds, r_ljtable);

    if (v == 1)
    {
        ds >> static_cast<AtomTableT<LJParameter>&>(table);
    }
    else
        throw version_error(v, "1", r_ljtable, CODELOC);

    return ds;
}

/** Null Constructor */
LJTable::LJTable() : AtomTableT<LJParameter>()
{}

/** Construct a table to hold the ljs for the Molecule
    described by 'molinfo' */
LJTable::LJTable(const MoleculeInfo &molinfo)
            : AtomTableT<LJParameter>(molinfo)
{}

/** Copy constructor - this is fast as implicit sharing is used */
LJTable::LJTable(const LJTable &other)
            : AtomTableT<LJParameter>(other)
{}

/** Destructor */
LJTable::~LJTable()
{}
