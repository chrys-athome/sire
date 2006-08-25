
#include "movedmols.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireFF;

static const RegisterMetaType<SireFF::MovedMols> r_movedmols("SireFF::MovedMols");

/** Serialise to a binary data stream */
QDataStream SIREFF_EXPORT &operator<<(QDataStream &ds, const MovedMols &movedmols)
{
    writeHeader(ds, r_movedmols, 1) << ;
    
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREFF_EXPORT &operator>>(QDataStream &ds, MovedMols &movedmols)
{
    VersionID v = readHeader(ds, r_movedmols);
    
    if (v == 1)
    {
    }
    else
        throw version_error(v, "1", r_movedmols, CODELOC);
        
    return ds;
}

/** Empty constructor */
MovedMols::MovedMols()
{}

/** Destructor */
MovedMols::~MovedMols()
{}


