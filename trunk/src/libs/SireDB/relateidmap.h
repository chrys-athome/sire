#ifndef SIREDB_RELATEIDMAP_H
#define SIREDB_RELATEIDMAP_H

#include <QMap>
#include <QMapIterator>

namespace SireDB
{

/** This is the ID number that is used to uniquely identify 
    each parameter */
typedef quint32 ParamID;

/** Typedef an unsigned integer as the type used internally to identify 
    relationships. Relationships are used to match atoms/atomgroups to 
    forcefield parameters. */
typedef quint32 RelateID;

/** This type is used to store the ID numbers of matching relationships,
    indexed by the score of the relationship */
typedef QMap<qint32, RelateID> RelateIDMap;

/** This is the iterator type over a RelateIDMap */
typedef QMapIterator<qint32, RelateID> RelateIDMapIterator;

}

#endif
