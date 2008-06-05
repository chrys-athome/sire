#ifndef SIREMOL_BONDSET_H
#define SIREMOL_BONDSET_H

#include <QSet>
#include <QList>

namespace SireMol
{

class Bond;

typedef QSet<Bond> BondSet;
typedef QList<Bond> BondList;

}

#endif
