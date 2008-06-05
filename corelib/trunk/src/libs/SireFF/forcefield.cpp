
#include "forcefield.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireFF;

static const RegisterMetaType<ForceField> r_forcefield("SireFF::ForceField", MAGIC_ONLY);

/** Serialise to a binary data stream */
QDataStream SIREFF_EXPORT &operator<<(QDataStream &ds, const ForceField&)
{
    writeHeader(ds, r_forcefield, 0);
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREFF_EXPORT &operator>>(QDataStream &ds, ForceField&)
{
    VersionID v = readHeader(ds, r_forcefield);
    
    if (v != 0)
        throw version_error(v, "1", r_forcefield, CODELOC);
    
    return ds;
}

ForceField::ForceField()
{}

ForceField::~ForceField()
{}
