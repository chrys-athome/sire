
#include "groupindexid.h"

#include <QDataStream>
#include <QString>

using namespace SireMol;

QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const GroupIndexID &cgatomid)
{
    ds << cgatomid.groupID() << cgatomid.index();
    return ds;
}

QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, GroupIndexID &cgatomid)
{
    ds >> cgatomid._gid >> cgatomid._idx;
    return ds;
}

QString GroupIndexID::toString() const
{
    return QString("%1:%2").arg(_gid.toString(),_idx.toString());
}
