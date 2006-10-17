#ifndef SIREMOL_CUTGROUPID_H
#define SIREMOL_CUTGROUPID_H

#include "id.h"

SIRE_BEGIN_HEADER

namespace SireMol
{

/** This ID number is used to identify a CutGroup.

    @author Christopher Woods
*/
class SIREMOL_EXPORT CutGroupID : public IDBase
{

public:
    CutGroupID() : IDBase()
    {}

    explicit CutGroupID(quint32 id) : IDBase(id)
    {}

    CutGroupID(const CutGroupID &other) : IDBase(other)
    {}

    ~CutGroupID()
    {}
};

}

/** This is a movable type */
Q_DECLARE_TYPEINFO(SireMol::CutGroupID, Q_MOVABLE_TYPE);

SIRE_END_HEADER

#endif
