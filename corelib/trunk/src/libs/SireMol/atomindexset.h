#ifndef SIREMOL_ATOMINDEXLIST_H
#define SIREMOL_ATOMINDEXLIST_H

#include <QList>
#include <QSet>
#include <QVector>

#include "atomindex.h"

namespace SireMol
{

typedef QList<AtomIndex> AtomIndexList;
typedef QSet<AtomIndex> AtomIndexSet;
typedef QVector<AtomIndex> AtomIndexVector;

typedef QList<ResNum> ResNumList;
typedef QSet<ResNum> ResNumSet;
typedef QVector<ResNum> ResNumVector;

}

#endif
