
#include "cgnumatomid.h"

#include <QDataStream>
#include <QString>
#include <QDebug>

using namespace SireMol;

/** primitive type streaming function */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const CGNumAtomID &cgatomid)
{
    ds << cgatomid.cutGroupNum() << cgatomid.atomID();

    return ds;
}

/** primitive type streaming function */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, CGNumAtomID &cgatomid)
{
    ds >> cgatomid._cgnum >> cgatomid._atmid;
    return ds;
}

QString CGNumAtomID::toString() const
{
    return QString("%1:%2").arg(_cgnum.toString(),_atmid.toString());
}
