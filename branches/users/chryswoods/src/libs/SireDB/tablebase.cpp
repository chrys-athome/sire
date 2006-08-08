
#include "tablebase.h"

#include <QDebug>

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireDB;

//Register TableBase as a pure virtual class
static const RegisterMetaType<TableBase> r_tablebase("SireDB::TableBase", MAGIC_ONLY);

/** Serialise to a binary data stream */
QDataStream SIREDB_EXPORT &operator<<(QDataStream &ds, const SireDB::TableBase&)
{
    writeHeader(ds, r_tablebase, 0);
    
    return ds;
}

QDataStream SIREDB_EXPORT &operator>>(QDataStream &ds, SireDB::TableBase&)
{
    VersionID v = readHeader(ds, r_tablebase);
    
    if (v != 0)
        throw version_error(v, "0", r_tablebase, CODELOC);

    return ds;
}

/** Constructor */
TableBase::TableBase()
{}

/** Copy constructor */
TableBase::TableBase(const TableBase&)
{}

/** Destructor */
TableBase::~TableBase()
{}
