
#include "newatom.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMol;
using namespace SireStream;

static const RegisterMetaType<NewAtom> r_newatom;

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const NewAtom &atom)
{
    writeHeader(ds, r_newatom, 1);

    SharedDataStream(ds) << atom.d << atom.cgatomid;

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, NewAtom &atom)
{
    VersionID v = readHeader(ds, r_newatom);

    if (v == 1)
    {
        SharedDataStream(ds) >> atom.d >> atom.cgatomid;
    }
    else
        throw version_error(v, "1", , CODELOC);

    return ds;
}
