#ifndef SIREMOL_GROUPID_H
#define SIREMOL_GROUPID_H

#include "id.h"

SIRE_BEGIN_HEADER

namespace SireMol
{

/** This ID number is used to identify a Group (e.g. a group
    of bonds or angles)
    
    @author Christopher Woods
*/
class SIREMOL_EXPORT GroupID : public ID<GroupID>
{

public:
    GroupID() : ID<GroupID>()
    {}
    
    GroupID(quint32 id) : ID<GroupID>(id)
    {}
    
    GroupID(const GroupID &other) : ID<GroupID>(other)
    {}
    
    ~GroupID()
    {}
};

}

/** This is a movable type */
Q_DECLARE_TYPEINFO(SireMol::GroupID, Q_MOVABLE_TYPE);

SIRE_END_HEADER

#endif
