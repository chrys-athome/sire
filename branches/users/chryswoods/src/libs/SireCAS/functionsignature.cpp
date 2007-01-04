
#include "functionsignature.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireCAS;

static const RegisterMetaType<FunctionSignature> r_sig;

/** Serialise to a binary data stream */
QDataStream SIRECAS_EXPORT &operator<<(QDataStream &ds, const FunctionSignature &sig)
{
    writeHeader(ds, r_sig, 1) << sig.name() << sig.args();

    return ds;
}
/** Deserialise from a binary data stream */
QDataStream SIRECAS_EXPORT &operator>>(QDataStream &ds, FunctionSignature &sig)
{
    VersionID v = readHeader(ds, r_sig);

    if (v == 1)
    {
        QString name;
        QSet<SireCAS::SymbolID> args;

        ds >> name >> args;

        sig = SireCAS::FunctionSignature(name,args);
    }
    else
        throw version_error(v, "1", r_sig, CODELOC);

    return ds;
}
