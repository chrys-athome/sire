
#include "molcgatomid.h"

#include <QDataStream>

using namespace SireMol;

QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const MolCGAtomID &id)
{
    ds << id.moleculeID() << id.cutGroupID() << id.atomID();
    return ds;
}

QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, MolCGAtomID &id)
{
    ds >> id._molid >> id._cgid >> id._atmid;
    return ds;
}

QString MolCGAtomID::toString() const
{
    return QString("%1:%2:%3").arg(_molid.toString(),
                                   _cgid.toString(),
                                   _atmid.toString());
}
