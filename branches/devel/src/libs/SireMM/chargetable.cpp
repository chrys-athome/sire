
#include "SireMol/qhash_siremol.h"

#include "chargetable.h"

#include "SireMol/moleculeinfo.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMM;
using namespace SireDB;
using namespace SireMol;

/** Register this as a concrete class */
static const RegisterMetaType<ChargeTable> r_chargetable("SireMM::ChargeTable");

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const ChargeTable &table)
{
    writeHeader(ds, r_chargetable, 1)
            << static_cast<const AtomTableT<ChargeParameter>&>(table);

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, ChargeTable &table)
{
    VersionID v = readHeader(ds, r_chargetable);

    if (v == 1)
    {
        ds >> static_cast<AtomTableT<ChargeParameter>&>(table);
    }
    else
        throw version_error(v, "1", r_chargetable, CODELOC);

    return ds;
}

/** Null Constructor */
ChargeTable::ChargeTable() : AtomTableT<ChargeParameter>()
{}

/** Construct a table to hold the charges for the Molecule
    described by 'molinfo' */
ChargeTable::ChargeTable(const MoleculeInfo &molinfo)
            : AtomTableT<ChargeParameter>(molinfo)
{}

/** Copy constructor - this is fast as implicit sharing is used */
ChargeTable::ChargeTable(const ChargeTable &other)
            : AtomTableT<ChargeParameter>(other)
{}

/** Destructor */
ChargeTable::~ChargeTable()
{}
