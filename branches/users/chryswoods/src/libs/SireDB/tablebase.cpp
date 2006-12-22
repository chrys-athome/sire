
#include "tablebase.h"

#include <QDebug>

#include "SireMol/molecule.h"
#include "SireMol/atomindex.h"

#include "SireError/errors.h"
#include "SireStream/datastream.h"

using namespace SireMol;
using namespace SireStream;
using namespace SireDB;

//Register TableBase as a pure virtual class
static const RegisterMetaType<TableBase> r_tablebase("SireDB::TableBase", MAGIC_ONLY);

/** Serialise to a binary data stream */
QDataStream SIREDB_EXPORT &operator<<(QDataStream &ds, const SireDB::TableBase &table)
{
    writeHeader(ds, r_tablebase, 1) << table.molinfo;

    return ds;
}

QDataStream SIREDB_EXPORT &operator>>(QDataStream &ds, SireDB::TableBase &table)
{
    VersionID v = readHeader(ds, r_tablebase);

    if (v == 1)
    {
        ds >> table.molinfo;
    }
    else
        throw version_error(v, "1", r_tablebase, CODELOC);

    return ds;
}

/** Constructor */
TableBase::TableBase()
{}

/** Construct a table to hold the parameters for the molecule
    described by 'moleculeinfo' */
TableBase::TableBase(const MoleculeInfo &moleculeinfo)
          : molinfo(moleculeinfo)
{}

/** Copy constructor */
TableBase::TableBase(const TableBase &other) : molinfo(other.molinfo)
{}

/** Destructor */
TableBase::~TableBase()
{}

/** Return whether or not this table is compatible with the molecule info
    'molinfo' (in other words, molinfo == this->info()) */
bool TableBase::isCompatibleWith(const MoleculeInfo &molinfo) const
{
    return molinfo == this->info();
}

/** Assert that this table is compatible with the molecule 'molecule'

    \throw SireError::incompatible_error
*/
void TableBase::assertCompatibleWith(const Molecule &molecule) const
{
    if ( not this->isCompatibleWith(molecule.info()) )
        throw SireError::incompatible_error( QObject::tr(
              "This table, which holds the parameters for the molecule \"%1\", "
              "is not compatible with the molecule \"%2\".")
                  .arg(molinfo.name(), molecule.name()), CODELOC );
}
