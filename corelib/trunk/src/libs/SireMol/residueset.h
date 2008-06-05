#ifndef SIREMOL_RESIDUESET_H
#define SIREMOL_RESIDUESET_H

#include <QList>
#include <QVector>

#include "SireBase/set.hpp"

#include "atomindex.h"

namespace SireMol
{
class Residue;
}

namespace SireMol
{

typedef SireBase::Set<ResNum, Residue> ResidueSet;

typedef QList<Residue> ResidueList;
typedef QVector<Residue> ResidueVector;

}

#endif
