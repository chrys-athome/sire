
#include "resnumindexid.h"

#include <QDataStream>
#include <QString>

using namespace SireMol;

QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const ResNumIndexID &id)
{
    ds << id.resNum() << id.index();
    return ds;
}

QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, ResNumIndexID &id)
{
    ds >> id._resnum >> id._idx;
    return ds;
}

QString ResNumIndexID::toString() const
{
    return QString("%1:%2").arg(_resnum.toString(),_idx.toString());
}
