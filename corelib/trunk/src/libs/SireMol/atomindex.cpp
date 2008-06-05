
#include "atomindex.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMol;

static const RegisterMetaType<AtomIndex> r_atomindex("SireMol::AtomIndex");

/** Function to serialise an AtomIndex to a binary data stream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const SireMol::AtomIndex &atm)
{
    writeHeader(ds, r_atomindex, 1) << atm.nme << atm.resnum;
    
    return ds;
}

/** Function to deserialise an AtomIndex from a binary data stream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, SireMol::AtomIndex &atm)
{
    VersionID v = readHeader(ds, r_atomindex);
    
    if (v == 1)
    {
        ds >> atm.nme >> atm.resnum;
    }
    else
        throw version_error(v, "1", r_atomindex, CODELOC);
    
    return ds;
}

/** Function to construct an AtomIndex from a boost tuple of the name and residue number */
AtomIndex::AtomIndex( const tuple<QString,ResNum> &tuple )
          : nme( tuple.get<0>() ), resnum( tuple.get<1>() )
{}

/** Return a string representation of the AtomIndex */
QString AtomIndex::toString() const
{
    return QString("(\"%1\",%2)").arg(nme,resnum.toString());
}
