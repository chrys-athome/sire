#ifndef SIRESYSTEM_SYSTEMID_H
#define SIRESYSTEM_SYSTEMID_H

#include "SireMol/id.h"

SIRE_BEGIN_HEADER

namespace SireSystem
{

/** This ID number is used to identify a System.

    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT SystemID : public SireMol::IDBase
{

public:
    SystemID() : IDBase()
    {}

    SystemID(quint32 id) : IDBase(id)
    {}

    SystemID(const SystemID &other) : IDBase(other)
    {}

    ~SystemID()
    {}
};

}

/** This is a movable type */
Q_DECLARE_TYPEINFO(SireSystem::SystemID, Q_MOVABLE_TYPE);

SIRE_END_HEADER

#endif
