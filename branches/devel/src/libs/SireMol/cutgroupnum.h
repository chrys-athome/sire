#ifndef SIREMOL_CUTGROUPNUM_H
#define SIREMOL_CUTGROUPNUM_H

#include "id.h"

SIRE_BEGIN_HEADER

namespace SireMol
{

/** This user-supplied ID number that is used to identify a CutGroup.

    @author Christopher Woods
*/
class SIREMOL_EXPORT CutGroupNum : public IDBase
{

public:
    CutGroupNum() : IDBase()
    {}

    explicit CutGroupNum(quint32 id) : IDBase(id)
    {}

    CutGroupNum(const CutGroupNum &other) : IDBase(other)
    {}

    ~CutGroupNum()
    {}
};

}

/** This is a movable type */
Q_DECLARE_TYPEINFO(SireMol::CutGroupNum, Q_MOVABLE_TYPE);

SIRE_END_HEADER

#endif
