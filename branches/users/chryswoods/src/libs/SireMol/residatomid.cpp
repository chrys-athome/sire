
#include "residatomid.h"

#include <QDataStream>
#include <QString>

using namespace SireMol;

QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const ResIDAtomID &id)
{
    ds << id.resID() << id.atomID();
    return ds;
}

QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, ResIDAtomID &id)
{
    ds >> id._resid >> id._atmid;
    return ds;
}

QString ResIDAtomID::toString() const
{
    return QString("%1:%2").arg(_resid.toString(),_atmid.toString());
}
