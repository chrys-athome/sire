#ifndef SIREDB_NAMEIDSET_H
#define SIREDB_NAMEIDSET_H

#include <QSet>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireDB
{

/** This type is used to store the index of a name */
typedef quint32 NameID;

/** A set of name indicies */
typedef QSet<NameID> NameIDSet;

QString convertToString(const NameIDSet &set);
NameIDSet convertToNameIDSet(const QString &string);

}

SIRE_END_HEADER

#endif
