
#include "cgatomid.h"

#include <QDataStream>
#include <QString>
#include <QDebug>

using namespace SireMol;

/** primitive type streaming function */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const CGAtomID &cgatomid)
{
    ds << cgatomid.cutGroupID() << cgatomid.atomID();
    
    return ds;
}

/** primitive type streaming function */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, CGAtomID &cgatomid)
{
    ds >> cgatomid._cgid >> cgatomid._atmid;
    return ds;
}

QString CGAtomID::toString() const
{
    return QString("%1:%2").arg(_cgid.toString(),_atmid.toString());
}
