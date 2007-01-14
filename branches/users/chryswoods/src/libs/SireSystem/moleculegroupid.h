#ifndef SIRESYSTEM_MOLECULEGROUPID_H
#define SIRESYSTEM_MOLECULEGROUPID_H

#include "SireMol/id.h"

SIRE_BEGIN_HEADER

namespace SireSystem
{

/** This ID number is used to identify a MoleculeGroup.

    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT MoleculeGroupID : public SireMol::IDBase
{

public:
    MoleculeGroupID() : IDBase()
    {}

    MoleculeGroupID(quint32 id) : IDBase(id)
    {}

    MoleculeGroupID(const MoleculeGroupID &other) : IDBase(other)
    {}

    ~MoleculeGroupID()
    {}
};

}

/** This is a movable type */
Q_DECLARE_TYPEINFO(SireSystem::MoleculeGroupID, Q_MOVABLE_TYPE);

SIRE_END_HEADER

#endif
