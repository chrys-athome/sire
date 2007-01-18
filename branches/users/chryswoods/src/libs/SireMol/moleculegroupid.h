#ifndef SIREMOL_MOLECULEGROUPID_H
#define SIREMOL_MOLECULEGROUPID_H

#include "id.h"

SIRE_BEGIN_HEADER

namespace SireMol
{

/** This ID number is used to identify a MoleculeGroup.

    @author Christopher Woods
*/
class SIREMOL_EXPORT MoleculeGroupID : public IDBase
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
Q_DECLARE_TYPEINFO(SireMol::MoleculeGroupID, Q_MOVABLE_TYPE);

SIRE_END_HEADER

#endif
