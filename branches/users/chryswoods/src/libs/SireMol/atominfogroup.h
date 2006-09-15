/**
  * @file
  *
  * C++ Interface: AtomInfoGroup
  *
  * Description: Interface for AtomInfoGroup
  *
  *
  * @author Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */
#ifndef SIREMOL_ATOMINFOGROUP_H
#define SIREMOL_ATOMINFOGROUP_H

#include "atominfo.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class AtomInfoGroup;
}

QDataStream& operator<<(QDataStream&, const SireMol::AtomInfoGroup&);
QDataStream& operator>>(QDataStream&, SireMol::AtomInfoGroup&);

namespace SireMol
{

/**
This class holds the group of AtomInfo objects of Atoms in a CutGroup. This class is designed to be used with CutGroup and CoordGroup, and allows the data of a CutGroup to be split into two; the coordinates go to CutGroup while the remaining meta information goes to AtomInfoGroup.

@author Christopher Woods
*/
class SIREMOL_EXPORT AtomInfoGroup
{

friend QDataStream& ::operator<<(QDataStream&, const AtomInfoGroup&);
friend QDataStream& ::operator>>(QDataStream&, AtomInfoGroup&);

public:
    AtomInfoGroup();

    ~AtomInfoGroup();

};

}

SIRE_END_HEADER

Q_DECLARE_METATYPE(SireMol::AtomInfoGroup)

#endif
