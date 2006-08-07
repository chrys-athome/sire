
#include "resnumatomid.h"

#include <QDataStream>
#include <QString>

using namespace SireMol;

QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const ResNumAtomID &id)
{
    ds << id.resNum() << id.atomID();
    return ds;
}

QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, ResNumAtomID &id)
{
    ds >> id._resnum >> id._atmid;
    return ds;
}

QString ResNumAtomID::toString() const
{
    return QString("%1:%2").arg(_resnum.toString(),_atmid.toString());
}
