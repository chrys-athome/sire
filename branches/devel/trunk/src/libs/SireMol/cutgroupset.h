#ifndef SIREMOL_CUTGROUPSET_H
#define SIREMOL_CUTGROUPSET_H

#include <QVarLengthArray>
#include <QVector>
#include <QHash>
#include <QList>
#include <QSet>

#include "SireBase/set.hpp"

#include "cutgroup.h"

namespace SireMol
{

/** Define a CutGroupSet class - this is a set of CutGroups indexed by CutGroupID */
typedef SireBase::Set<MolCutGroupID,CutGroup> CutGroupSet;

/** Typedef a list of CutGroups */
typedef QList<CutGroup> CutGroupList;

/** Typedef a vector of CutGroups */
typedef QVector<CutGroup> CutGroupVector;

/** Typedef a set of CutGroupID */
typedef QSet<CutGroupID> CutGroupIDSet;

/** Typedef a fast array class to hold lists of CutGroups - this is a
    low-level QVarLengthArray, which uses the stack to store the first 'n' pointers,
    and the heap for the rest. This should be very fast for listing CutGroups. */
typedef QVarLengthArray<CutGroup, 512> CutGroupArray;

}

#endif
