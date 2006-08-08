
#include "molcutgroupid.h"

#include <QDataStream>

using namespace SireMol;

QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const MolCutGroupID &molcgid)
{
    ds << molcgid.moleculeID() << molcgid.cutGroupID();
    return ds;
}

QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, MolCutGroupID &molcgid)
{
    ds >> molcgid._molid >> molcgid._cgid;
    return ds;
}

QString MolCutGroupID::toString() const
{
    return QString("%1:%2").arg(_molid.toString(),_cgid.toString());
}
