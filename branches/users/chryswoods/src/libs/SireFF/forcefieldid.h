#ifndef SIREFF_FORCEFIELDID_H
#define SIREFF_FORCEFIELDID_H

#include "SireMol/id.h"

SIRE_BEGIN_HEADER

namespace SireFF
{

/** This ID number is used to identify a ForceField.

    @author Christopher Woods
*/
class SIREFF_EXPORT ForceFieldID : public SireMol::IDBase
{

public:
    ForceFieldID() : IDBase()
    {}

    ForceFieldID(quint32 id) : IDBase(id)
    {}

    ForceFieldID(const ForceFieldID &other) : IDBase(other)
    {}

    ~ForceFieldID()
    {}
};

}

/** This is a movable type */
Q_DECLARE_TYPEINFO(SireFF::ForceFieldID, Q_MOVABLE_TYPE);

SIRE_END_HEADER

#endif
