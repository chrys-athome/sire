
#include "residueidset.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMol;

static const RegisterMetaType<ResidueID> r_residueid("SireMol::ResidueID");

/** Serialise to a binary data stream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const ResidueID &resid)
{
    writeHeader(ds, r_residueid, 1) << resid.name() << resid.number();
    
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, ResidueID &resid)
{
    VersionID v = readHeader(ds, r_residueid);
    
    if (v == 1)
    {
        ds >> resid.rname >> resid.rnum;
    }
    else
        throw version_error(v, "1", r_residueid, CODELOC);
    
    return ds;
}

uint SIREMOL_EXPORT qHash(const SireMol::ResidueID &resnam)
{
    quint32 namehash = qHash(resnam.name());
    quint32 reshash = qHash(resnam.number());

    return (namehash << 16) | (reshash & 0x0000FFFF);
}

template<>
SireMol::ResNum SIREMOL_EXPORT set_indexer(const SireMol::ResidueID &resnam)
{
    return resnam.number();
}
