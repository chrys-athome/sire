#ifndef SIREMOL_MOLECULEGROUPSET_H
#define SIREMOL_MOLECULEGROUPSET_H

#include <QList>
#include <QVector>

#include "SireBase/set.hpp"

#include "molecule.h"

namespace SireMol
{

typedef SireBase::Set<MoleculeID, Molecule> MoleculeSet;

typedef QList<Molecule> MoleculeList;
typedef QVector<Molecule> MoleculeVector;

}

#endif
